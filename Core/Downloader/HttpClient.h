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

#ifndef __Downloader__HttpClient__
#define __Downloader__HttpClient__

#include "Common.h"
#include <curl/curl.h>

/**
 Simple HTTP Client. Only one request at the same time.
 */
class HttpClient
{
public:
    HttpClient();
    HttpClient(string agent);
    virtual ~HttpClient();
    
    string get(string url);
    bool post(string url, string data);
    /**
     Download url into folder
     */
    bool download(string url, string folder);
    /**
     Download url and rename it to satisfy filepath
     */
    bool downloadAs(string url, string filepath);
    
private:
    static size_t writeToFile(void * ptr, size_t size, size_t nmemb, FILE * stream);
    static size_t writeToString(char * ptr, size_t size, size_t nmemb, string * sp);
    
    void initCURL();
    
private:
    CURL * m_curl;
};

#endif /* defined(__Downloader__HttpClient__) */
