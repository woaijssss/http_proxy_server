#include <iostream>

#include "WHPSThreadPool.h"

using namespace std;

WHPSThreadPool::WHPSThreadPool(int size, WHPSEpollEventLoop* main_loop)
        : m_mainLoop(main_loop),
          m_index(0),
          m_size(size),
          m_task(m_size)
{
        m_task.stop();
        this->createThreads();

        if (m_size > 100)
        {
                perror("too many threads, exit...");
                exit(-1);
        }
}

WHPSThreadPool::WHPSThreadPool(int size, task_func_t callback, WHPSEpollEventLoop* main_loop)
        : m_mainLoop(main_loop),
          m_index(0),
          m_size(size),
          m_callBack(callback),
          m_task(m_size)
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
        this->stop();
}

WHPSEpollEventLoop& WHPSThreadPool::getOneLoop()
{
        if (m_size)      // 多线程
        {
                WHPSEpollEventLoop& loop = m_vTh[m_index]->getLoop();
                m_index = (m_index + 1) % m_size;

                return loop;
        } else            // 单线程(只有主线程时调用)
        {
                return *m_mainLoop;
        }
}

void WHPSThreadPool::start()
{
        for (int i = 0; i < m_size; i++)
        {
                m_vTh[i]->start();
        }
}

void WHPSThreadPool::addTask(task_func_t task)
{
        m_task.addTask(task);
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
        m_task.stop();
        for (int i = 0; i < m_size; i++)
        {
                // __v_th[i]->stop();
                delete m_vTh[i];
        }
}
