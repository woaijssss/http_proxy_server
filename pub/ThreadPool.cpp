#include <iostream>
#include "ThreadPool.h"

using namespace std;

#define MAX_THREAD      100

ThreadPool::ThreadPool(int size)
        : m_size(size),
          m_task(m_size)
{
        this->createThreads();

        if (m_size > 100)
        {
                perror("too many threads, exit...");
                exit(-1);
        }
}

ThreadPool::ThreadPool(int size, task_func_t callback)
        : m_size(size),
          m_callback(callback),
          m_task(m_size)
{
        this->createThreads();

        if (m_size > 100)
        {
                perror("too many threads, exit...");
                exit(-1);
        }
}

ThreadPool::~ThreadPool()
{
        this->stop();
}

void ThreadPool::start()
{
        for (int i = 0; i < m_size; i++)
        {
                m_vTh[i]->start(m_callback);
        }
}

void ThreadPool::stop()
{
        for (int i = 0; i < m_size; i++)
        {
                // m_vTh[i]->stop();
                delete m_vTh[i];
        }
}

void ThreadPool::createThreads()
{
        for (int i = 0; i < m_size; i++)
        {
                ImplThread* th = new CPPThread(m_task);
                m_vTh.push_back(th);
        }
}
