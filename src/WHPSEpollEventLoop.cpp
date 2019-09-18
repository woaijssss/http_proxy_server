
#include "WHPSEpollEventLoop.h"

WHPSEpollEventLoop::WHPSEpollEventLoop(int maxevents /*= 1024*/, int timeout /*= 100*/)
        : _poller(maxevents, timeout)
        , _event_queue()
        , _is_stop(false)
{

}

WHPSEpollEventLoop::~WHPSEpollEventLoop()
{
        // std::lock_guard<std::mutex> lock(_mutex);
        _is_stop = true;
}

void WHPSEpollEventLoop::stop()
{
        _poller.setTimeout(0);  // 立即返回
        _is_stop = true;
}

void WHPSEpollEventLoop::addEvent(event_chn* p_event)
{
        _poller.addEvent(p_event);
}

void WHPSEpollEventLoop::delEvent(event_chn* p_event)
{
        _poller.delEvent(p_event);
}

void WHPSEpollEventLoop::updateEvent(event_chn* p_event)
{
        _poller.updateEvent(p_event);
}

Task<WHPSEpollEventLoop::task_t>& WHPSEpollEventLoop::getTask()
{
        return _task;
}

void WHPSEpollEventLoop::addTask(task_t func_cb)
{
        _task.addTask(func_cb);
}

#include <iostream>
#include <thread>
#include <mutex>
using namespace std;
void WHPSEpollEventLoop::loop()
{
        while (!_is_stop)
        {
                _poller.poll(_event_queue);         // 获取当前所有的事件

                /* 当前是线程池所有线程公用一个事件队列，不加锁会崩溃
                 * 此处需要修改成每个线程单独一个事件队列即可
                 */
                // mutex__.lock();
                for (event_chn* chn : _event_queue)     // 遍历事件队列
                {
                        if (chn)
                        {
                                chn->exCallback();
                        }
                }

                _event_queue.clear();   // 执行结束后清空队列
                // mutex__.unlock();
        }
}

#include <unistd.h>
void WHPSEpollEventLoop::loopOne()
{
        _poller.poll(_event_queue);         // 获取当前所有的事件
        /* 当前是线程池所有线程，每个线程单独一个事件队列
         */
        for (event_chn* chn : _event_queue)     // 遍历事件队列
        {
                std::lock_guard<std::mutex> lock(_mutex);
                if (chn)
                {
                        // sleep(10);      // 当A线程执行到此时，B线程执行了WHPSTcpSession的析构函数(客户端断开)，valgrind会报错(必现)
                        chn->exCallback();
                }
        }

        _event_queue.clear();   // 执行结束后清空队列
}
