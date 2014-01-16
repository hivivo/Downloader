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

#include "Downloader.h"
#include "HttpClient.h"

#pragma mark Static define and Constructors

pthread_mutex_t Downloader::s_waiting;
pthread_mutex_t Downloader::s_running;
pthread_mutex_t Downloader::s_callback;
pthread_mutex_t Downloader::s_threadCount;

Downloader * Downloader::instance()
{
    static Downloader * s_instance = NULL;
    
    if (s_instance == NULL)
    {
        s_instance = new Downloader;
    }
    
    return s_instance;
}

Downloader::Downloader() : m_lastId(0), m_threadCount(0), m_onCompleted([](int){})
{
    // init mutexes
    pthread_mutex_init(&s_waiting, NULL);
    pthread_mutex_init(&s_running, NULL);
    pthread_mutex_init(&s_callback, NULL);
    pthread_mutex_init(&s_threadCount, NULL);
}

Downloader::~Downloader()
{
    pthread_mutex_destroy(&s_waiting);
    pthread_mutex_destroy(&s_running);
    pthread_mutex_destroy(&s_callback);
    pthread_mutex_destroy(&s_threadCount);
}

#pragma mark - Public Methods

void Downloader::setCompletedCallback(DownloaderCallback callback)
{
    m_onCompleted = callback;
}

int Downloader::download(string url, string folder)
{
    return download(url, folder, "");
}

int Downloader::download(string url, string folder, DownloaderCallback callback)
{
    return download(url, folder, "", callback);
}

int Downloader::download(string url, string folder, string filename)
{
    return download(url, folder, filename, [](int){});
}

int Downloader::download(string url, string folder, string filename, DownloaderCallback callback)
{
    Task task;
    task.id = ++m_lastId;
    task.url = url;
    task.folder = folder;
    task.filename = filename;
    task.callback = callback;
    
    // put it in to the queue
    pthread_mutex_lock(&s_waiting);
    m_waiting.push(task);
    pthread_mutex_unlock(&s_waiting);
    
    // check thread count
    bool canCreateThread = false;
    
    pthread_mutex_lock(&s_threadCount);
    if (m_threadCount < kMaxDownloadThreads)
    {
        canCreateThread = true;
        ++m_threadCount;
    }
    pthread_mutex_unlock(&s_threadCount);
    
    if (canCreateThread)
    {
        // create new thread to execute the task
        pthread_t thread;
        pthread_create(&thread, NULL, &Downloader::runNextTask, NULL);
        pthread_detach(thread);
    }
    
    return task.id;
}

#pragma mark - 

void * Downloader::runNextTask(void * dummy)
{
    // call instance method instead 'cause I don't like static
    instance()->runNextTask();
    return dummy;
}

// caution: this method will be called in threads!
void Downloader::runNextTask()
{
    HttpClient client;
    
    // alwasy looking for new task to execute
    while (true)
    {
        Task task;
        
        // try to fetch next task
        pthread_mutex_lock(&s_waiting);
        if (!m_waiting.empty())
        {
            task = m_waiting.front();
            m_waiting.pop();
        }
        pthread_mutex_unlock(&s_waiting);
        
        if (task.id == 0) break; // no new task
        
        // remember it
        pthread_mutex_lock(&s_running);
        m_running[task.id] = pthread_self();
        pthread_mutex_unlock(&s_running);
        
        // use httpclient to download it
        if (task.filename.empty())
        {
            client.download(task.url, task.folder);
        }
        else
        {
            client.downloadAs(task.url, task.folder + kPathSeparator + task.filename);
        }
        
        // remove me from the running list
        pthread_mutex_lock(&s_running);
        m_running.erase(task.id);
        pthread_mutex_unlock(&s_running);
        
        callCallbackSafely(task.callback, task.id);
    }
    
    // at last, minus thread count
    pthread_mutex_lock(&s_threadCount);
    bool allDone = --m_threadCount == 0;
    pthread_mutex_unlock(&s_threadCount);
    
    if (allDone) callCallbackSafely(m_onCompleted, 0);
}

void Downloader::callCallbackSafely(DownloaderCallback callback, int p)
{
    pthread_mutex_lock(&s_callback);
    callback(p);
    pthread_mutex_unlock(&s_callback);
}
