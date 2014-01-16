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

#ifndef __Downloader__Downloader__
#define __Downloader__Downloader__

#include "Common.h"

typedef function<void(int)> DownloaderCallback;

class Downloader
{
public:
    static Downloader * instance();
    virtual ~Downloader();
    
    /**
     Setup a callback function. When all tasks are done, call it.
     */
    void setCompletedCallback(DownloaderCallback callback);
    
    /**
     Download a file from url.
     @return: task ID. If failed, return -1
     */
    int download(string url, string folder);
    int download(string url, string folder, DownloaderCallback callback);
    int download(string url, string folder, string filename);
    int download(string url, string folder, string filename, DownloaderCallback callback);
    
protected:
    /**
     Description of a download task
     */
    struct Task
    {
        int id;
        string url;
        string folder;
        string filename;
        DownloaderCallback callback;
        
        Task() : id(0) {}
    };
    
private:
    Downloader(); // for singleton
    static void * runNextTask(void*); // for thread start
    void runNextTask(); // for download the next task in the waiting list
    void callCallbackSafely(DownloaderCallback callback, int); // make sure the callbacks are executed one by one
    
private:
    int m_lastId;
    
    queue<Task> m_waiting;
    static pthread_mutex_t s_waiting;
    
    unordered_map<int, pthread_t> m_running;
    static pthread_mutex_t s_running;
    
    int m_threadCount;
    static pthread_mutex_t s_threadCount;
    
    DownloaderCallback m_onCompleted;
    static pthread_mutex_t s_callback;
    
};

#endif /* defined(__Downloader__Downloader__) */
