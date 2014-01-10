//
//  Downloader.cpp
//  Downloader
//
//  Created by Xu Xu on 1/8/14.
//  Copyright (c) 2014 Xu Xu. All rights reserved.
//

#include "Downloader.h"
#include "HttpClient.h"

Downloader * Downloader::instance()
{
    static Downloader * s_instance = NULL;
    
    if (s_instance == NULL)
    {
        s_instance = new Downloader;
    }
    
    return s_instance;
}

Downloader::Downloader()
{
    
}

Downloader::~Downloader()
{
    
}



int Downloader::download(string url)
{
    Task task;
    task.url = url;
    
    // TODO: need lock
    m_waiting.push(task);
    
    // TODO: check thread count
    
    
    
    pthread_t thread;
    
    pthread_create(&thread, NULL, &Downloader::runTask, task);
    pthread_detach(thread);
    
    return 0;
}




void * Downloader::runTask(void * param)
{
    Task * task = (Task *) param;
    
    HttpClient client;
    
    client.downloadAs(task->url, task->filepath);
    
    delete task;
    
    return NULL;
}