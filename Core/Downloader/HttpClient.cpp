/*****************************************************************************
 Copyright (c) 2014 Xu Xu (Vivo)
 
 http://git.xuxu.name/downloader
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HttpClient.h"

#pragma mark - Constructor & Destructor

HttpClient::HttpClient()
{
    initCURL();
}

HttpClient::HttpClient(string agent) : HttpClient()
{
    curl_easy_setopt(m_curl, CURLOPT_USERAGENT, agent.c_str());
}


HttpClient::~HttpClient()
{
    curl_easy_cleanup(m_curl);
}

#pragma mark - Public Methods

string HttpClient::get(string url)
{
    // set url
    curl_easy_setopt(m_curl, CURLOPT_URL, url.c_str());
    
    // forward all data to this func
    curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, &HttpClient::writeToString);
    
    string response;
    
    curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, &response);
    
    // do it
    curl_easy_perform(m_curl);
    
    return response;
}

bool HttpClient::download(string url, string folder)
{
    // try guess filename from the url
    auto pos = find(url.rbegin(), url.rend(), '/');
    string name = url.substr(distance(pos, url.rend()));
    
    if (folder.back() != kPathSeparator) folder += kPathSeparator;
    
    return downloadAs(url, folder + name);
}

bool HttpClient::downloadAs(string url, string filepath)
{
    // set url
    curl_easy_setopt(m_curl, CURLOPT_URL, url.c_str());
    
    // forward all data to this func
    curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, &HttpClient::writeToFile);
    
    // try to write a temp file instead of the real file
    string tmpFile = filepath + ".tmp";
    
    // open the file
    FILE * file = fopen(tmpFile.c_str(), "wb");
    if (file)
    {
        // write the page body to this file handle. CURLOPT_FILE is also known as CURLOPT_WRITEFILE
        curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, file);
        
        // do it
        CURLcode result = curl_easy_perform(m_curl);
        
        fclose(file);
        
        // get HTTP response code
        int responseCode;
        curl_easy_getinfo(m_curl, CURLINFO_RESPONSE_CODE, &responseCode);
        
        if (result == CURLE_OK && responseCode == 200) // only assume 200 is correct
        {
            // rename it to real filename
            rename(tmpFile.c_str(), filepath.c_str());
            return true;
        }
        
        remove(tmpFile.c_str()); // clean tmp file
    }
    
    return false;
}

#pragma mark - Data Receiver

size_t HttpClient::writeToFile(void * ptr, size_t size, size_t nmemb, FILE * stream)
{
    return fwrite(ptr, size, nmemb, stream);
}

size_t HttpClient::writeToString(char * ptr, size_t size, size_t nmemb, string * sp)
{
    size_t len = size * nmemb;
    sp->insert(sp->end(), ptr, ptr + len);
    return len;
}

#pragma mark - Private Helper

void HttpClient::initCURL()
{
    // init global
    static bool globalInitialized = false;
    if (!globalInitialized)
    {
        curl_global_init(CURL_GLOBAL_ALL); // there won't be curl_global_cleanup();
        globalInitialized = true;
    }
    
    // init current session
    m_curl = curl_easy_init();
    
    //    curl_easy_setopt(m_curl, CURLOPT_HTTPAUTH, CURLAUTH_ANY);	// support basic, digest, and NTLM authentication
    // try not to use signals
    curl_easy_setopt(m_curl, CURLOPT_NOSIGNAL, 1L);
    
    // set a default user agent
    curl_easy_setopt(m_curl, CURLOPT_USERAGENT, curl_version());
    
#ifdef DEBUG_MODE
    // Switch on full protocol/debug output while testing
    curl_easy_setopt(m_curl, CURLOPT_VERBOSE, 1L);
    
    // disable progress meter, set to 0L to enable and disable debug output
    curl_easy_setopt(m_curl, CURLOPT_NOPROGRESS, 1L);
#endif
}
