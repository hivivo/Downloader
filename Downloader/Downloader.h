//
//  Downloader.h
//  Downloader
//
//  Created by Xu Xu on 1/8/14.
//  Copyright (c) 2014 Xu Xu. All rights reserved.
//

#ifndef __Downloader__Downloader__
#define __Downloader__Downloader__

#include "Common.h"

class Downloader
{
public:
    static Downloader * instance();
    virtual ~Downloader();
    
    /**
     Download a file from url.
     @return: task ID. If failed, return -1
     */
    int download(string url, string folder);
    
protected:
    /**
     Description of a download task
     */
    struct Task
    {
        int id;
        string url;
        string folder;
        Task() : id(0) {}
    };
    
private:
    Downloader(); // for singleton
    static void * runNextTask(void*); // for thread start
    void runNextTask(); // for download the next task in the waiting list
    
private:
    queue<Task> m_waiting;
    static pthread_mutex_t s_waiting;
    
    unordered_map<int, pthread_t> m_running;
    static pthread_mutex_t s_running;
    
    int m_lastId;
};

#endif /* defined(__Downloader__Downloader__) */
