
#ifndef __TASK_H__
#define __TASK_H__

#include <queue>
#include <mutex>

/* 线程任务类
 * 考虑到可能有不同类型的线程池，因此将任务类型做成模板，以匹配任何的数据类型
 */
template <class T>
class Task
{
public:
        Task();
        ~Task();

public:
        /* 向任务队列添加一个任务 */
        inline void addTask(T task)
        {
                std::lock_guard<std::mutex> lock(__mutex);
                __tq.push(task);
        }

        /* 从任务队列获取一个任  */
        inline T get()
        {
                std::lock_guard<std::mutex> lock(__mutex);
                T task = __tq.front();
                // __tq.pop();          // 是否要保证任务成功执行再删除(待定)
        }

private:
        std::queue<T> __tq;     // 任务队列
        std::mutex __q_mutex;   // 任务锁
};

#endif  // __TASK_H__