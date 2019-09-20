#include <iostream>
#include <sys/time.h>

#include "WHPSTimer.h"
#include "WHPSTimerManager.h"

using namespace std;

#define INVALID_TIMERID -1
#define MAX_FIRE_TIME   0xFFFFFFFF

WHPSTimer::WHPSTimer(TimerCallback_t cb, void* param, const int& interval)
        : _isActive(false)
        , _isStop(true)
        , _id(this->getMilliseconds() + long(this))
        , _mutex(new std::mutex())
{
        _interval = interval;
        _fireTime = _interval + this->getMilliseconds();

        this->setTimerCallback(cb);
}

WHPSTimer::~WHPSTimer()
{
        if (_mutex)
        {
//                delete _mutex;
//                _mutex = nullptr;
        }
}

long WHPSTimer::fireTime()
{
        return _fireTime;
}

int WHPSTimer::interval()
{
        return _interval;
}

void WHPSTimer::setInterval(const int& interval)
{
        _interval = interval;
}

void WHPSTimer::setTimerCallback(TimerCallback_t cb)
{
        _timeCB = cb;
}

WHPSTimer::TimerCallback_t WHPSTimer::getTimerCallback()
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

bool WHPSTimer::operator==(WHPSTimer& right)
{
        return (this->id() == right.id());
}

bool WHPSTimer::isValid()
{
        return (_isStop ? false : true);
}

long WHPSTimer::getMilliseconds()
{
        struct timeval tv;
        gettimeofday(&tv, NULL);

        return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void WHPSTimer::start()
{
        std::lock_guard<std::mutex> lock(*_mutex);
        _isStop = false;
        _isActive = true;
        _fireTime = _interval + this->getMilliseconds();
        GetTimerManager()->addTimer(*this);
}

void WHPSTimer::stop()
{
        std::lock_guard<std::mutex> lock(*_mutex);
        if (_isStop)    // 已经停止
        {
                return;
        }

        _isStop = true;
        _isActive = false;
        _fireTime = MAX_FIRE_TIME;    // 设置为永久
        GetTimerManager()->delTimer(*this);
        _id = INVALID_TIMERID;
}

const unsigned long& WHPSTimer::id()
{
        return _id;
}
