
#ifndef __WHPS_TIMER_H__
#define __WHPS_TIMER_H__

#include <thread>
#include <mutex>
#include <memory>

#include "Heap.h"
#include "ImplTimer.h"
#include "ImplSingleton.h"
#include "SingletonRegister.h"

/* 定时器 */
class WHPSTimer: public ImplTimer<WHPSTimer>
{
public:
        using TimerCallback = ImplTimer<WHPSTimer>::TimerCallback;
public:
        /* interval为触发间隔时间的毫秒数 */
        WHPSTimer(TimerCallback cb = nullptr, void* param = NULL, const int& interval = -1);

        virtual ~WHPSTimer();

        virtual const long& fireTime();

        /* 返回定时器触发间隔时间 */
        virtual const int& interval();

        /* 设置定时器触发间隔时间 */
        virtual void setInterval(const int& interval);

        virtual void setTimerCallback(TimerCallback cb);

        virtual TimerCallback getTimerCallback();

        virtual void start();

        virtual void stop();

        /* 获取当前时间毫秒数 */
        inline long getMilliseconds();

        const int& id() const;

        bool operator<(WHPSTimer& right);

        bool operator>(WHPSTimer& right);

        bool operator==(const WHPSTimer& right);

        /* 判断当前定时器是否有效 */
        bool isValid();
private:
        bool _isActive;         // 定时器活跃度标识
        int _id;
};

/* 定时器管理器，应具备如下结构：
 * （1）该类为单例模式；
 * （2）该类管理timer池，所有新增的timer都加入池中；
 * （3）该类的单例对象，由单个独立的线程执行；
 * （4）该类应包含其他线程任务队列的句柄，timer触发的回调函数的执行，通过Task队列，传递给线程池执行
 */
class TimerManager: public ImplSingleton<TimerManager>, public Heap<WHPSTimer>
{
public:
        static TimerManager* GetInstance();

        virtual ~TimerManager();

        /* 添加一个定时器 */
        void addTimer(WHPSTimer t);

        /* 根据定时器id，从池中删除 */
        void delTimer(const WHPSTimer& t);

        void print();

        /* 循环检测所有timer，并执行相应的触发回调 */
        void loop();

private:
        TimerManager();

        /* 停止所有定时器，释放线程资源 */
        void stop();

        /* 当前线程等待时间 */
        long waitTime();

        void onCallTask(WHPSTimer);

private:
        static std::shared_ptr<TimerManager> _timer_manager;
        std::thread _thrd;
        bool _is_stop /*= false*/;
        std::mutex _mutex;
};
GET_SINGLETON_OBJECT_PTR(TimerManager)

#endif  // __WHPS_TIMER_H__