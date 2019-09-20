
#include "WHPSEpollEventLoop.h"

WHPSEpollEventLoop::WHPSEpollEventLoop(int maxevents /*= 1024*/, int timeout /*= 100*/)
        : _poller(maxevents, timeout)
        , _event_queue_main()
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
                _poller.poll(_event_queue_main);         // 获取当前所有的事件

                /* 当前是线程池所有线程公用一个事件队列，不加锁会崩溃
                 * 此处需要修改成每个线程单独一个事件队列即可
                 */
                for (event_chn* chn : _event_queue_main)     // 遍历事件队列
                {
                        if (chn)
                        {
                                chn->exCallback();
                        }
                }

                _event_queue_main.clear();   // 执行结束后清空队列
        }
}

#include <unistd.h>
void WHPSEpollEventLoop::loopOne()
{
        _poller.poll(_event_queue);         // 获取当前所有的事件
        /* 当前是线程池所有线程，每个线程单独一个事件队列
         * 该队列不能遍历，因为存在应用层超时，触发删除操作，从而导致迭代器失效。
         * 实际做法：
         *      每次获取当前队列的第一个值，并上锁，触发事件
         * 这样的做法，不会导致调用和删除冲突，保证资源的安全。
         */
//        for (event_chn* chn : _event_queue)     // 遍历事件队列
        while (0 != _event_queue.size())
        {
                std::lock_guard<std::mutex> lock(_mutex);

                event_chn* chn = _event_queue.front();

                if (chn)
                {
                        // sleep(10);      // 当A线程执行到此时，B线程执行了WHPSTcpSession的析构函数(客户端断开)，valgrind会报错(必现)
                        chn->exCallback();
                }

                /* 防止处理过程中被清除，造成取空导致崩溃的情况 */
                if (_event_queue.size())
                {
                        _event_queue.pop_front();
                }
        }

        _event_queue.clear();   // 执行结束后清空队列
}
