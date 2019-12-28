
#include <iostream>
#include <sys/time.h>

#include "WHPSTimer.h"

using namespace std;

// TimerManager tmg;

WHPSTimer::WHPSTimer(TimerCallback cb, void* param, const int& interval)
        : _isActive(false), _id(interval/1)
{
        _interval = interval;
        _fireTime = _interval+this->getMilliseconds();

        this->setTimerCallback(cb);
}

WHPSTimer::~WHPSTimer() 
{

}

const long& WHPSTimer::fireTime()
{
        return _fireTime;
}

const int& WHPSTimer::interval()
{
        return _interval;
}

void WHPSTimer::setInterval(const int& interval)
{
        _interval = interval;
}

void WHPSTimer::setTimerCallback(TimerCallback cb)
{
        _timeCB = cb;
}

WHPSTimer::TimerCallback WHPSTimer::getTimerCallback()
{
        return _timeCB;
}

bool WHPSTimer::operator<(WHPSTimer& right)
{
        return (this->fireTime() < right.fireTime());
}

bool WHPSTimer::operator>(WHPSTimer& right)
{
        return (this->fireTime() > right.fireTime());
}

bool WHPSTimer::operator==(const WHPSTimer& right)
{
        return (this->id() == right.id());
}

bool WHPSTimer::isValid()
{
        return ((_id > 0) ? true : false);
}

long WHPSTimer::getMilliseconds()
{
        struct timeval tv;
        gettimeofday(&tv, NULL);

        return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void WHPSTimer::start()
{
        _isActive = true;
        _fireTime = _interval+this->getMilliseconds();
        // tmg.addTimer(*this);
}

void WHPSTimer::stop()
{
        _isActive = false;
        _fireTime = 0xFFFFFFFF;    // 璁剧疆涓烘案涔�
        // tmg.delTimer(*this);
}

const int& WHPSTimer::id() const 
{
        return _id;
}
