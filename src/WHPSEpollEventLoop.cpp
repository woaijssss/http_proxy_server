
#include "WHPSEpollEventLoop.h"

WHPSEpollEventLoop::WHPSEpollEventLoop(int maxevents, int timeout)
        : _poller(maxevents, timeout)
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

}
