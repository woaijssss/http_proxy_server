
#include "WHPSEpollEventLoop.h"

WHPSEpollEventLoop::WHPSEpollEventLoop(int maxevents, int timeout)
        : _poller(maxevents, timeout)
        , _event_queue()
{

}

WHPSEpollEventLoop::~WHPSEpollEventLoop()
{

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

void WHPSEpollEventLoop::loop()
{
        while (true)
        {
                _poller.poll(_event_queue);         // 获取当前所有的事件

                for (event_chn* chn : _event_queue)     // 遍历事件队列
                {
                        chn->exCallback();
                }

                _event_queue.clear();   // 执行结束后清空队列
        }
}
