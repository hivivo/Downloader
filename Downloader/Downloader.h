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
    int download(string url);
    
#ifdef DEBUG_MODE
    void wait();
#endif
    
protected:
    /**
     Description of a download task
     */
    struct Task
    {
        string url;
        string filepath;
    };
    
private:
    Downloader(); // for singleton
    static void * runTask(void * param); // for thread start
    void runTask(Task task); // for download a task
    
    
private:
    queue<Task> m_waiting;
    unordered_map<int, pthread_t> m_running;
    
};

#endif /* defined(__Downloader__Downloader__) */
