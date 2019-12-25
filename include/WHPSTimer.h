#ifndef __WHPS_TIMER_H__
#define __WHPS_TIMER_H__

#include <thread>
#include <mutex>

#include "Heap.h"
#include "ImplTimer.h"
#include "WHPSLog.h"

/* 定时器 */
class WHPSTimer final: public ImplTimer<WHPSTimer>
{
public:
        using TimerCallback_t = ImplTimer<WHPSTimer>::TimerCallback_t;
public:
        /* interval为触发间隔时间的毫秒数 */
        WHPSTimer(TimerCallback_t cb = nullptr, void* param = NULL, const int& interval = -1);

        virtual ~WHPSTimer();

        virtual long fireTime() override;

        /* 返回定时器触发间隔时间 */
        virtual int interval() override;

        /* 设置定时器触发间隔时间 */
        virtual void setInterval(const int& interval);

        virtual void setTimerCallback(TimerCallback_t cb);

        virtual TimerCallback_t getTimerCallback();

        virtual void start();

        virtual void stop();

        /* 获取当前时间毫秒数 */
        long getMilliseconds();

        const unsigned long& id();

        bool operator<(WHPSTimer& right);

        bool operator>(WHPSTimer& right);

        bool operator==(WHPSTimer& right);

        /* 判断当前定时器是否有效 */
        bool isValid();

private:
        bool m_isActive;         // 定时器活跃度标识
        bool m_isStop;           // 是否已经停止
        unsigned long m_id;
//        std::mutex _mutex;
        std::shared_ptr<std::mutex> m_mutex;
//        std::mutex* _mutex;
};

#endif  // __WHPS_TIMER_H__
