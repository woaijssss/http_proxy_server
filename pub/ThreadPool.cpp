
#include "ThreadPool.h"

ThreadPool::ThreadPool(int size)
        : __size(size)
{
        this->createThreads();
}

ThreadPool::~ThreadPool()
{
        this->stop();
}

void ThreadPool::start()
{
        for (int i = 0; i < __size; i++)
        {
                __v_th[i]->getThread();
                __v_th[i]->start();
        }
}

void ThreadPool::stop()
{
        for (int i = 0; i < __size; i++)
        {
                // __v_th[i]->stop();
                delete __v_th[i];
        }
}

void ThreadPool::createThreads()
{
        for (int i = 0; i < __size; i++)
        {
                ImplThread* th = new CPPThread();
                __v_th.push_back(th);
        }
}