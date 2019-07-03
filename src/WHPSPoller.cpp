
#include <string.h>

#include "WHPSPoller.h"

WHPSPoller::WHPSPoller()
{

}
       
WHPSPoller::~WHPSPoller()
{

}

void WHPSPoller::addToEventList(int fd, event_chn* p_event)
{
        _event_list.insert(fd, p_event);
}

void WHPSPoller::delFromEventList(int fd)
{
        _event_list.erase(fd);
}

/*
void WHPSPoller::updateEventList(int fd)
{
        // nothing to do
}
*/

void WHPSPoller::addEvent(event_chn* p_event)
{
        int fd = p_event->getFd();
        SEplEvent evt;
        memset(&evt, 0, sizeof(evt));
        evt.events = p_event->getEvents();
        //evt.data.fd = fd;
        evt.data.ptr = p_event;

        this->addToEventList(fd, p_event);
        this->__addEvent(fd, evt);
}

void WHPSPoller::delEvent(event_chn* p_event)
{
        int fd = p_event->getFd();
        SEplEvent evt;
        memset(&evt, 0, sizeof(evt));
        evt.events = p_event->getEvents();
        //evt.data.fd = fd;
        evt.data.ptr = p_event;

        this->delFromEventList(fd);
        this->__delEvent(fd, evt);
}

void WHPSPoller::updateEvent(event_chn* p_event)
{
        int fd = p_event->getFd();
        SEplEvent evt;
        memset(&evt, 0, sizeof(evt));
        evt.events = p_event->getEvents();
        //evt.data.fd = fd;
        evt.data.ptr = p_event;

        this->__updateEvent(fd, evt);
}