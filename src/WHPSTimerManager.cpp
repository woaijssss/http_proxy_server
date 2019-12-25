/*
 * WHPSTimerManager.cpp
 *
 *  Created on: Sep 20, 2019
 *      Author: wenhan
 */

#include "WHPSTimerManager.h"

std::shared_ptr<TimerManager> TimerManager::m_timerManager;

using namespace std;

TimerManager::TimerManager()
        : Heap(),
          m_thrd(thread(&TimerManager::loop, this)),
          m_isStop(false)
{
        WHPSLogDebug("timer pool init...");
}

TimerManager::~TimerManager()
{
        this->stop();
}

TimerManager* TimerManager::GetInstance()
{
        // if (!_tcp_server.get())
        if (!m_timerManager)
        {
                m_timerManager = std::shared_ptr<TimerManager>(new TimerManager());
        }

        return m_timerManager.get();
}

void TimerManager::addTimer(WHPSTimer t)
{
        this->push(t);
}

void TimerManager::delTimer(WHPSTimer& t)
{
        WHPSLogDebug("TimerManager::delTimer: %ld", t.id());
//        std::lock_guard<std::mutex> lock(m_mutex);
        bool is_success = this->erase(t);

        if (is_success)
        {
                WHPSLogDebug("TimerManager::delTimer 删除定时器: %ld", t.id());
                WHPSLogDebug("TimerManager::delTimer 队列大小: %ld", this->size());
        }
}

void TimerManager::stop()
{
        m_isStop = true;
        m_thrd.join();
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

        while (!m_isStop)
        {
                WHPSTimer t;
                if (this->size())
                {
                        t = this->front();
                        sleep_time = this->waitTime(t);
                } else
                {
                        sleep_time = 10;
                }

                if (sleep_time)
                {
                        std::this_thread::sleep_for(chrono::milliseconds(sleep_time));
                }

                t = this->pop();
                std::lock_guard<std::mutex> lock(m_mutex);

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

