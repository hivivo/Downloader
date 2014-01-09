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

typedef function<void(string)> HttpClientCallback;

/**
 Simple HTTP Client. Only one request at the same time.
 */
class HttpClient
{
public:
    HttpClient();
    HttpClient(string agent);
    virtual ~HttpClient();
    
    bool get(string url, HttpClientCallback callback);
    bool post(string url, string data, HttpClientCallback callback);
    bool download(string url, string filepath);
    
    bool isRequesting();
    
private:
    // FIXME: why static here?
    static size_t onReceived(void *ptr, size_t size, size_t nmemb, void * stream);
    
private:
    HttpClientCallback m_callback;
    CURL * m_curl;
    bool m_requesting;
};

#endif /* defined(__Downloader__HttpClient__) */
