#include "WHPSEpollEventLoop.h"

WHPSEpollEventLoop::WHPSEpollEventLoop(int maxevents /*= 1024*/, int timeout /*= 100*/)
        : _poller(maxevents, timeout),
          m_eventQueueMain(),
          m_eventQueue(),
          m_isStop(false),
          m_task(0)
{

}

WHPSEpollEventLoop::~WHPSEpollEventLoop()
{
        // std::lock_guard<std::mutex> lock(m_mutex);
        m_isStop = true;
}

void WHPSEpollEventLoop::stop()
{
        _poller.setTimeout(0);  // 立即返回
        m_isStop = true;
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
        return m_task;
}

void WHPSEpollEventLoop::addTask(task_t func_cb)
{
        m_task.addTask(func_cb);
}

#include <iostream>
#include <thread>
#include <mutex>
using namespace std;
void WHPSEpollEventLoop::loop()
{
        while (!m_isStop)
        {
                _poller.poll(m_eventQueueMain);         // 获取当前所有的事件

                /* 当前是线程池所有线程公用一个事件队列，不加锁会崩溃
                 * 此处需要修改成每个线程单独一个事件队列即可
                 */
                for (event_chn* chn : m_eventQueueMain)     // 遍历事件队列
                {
                        if (chn)
                        {
                                chn->exCallback();
                        }
                }

                m_eventQueueMain.clear();   // 执行结束后清空队列
        }
}

#include <unistd.h>
void WHPSEpollEventLoop::loopOne()
{
        _poller.poll(m_eventQueue);         // 获取当前所有的事件
        /* 当前是线程池所有线程，每个线程单独一个事件队列
         * 该队列不能遍历，因为存在应用层超时，触发删除操作，从而导致迭代器失效。
         * 实际做法：
         *      每次获取当前队列的第一个值，并上锁，触发事件
         * 这样的做法，不会导致调用和删除冲突，保证资源的安全。
         */
//        for (event_chn* chn : m_eventQueue)     // 遍历事件队列
//        while (!m_eventQueue.empty())
        while (true)
        {
                std::lock_guard<std::mutex> lock(m_mutex);

                if (m_eventQueue.empty())
                {
                        break;
                }

                event_chn* chn = m_eventQueue.front();

                /* 防止处理过程中被清除，造成取空导致崩溃的情况 */
                if (!chn)
                {
                        continue;
                }

                //cout << "===========WHPSEpollEventLoop::loopOne chn: " << chn << endl;
                chn->exCallback();
                //cout << "===========WHPSEpollEventLoop::loopOne size: " << m_eventQueue.size() << endl;
                m_eventQueue.pop_front();
                //cout << "===========WHPSEpollEventLoop::loopOne size after: " << m_eventQueue.size() << endl;
        }

        m_eventQueue.clear();   // 执行结束后清空队列
}
