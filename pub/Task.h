
#ifndef __TASK_H__
#define __TASK_H__

#include <queue>
#include <mutex>
#include <atomic>
#include <functional>

#include <iostream>
using namespace std;

using task_func_t = std::function<void()>;

/* 线程任务类
 * 考虑到可能有不同类型的线程池，因此将任务类型做成模板，以匹配任何的数据类型
 */
template <class T>
class Task
{
public:
        Task(const int& users)
                : _users(users)
                ,__tq_size(0)
                ,__q_mutex(PTHREAD_MUTEX_INITIALIZER)
                , _condition(PTHREAD_COND_INITIALIZER)
        {

        }

        ~Task()
        {

        }

public:
        /* 当前锁是对队列操作的全局锁，数据量大时，效率低
         * 后面设计成局部锁，或更改逻辑使得不需要加锁
         */
        /* 向任务队列添加一个任务
         * 生产者调用
         */
        inline void addTask(T task)
        {
//                std::lock_guard<std::mutex> lock(__q_mutex);
                pthread_mutex_lock(&__q_mutex);
                __tq.push(task);
                __tq_size++;    // 任务数加1
                pthread_mutex_unlock(&__q_mutex);
                pthread_cond_signal(&_condition);       // 通知消费线程可以取任务
        }

        /* 从任务队列获取一个任务
         * 消费者调用
         */
        inline T get()
        {
//                std::lock_guard<std::mutex> lock(__q_mutex);

                pthread_mutex_lock(&__q_mutex);
                T task;

                while (!__tq_size)
                {
                        pthread_cond_wait(&_condition, &__q_mutex); //api做了三件事情 //pthread_cond_wait假醒
                }

                if (__tq_size > 0)
                {
                        task = __tq.front();
                        __tq.pop();          // 是否要保证任务成功执行再删除(待定)
                        __tq_size--;    // 任务数减1
                }

                pthread_mutex_unlock(&__q_mutex);

                return task;
        }

        /* 获取当前Task队列的任务数 */
        const size_t& size() const
        {
                return __tq_size;
        }

        /* 停止生产线运作
         */
        void stop()
        {
                __tq_size = -1;

                while (_users > 0)
                {
                        if (!pthread_mutex_trylock(&__q_mutex))
                        {
                                pthread_cond_signal(&_condition);       // 通知消费线程可以取任务
                                _users--;
                                pthread_mutex_unlock(&__q_mutex);
                        }
                }

        }

private:
        std::queue<T> __tq;     // 任务队列
//        std::mutex __q_mutex;   // 任务锁
        int _users;             // 使用者数量
        std::atomic<int>     __tq_size;   // 任务量
        pthread_mutex_t __q_mutex;   // 任务锁
        pthread_cond_t _condition;
};

#endif  // __TASK_H__
