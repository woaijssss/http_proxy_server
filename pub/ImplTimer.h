#ifndef __IMPL_TIMER_H__
#define __IMPL_TIMER_H__

#include <functional>

template<class T>
class ImplTimer
{
public:
        using TimerCallback_t = std::function<void(T&)>;
        virtual ~ImplTimer()
        {
        }
protected:
        /* 获取触发剩余时间的毫秒数 */
        virtual long fireTime() = 0;

        /* 返回定时器触发间隔时间 */
        virtual int interval() = 0;

        /* 设置定时器触发间隔时间 */
        virtual void setInterval(const int& interval) = 0;

        virtual void setTimerCallback(TimerCallback_t cb) = 0;

        virtual TimerCallback_t getTimerCallback() = 0;

        /* 启动定时器 */
        virtual void start() = 0;

        /* 停止定时器 */
        virtual void stop() = 0;

protected:
        int m_interval;          // 定时器间隔时间
        long m_fireTime;         // 触发时间毫秒数（绝对时间）
        TimerCallback_t m_timeCb;  // 定时器触发回调函数
};

#endif  // __IMPL_TIMER_H__
