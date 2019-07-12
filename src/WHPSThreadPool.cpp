
#include <iostream>

#include "WHPSThreadPool.h"

using namespace std;

WHPSThreadPool::WHPSThreadPool(int size, WHPSEpollEventLoop& main_loop)
        : _main_loop(main_loop)
        , _index(0)
        , _size(size)
{
        this->createThreads();

        if (_size > 100)
        {
                cout << "too many threads, exit..." << endl;
                exit(-1);
        }
}

WHPSThreadPool::WHPSThreadPool(int size, WHPSEpollEventLoop& main_loop, task_func_t callback)
        : _main_loop(main_loop)
        , _index(0)
        , _size(size)
        , _callback(callback)
{
        this->createThreads();

        if (_size > 100)
        {
                cout << "too many threads, exit..." << endl;
                exit(-1);
        }
}

WHPSThreadPool::~WHPSThreadPool()
{
        cout << "-------WHPSThreadPool::stop...." << endl;
        this->stop();
}

WHPSEpollEventLoop& WHPSThreadPool::getOneLoop()
{
        if (_size)      // 多线程
        {
                WHPSEpollEventLoop& loop = _v_th[_index]->getLoop();
                _index = (_index+1) % _size;
                cout << "------index: " << _index << endl;
                return loop;
        }
        else            // 单线程
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