#include <iostream>

#include "WHPSThreadPool.h"

using namespace std;

WHPSThreadPool::WHPSThreadPool(int size, WHPSEpollEventLoop& main_loop)
        : m_mainLoop(main_loop)
        , m_index(0)
        , m_size(size)
        , m_task(m_size)
{
        this->createThreads();

        if (m_size > 100)
        {
                perror("too many threads, exit...");
                exit(-1);
        }
}

WHPSThreadPool::WHPSThreadPool(int size, WHPSEpollEventLoop& main_loop, task_func_t callback)
        : m_mainLoop(main_loop)
        , m_index(0)
        , m_size(size)
        , m_callback(callback)
        , m_task(m_size)
{
        this->createThreads();

        if (m_size > 100)
        {
                perror("too many threads, exit...");
                exit(-1);
        }
}

WHPSThreadPool::~WHPSThreadPool()
{
        m_task.stop();
        this->stop();
}

WHPSEpollEventLoop& WHPSThreadPool::getOneLoop()
{
        if (m_size)      // 多线程
        {
                WHPSEpollEventLoop& loop = m_vTh[m_index]->getLoop();
                m_index = (m_index+1) % m_size;
//                cout << "------index: " << m_index << endl;
                return loop;
        }
        else            // 单线程(只有主线程时调用)
        {
                return m_mainLoop;
        }
}

void WHPSThreadPool::start()
{
        for (int i = 0; i < m_size; i++)
        {
                m_vTh[i]->start();
        }
}

void WHPSThreadPool::createThreads()
{
        for (int i = 0; i < m_size; i++)
        {
                WHPSThread* th = new WHPSThread(m_task);
                m_vTh.push_back(th);
        }
}

void WHPSThreadPool::stop()
{
        for (int i = 0; i < m_size; i++)
        {
                // _m_vTh[i]->stop();
                delete m_vTh[i];
        }
}


WHPSWorkerThreadPool::WHPSWorkerThreadPool(int size)
        : m_task(size)
        , m_index(0)
        , m_size(size)
{
        this->createThreads();

        if (m_size > 100)
        {
                perror("too many threads, exit...");
                exit(-1);
        }
}

WHPSWorkerThreadPool::WHPSWorkerThreadPool(int size, task_func_t callback)
        : m_task(size)
        , m_index(0)
        , m_size(size)
        , m_callback(callback)
{
        this->createThreads();

        if (m_size > 100)
        {
                perror("too many threads, exit...");
                exit(-1);
        }
}

WHPSWorkerThreadPool::~WHPSWorkerThreadPool()
{
        m_task.stop();
        this->stop();
}

void WHPSWorkerThreadPool::start()
{
        for (int i = 0; i < m_size; i++)
        {
                m_vTh[i]->start();
        }
}

void WHPSWorkerThreadPool::addTask(task_func_t task)
{
        m_task.addTask(task);
}

void WHPSWorkerThreadPool::createThreads()
{
        for (int i = 0; i < m_size; i++)
        {
                WHPSWorkerThread* th = new WHPSWorkerThread(m_task);
                m_vTh.push_back(th);
        }
}

void WHPSWorkerThreadPool::stop()
{
        for (int i = 0; i < m_size; i++)
        {
                // _m_vTh[i]->stop();
                delete m_vTh[i];
        }
}
