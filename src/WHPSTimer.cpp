#include <iostream>
#include <sys/time.h>

#include "WHPSTimer.h"
#include "WHPSTimerManager.h"

using namespace std;

#define INVALID_TIMERID -1
#define MAX_FIRE_TIME   0xFFFFFFFF

WHPSTimer::WHPSTimer(TimerCallback_t cb, void* param, const int& interval)
        : m_isActive(false),
          m_isStop(true),
          m_id(this->getMilliseconds() + long(this)),
          m_mutex(new std::mutex())
{
        m_interval = interval;
        m_fireTime = m_interval + this->getMilliseconds();

        this->setTimerCallback(cb);
}

WHPSTimer::~WHPSTimer()
{
        if (m_mutex)
        {
//                delete m_mutex;
//                m_mutex = nullptr;
        }
}

long WHPSTimer::fireTime()
{
        return m_fireTime;
}

int WHPSTimer::interval()
{
        return m_interval;
}

void WHPSTimer::setInterval(const int& interval)
{
        m_interval = interval;
}

void WHPSTimer::setTimerCallback(TimerCallback_t cb)
{
        m_timeCb = cb;
}

WHPSTimer::TimerCallback_t WHPSTimer::getTimerCallback()
{
        return m_timeCb;
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
        return (m_isStop ? false : true);
}

long WHPSTimer::getMilliseconds()
{
        struct timeval tv;
        gettimeofday(&tv, NULL);

        return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void WHPSTimer::start()
{
        std::lock_guard<std::mutex> lock(*m_mutex);
        m_isStop = false;
        m_isActive = true;
        m_fireTime = m_interval + this->getMilliseconds();
        GetTimerManager()->addTimer(*this);
}

void WHPSTimer::stop()
{
        std::lock_guard<std::mutex> lock(*m_mutex);
        if (m_isStop)    // 已经停止
        {
                return;
        }

        m_isStop = true;
        m_isActive = false;
        m_fireTime = MAX_FIRE_TIME;    // 设置为永久
        GetTimerManager()->delTimer(*this);
        m_id = INVALID_TIMERID;
}

const unsigned long& WHPSTimer::id()
{
        return m_id;
}
