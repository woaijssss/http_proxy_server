
#include <iostream>

#include "WHPSThreadPool.h"

using namespace std;

WHPSThreadPool::WHPSThreadPool(int size, WHPSEpollEventLoop& main_loop)
        : _main_loop(main_loop)
        , _index(0)
        , _size(size)
        , _task(_size)
{
        this->createThreads();

        if (_size > 100)
        {
                perror("too many threads, exit...");
                exit(-1);
        }
}

WHPSThreadPool::WHPSThreadPool(int size, WHPSEpollEventLoop& main_loop, task_func_t callback)
        : _main_loop(main_loop)
        , _index(0)
        , _size(size)
        , _callback(callback)
        , _task(_size)
{
        this->createThreads();

        if (_size > 100)
        {
                perror("too many threads, exit...");
                exit(-1);
        }
}

WHPSThreadPool::~WHPSThreadPool()
{
        _task.stop();
        this->stop();
}

WHPSEpollEventLoop& WHPSThreadPool::getOneLoop()
{
        if (_size)      // 多线程
        {
                WHPSEpollEventLoop& loop = _v_th[_index]->getLoop();
                _index = (_index+1) % _size;
//                cout << "------index: " << _index << endl;
                return loop;
        }
        else            // 单线程(只有主线程时调用)
        {
                return _main_loop;
        }
}

void WHPSThreadPool::start()
{
        for (int i = 0; i < _size; i++)
        {
                _v_th[i]->start();
        }
}

void WHPSThreadPool::createThreads()
{
        for (int i = 0; i < _size; i++)
        {
                WHPSThread* th = new WHPSThread(_task);
                _v_th.push_back(th);
        }
}

void WHPSThreadPool::stop()
{
        for (int i = 0; i < _size; i++)
        {
                // __v_th[i]->stop();
                delete _v_th[i];
        }
}


WHPSWorkerThreadPool::WHPSWorkerThreadPool(int size)
        : _task(size)
        , _index(0)
        , _size(size)
{
        this->createThreads();

        if (_size > 100)
        {
                perror("too many threads, exit...");
                exit(-1);
        }
}

WHPSWorkerThreadPool::WHPSWorkerThreadPool(int size, task_func_t callback)
        : _task(size)
        , _index(0)
        , _size(size)
        , _callback(callback)
{
        this->createThreads();

        if (_size > 100)
        {
                perror("too many threads, exit...");
                exit(-1);
        }
}

WHPSWorkerThreadPool::~WHPSWorkerThreadPool()
{
        _task.stop();
        this->stop();
}

void WHPSWorkerThreadPool::start()
{
        for (int i = 0; i < _size; i++)
        {
                _v_th[i]->start();
        }
}

void WHPSWorkerThreadPool::createThreads()
{
        for (int i = 0; i < _size; i++)
        {
                WHPSWorkerThread* th = new WHPSWorkerThread(_task);
                _v_th.push_back(th);
        }
}

void WHPSWorkerThreadPool::stop()
{
        for (int i = 0; i < _size; i++)
        {
                // __v_th[i]->stop();
                delete _v_th[i];
        }
}
