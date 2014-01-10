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

Downloader * Downloader::instance()
{
    static Downloader * s_instance = NULL;
    
    if (s_instance == NULL)
    {
        s_instance = new Downloader;
    }
    
    return s_instance;
}

Downloader::Downloader() : m_lastId(0)
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
    
    // TODO: check thread count
    
    // create new thread to execute the task
    pthread_t thread;
    pthread_create(&thread, NULL, &Downloader::runNextTask, NULL);
    pthread_detach(thread);
    
    // remember it
    pthread_mutex_lock(&s_running);
    m_running[task.id] = thread;
    pthread_mutex_unlock(&s_running);
    
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
    
    // use httpclient to download it
    HttpClient client;
    client.download(task.url, task.folder);
    
    // remove me from the running list
    pthread_mutex_lock(&s_running);
    m_running.erase(task.id);
    pthread_mutex_unlock(&s_running);
}
