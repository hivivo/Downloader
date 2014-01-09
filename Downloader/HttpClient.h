//
//  HttpClient.h
//  Downloader
//
//  Created by Xu Xu on 1/8/14.
//  Copyright (c) 2014 Xu Xu. All rights reserved.
//

#ifndef __Downloader__HttpClient__
#define __Downloader__HttpClient__

#include "Common.h"
#include <curl/curl.h>

typedef function<void(string)> HttpClientCallback; // TODO: remove it, for useless

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
    bool post(string url, string data, HttpClientCallback callback);
    /**
     Download url into folder
     */
    bool download(string url, string folder);
    /**
     Download url and rename it to satisfy filepath
     */
    bool downloadAs(string url, string filepath);
    
    bool isRequesting();
    
private:
    // FIXME: why static here?
    static size_t writeToFile(void * ptr, size_t size, size_t nmemb, FILE * stream);
    static size_t writeToString(char * ptr, size_t size, size_t nmemb, string * sp);
    
    void initCURL();
    
private:
    HttpClientCallback m_callback;
    CURL * m_curl;
    bool m_requesting;
};

#endif /* defined(__Downloader__HttpClient__) */
