
#include <iostream>
#include <sys/time.h>

#include "WHPSTimer.h"

using namespace std;

std::shared_ptr<TimerManager> TimerManager::_timer_manager;

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
        GetTimerManager()->addTimer(*this);
}

void WHPSTimer::stop()
{
        _id = -1;
        _isActive = false;
        _fireTime = 0xFFFFFFFF;    // 设置为永久
        GetTimerManager()->delTimer(*this);
}

const int& WHPSTimer::id() const 
{
        return _id;
}


TimerManager::TimerManager()
        : Heap()
        , _thrd(thread(&TimerManager::loop, this))
        , _is_stop(false)
{
        cout << "timer pool init..." << endl;
}

TimerManager::~TimerManager()
{
        this->stop();
}

TimerManager* TimerManager::GetInstance()
{
        // if (!_tcp_server.get())
        if (!_timer_manager)
        {
                _timer_manager = std::shared_ptr<TimerManager>(new TimerManager());
        }

        return _timer_manager.get();
}

void TimerManager::addTimer(WHPSTimer t)
{
        this->push(t);
}

void TimerManager::delTimer(const WHPSTimer& t)
{
        std::lock_guard<std::mutex> lock(_mutex);
        std::list<WHPSTimer>::iterator it = this->find(t);

        if (it != getHeap().end())
        {
                cout << "删除定时器： " << it->id() << endl;
                this->erase(it);        
                cout << "队列大小： " << this->size() << endl;
        }
}

void TimerManager::print()
{
        for (auto& obj: getHeap())
        {
                cout << obj.interval() << endl;
        }
}

void TimerManager::stop()
{
        _is_stop = true;
        _thrd.join();
}

long TimerManager::waitTime()
{
        WHPSTimer t = this->front();
        const long& fire_time = t.fireTime();
        const long& now = t.getMilliseconds();

        /* 防止出现触发时间过期的情况，导致负数(long)或溢出(unsigned long) */
        return ((fire_time >= now) ? (fire_time-now) : 0);
}

#include <unistd.h>
void TimerManager::loop()
{
        while (!_is_stop)
        {
                std::lock_guard<std::mutex> lock(_mutex);
                if (!this->size())
                {
                        usleep(100);
                        continue;
                }

                // std::this_thread::nanpsleep(this->waitTime());
                std::this_thread::sleep_for(chrono::milliseconds(this->waitTime()));
                WHPSTimer t = this->pop();

                if (t.isValid())
                {
                        t.getTimerCallback()(t);
                        t.start();
                }
        }
}