//
//  HttpClient.cpp
//  Downloader
//
//  Created by Xu Xu on 1/8/14.
//  Copyright (c) 2014 Xu Xu. All rights reserved.
//

#include "HttpClient.h"

HttpClient::HttpClient() : m_requesting(false)
{
    // init global
    static bool globalInitialized = false;
    if (!globalInitialized)
    {
        curl_global_init(CURL_GLOBAL_ALL);
        globalInitialized = true;
    }
    
    // init current session
    m_curl = curl_easy_init();
    
//    curl_easy_setopt(m_curl, CURLOPT_HTTPAUTH, CURLAUTH_ANY);	// support basic, digest, and NTLM authentication
    // try not to use signals
    curl_easy_setopt(m_curl, CURLOPT_NOSIGNAL, 1L);
    
    // set a default user agent
    curl_easy_setopt(m_curl, CURLOPT_USERAGENT, curl_version());
}

HttpClient::HttpClient(string agent) : HttpClient()
{
    curl_easy_setopt(m_curl, CURLOPT_USERAGENT, agent.c_str());
}


HttpClient::~HttpClient()
{
    curl_easy_cleanup(m_curl);
}

bool HttpClient::isRequesting()
{
    return m_requesting;
}

bool HttpClient::download(string url, string filepath)
{
    // set url
    curl_easy_setopt(m_curl, CURLOPT_URL, url.c_str());
    
#ifdef DEBUG_MODE
    // Switch on full protocol/debug output while testing
    curl_easy_setopt(m_curl, CURLOPT_VERBOSE, 1L);
    
    // disable progress meter, set to 0L to enable and disable debug output
    curl_easy_setopt(m_curl, CURLOPT_NOPROGRESS, 1L);
#endif
    
    // forward all data to this func
    curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, &HttpClient::onReceived);
    
    // open the file
    FILE * file = fopen(filepath.c_str(), "wb");
    if (file)
    {
        // write the page body to this file handle. CURLOPT_FILE is also known as CURLOPT_WRITEFILE
        curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, file);
        
        // do it
        curl_easy_perform(m_curl);
        
        fclose(file);
        
        return true;
    }
    
    return false;
}

size_t HttpClient::onReceived(void *ptr, size_t size, size_t nmemb, void *stream)
{
    return fwrite(ptr, size, nmemb, (FILE*) stream);
}
