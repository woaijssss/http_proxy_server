
#ifndef __TASK_H__
#define __TASK_H__

#include <queue>
#include <mutex>
#include <functional>

using task_func_t = std::function<void()>;

/* 线程任务类
 * 考虑到可能有不同类型的线程池，因此将任务类型做成模板，以匹配任何的数据类型
 */
template <class T>
class Task
{
public:
        Task()
                : __tq_size(0)
        {

        }

        ~Task()
        {

        }

public:
        /* 当前锁是对队列操作的全局锁，数据量大时，效率低
         * 后面设计成局部锁，或更改逻辑使得不需要加锁
         */
        /* 向任务队列添加一个任务 */
        inline void addTask(T task)
        {
                std::lock_guard<std::mutex> lock(__q_mutex);
                __tq.push(task);
                __tq_size++;    // 任务数加1
        }

        /* 从任务队列获取一个任务 */
        inline T get()
        {
                std::lock_guard<std::mutex> lock(__q_mutex);
                T task;

                if (__tq_size)
                {
                        task = __tq.front();
                        __tq.pop();          // 是否要保证任务成功执行再删除(待定)
                        __tq_size--;    // 任务数减1
                }

                return task;
        }

        /* 获取当前Task队列的任务数 */
        const size_t& size() const
        {
                return __tq_size;
        }

private:
        std::queue<T> __tq;     // 任务队列
        std::mutex __q_mutex;   // 任务锁
        size_t     __tq_size;   // 任务量
};

#endif  // __TASK_H__