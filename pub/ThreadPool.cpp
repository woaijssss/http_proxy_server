
#include <iostream>
#include "ThreadPool.h"

using namespace std;

#define MAX_THREAD      100

ThreadPool::ThreadPool(int size)
        : __size(size)
{
        this->createThreads();

        if (__size > 100)
        {
                perror("too many threads, exit...");
                exit(-1);
        }
}

ThreadPool::ThreadPool(int size, task_func_t callback)
        : __size(size)
        , __callback(callback)
{
        this->createThreads();

        if (__size > 100)
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
        for (int i = 0; i < __size; i++)
        {
                __v_th[i]->start(__callback);
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
                ImplThread* th = new CPPThread(__task);
                __v_th.push_back(th);
        }
}
