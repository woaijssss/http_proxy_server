
#include "WHPSEpollEventLoop.h"

static int g_nthreads = 0;     // 可做成配置

WHPSEpollEventLoop::WHPSEpollEventLoop(int maxevents, int timeout)
        : _poller(maxevents, timeout)
        , _event_queue()
        , _is_stop(false)
{

}

WHPSEpollEventLoop::~WHPSEpollEventLoop()
{
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

#include <iostream>
#include <thread>
#include <mutex>
using namespace std;
std::mutex mutex__;
void WHPSEpollEventLoop::loop()
{
        while (!_is_stop)
        {
                // cout << "------thread_call(" << (unsigned int)std::hash<std::thread::id>()(std::this_thread::get_id()) << ")" << endl;
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

void WHPSEpollEventLoop::loopOne()
{
        // cout << "WHPSEpollEventLoop::loopOne------thread_call(" << (unsigned int)std::hash<std::thread::id>()(std::this_thread::get_id()) << ")" << endl;
        _poller.poll(_event_queue);         // 获取当前所有的事件

        /* 当前是线程池所有线程公用一个事件队列，不加锁会崩溃
         * 此处需要修改成每个线程单独一个事件队列即可
         */
        for (event_chn* chn : _event_queue)     // 遍历事件队列
        {
                if (chn)
                {
                        chn->exCallback();
                }
        }

        _event_queue.clear();   // 执行结束后清空队列
}
