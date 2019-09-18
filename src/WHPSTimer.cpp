#include <iostream>
#include <sys/time.h>

#include "WHPSTimer.h"

using namespace std;

std::shared_ptr<TimerManager> TimerManager::_timer_manager;

WHPSTimer::WHPSTimer(TimerCallback_t cb, void* param, const int& interval)
        : _isActive(false), _isStop(true), _id(this->getMilliseconds() + long(this))
{
        _interval = interval;
        _fireTime = _interval + this->getMilliseconds();

        this->setTimerCallback(cb);
}

WHPSTimer::~WHPSTimer()
{

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
//        return ((_id > 0) ? true : false);
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
        {
//                std::lock_guard<std::mutex> lock(_mutex);
                _isStop = false;
                _isActive = true;
                _fireTime = _interval + this->getMilliseconds();
        }
        GetTimerManager()->addTimer(*this);
}

void WHPSTimer::stop()
{
        {
//                std::lock_guard<std::mutex> lock(_mutex);
                _isStop = true;
                _isActive = false;
                _fireTime = 0xFFFFFFFF;    // 设置为永久
        }
        GetTimerManager()->delTimer(*this);
                _id = -1;
}

const unsigned long& WHPSTimer::id()
{
        return _id;
}

TimerManager::TimerManager()
        : Heap(), _thrd(thread(&TimerManager::loop, this)), _is_stop(false)
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

void TimerManager::delTimer(WHPSTimer& t)
{
        cout << "TimerManager::delTimer" << endl;
//        std::lock_guard<std::mutex> lock(_mutex);
        bool is_success = this->erase(t);

        if (is_success)
        {
                cout << "删除定时器： " << t.id() << endl;
                cout << "队列大小： " << this->size() << endl;
        }
//        else
//        {
//                cout << "未找到定时器: " << t.id() << endl;
//        }
}

void TimerManager::print()
{
        for (auto& obj : getHeap())
        {
                cout << obj.interval() << endl;
        }
}

void TimerManager::stop()
{
        _is_stop = true;
        _thrd.join();
}

long TimerManager::waitTime(WHPSTimer& t)
{
        // WHPSTimer t = this->front();
        long fire_time = t.fireTime();
        long now = t.getMilliseconds();

        /* 防止出现触发时间过期的情况，导致负数(long)或溢出(unsigned long) */
        return (((fire_time >= now) && (fire_time != 0xFFFFFFFF)) ? (fire_time - now) : 0);
}

#include <unistd.h>
void TimerManager::loop()
{
        int sleep_time = 10;    // ms
        while (!_is_stop)
        {
                WHPSTimer t;
                if (this->size())
                {
                        t = this->front();
                        sleep_time = this->waitTime(t);
                }
                else
                {
                        sleep_time = 10;
                }

                 cout << "sleep_time: " << sleep_time << endl;
                if (sleep_time)
                {
                        std::this_thread::sleep_for(chrono::milliseconds(sleep_time));
                }
                
                t = this->pop();
                std::lock_guard<std::mutex> lock(_mutex);

                if (t.isValid())
                {
                        t.getTimerCallback()(t);        // 执行定时器回调

                        if (t.isValid())
                        {
                                t.start();
                        }
                }
        }
}
