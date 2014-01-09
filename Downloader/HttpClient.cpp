//
//  HttpClient.cpp
//  Downloader
//
//  Created by Xu Xu on 1/8/14.
//  Copyright (c) 2014 Xu Xu. All rights reserved.
//

#include "HttpClient.h"

#include <curl/curl.h>

HttpClient::HttpClient() : m_requesting(false)
{
    // init global
    if (!s_globalInitialized)
    {
        curl_global_init(CURL_GLOBAL_ALL);
        s_globalInitialized = true;
    }
    
    // init current session
    m_curl = curl_easy_init();
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
    
    /* debug mode
    // Switch on full protocol/debug output while testing
    curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);
    
    // disable progress meter, set to 0L to enable and disable debug output
    curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);
    */
    
    // forward all data to this func
    curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, &HttpClient::onReceived);
    
    // open the file
    
}
