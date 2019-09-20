/*
 * WHPSTimerManager.cpp
 *
 *  Created on: Sep 20, 2019
 *      Author: wenhan
 */

#include "WHPSTimerManager.h"

std::shared_ptr<TimerManager> TimerManager::_timer_manager;

using namespace std;

TimerManager::TimerManager()
        : Heap(), _thrd(thread(&TimerManager::loop, this)), _is_stop(false)
{
        WHPSLogInfo("timer pool init...");
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
        WHPSLogInfo("TimerManager::delTimer: %ld", t.id());
//        std::lock_guard<std::mutex> lock(_mutex);
        bool is_success = this->erase(t);

        if (is_success)
        {
                WHPSLogInfo("TimerManager::delTimer 删除定时器: %ld", t.id());
                WHPSLogInfo("TimerManager::delTimer 队列大小: %ld", this->size());
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

