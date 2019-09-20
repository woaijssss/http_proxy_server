/*
 * TimerManager.h
 *
 *  Created on: Sep 20, 2019
 *      Author: wenhan
 */

#ifndef __TIMERMANAGER_H__
#define __TIMERMANAGER_H__

#include <thread>
#include <memory>

#include "ImplSingleton.h"
#include "SingletonRegister.h"
#include "WHPSTimer.h"

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
        void delTimer(WHPSTimer& t);

        // 循环输出所有定时器间隔时间
//        void print();

        /* 循环检测所有timer，并执行相应的触发回调 */
        void loop();

private:
        TimerManager();

        /* 停止所有定时器，释放线程资源 */
        void stop();

        /* 当前线程等待时间 */
        long waitTime(WHPSTimer& t);

        void onCallTask(WHPSTimer);

private:
        static std::shared_ptr<TimerManager> _timer_manager;
        std::thread _thrd;
        bool _is_stop /*= false*/;
        std::mutex _mutex;
};
GET_SINGLETON_OBJECT_PTR(TimerManager)


#endif /* __TIMERMANAGER_H__ */
