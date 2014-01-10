//
//  Downloader.cpp
//  Downloader
//
//  Created by Xu Xu on 1/8/14.
//  Copyright (c) 2014 Xu Xu. All rights reserved.
//

#include "Downloader.h"
#include "HttpClient.h"

#pragma mark Static define and Constructors

pthread_mutex_t Downloader::s_waiting;
pthread_mutex_t Downloader::s_running;
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

Downloader::Downloader() : m_lastId(0), m_threadCount(0)
{
    
}

Downloader::~Downloader()
{
    
}

#pragma mark - Public Methods

int Downloader::download(string url, string folder)
{
    Task task;
    task.id = ++m_lastId;
    task.url = url;
    task.folder = folder;
    
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
    Task task;
    
    // try to fetch next task
    pthread_mutex_lock(&s_waiting);
    if (!m_waiting.empty())
    {
        task = m_waiting.front();
        m_waiting.pop();
    }
    pthread_mutex_unlock(&s_waiting);
    
    if (task.id == 0) return;
    
    // remember it
    pthread_mutex_lock(&s_running);
    m_running[task.id] = pthread_self();
    pthread_mutex_unlock(&s_running);
    
    // use httpclient to download it
    HttpClient client;
    client.download(task.url, task.folder);
    
    // remove me from the running list
    pthread_mutex_lock(&s_running);
    m_running.erase(task.id);
    pthread_mutex_unlock(&s_running);
    
    // try run next
    runNextTask();
    
    // at last, minus thread count
    pthread_mutex_lock(&s_threadCount);
    --m_threadCount;
    pthread_mutex_unlock(&s_threadCount);
}
