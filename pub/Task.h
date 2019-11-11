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
template<class T>
class Task
{
public:
        Task(const int& users)
                : m_used(users),
                  m_tqSize(0),
                  m_qMutex(PTHREAD_MUTEX_INITIALIZER),
                  m_condition(PTHREAD_COND_INITIALIZER)
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
//                std::lock_guard<std::mutex> lock(m_qMutex);
                pthread_mutex_lock(&m_qMutex);
                m_tq.push(task);
                m_tqSize++;    // 任务数加1
                pthread_mutex_unlock(&m_qMutex);
                pthread_cond_signal(&m_condition);       // 通知消费线程可以取任务
        }

        /* 从任务队列获取一个任务
         * 消费者调用
         */
        inline T get()
        {
//                std::lock_guard<std::mutex> lock(m_qMutex);

                pthread_mutex_lock(&m_qMutex);
                T task;

                while (!m_tqSize)
                {
                        pthread_cond_wait(&m_condition, &m_qMutex); //api做了三件事情 //pthread_cond_wait假醒
                }

                if (m_tqSize > 0)
                {
                        task = m_tq.front();
                        m_tq.pop();          // 是否要保证任务成功执行再删除(待定)
                        m_tqSize--;    // 任务数减1
                }

                pthread_mutex_unlock(&m_qMutex);

                return task;
        }

        /* 获取当前Task队列的任务数 */
        const size_t& size() const
        {
                return m_tqSize;
        }

        /* 停止生产线运作
         */
        void stop()
        {
                m_tqSize = -1;

                while (m_used > 0)
                {
                        if (!pthread_mutex_trylock(&m_qMutex))
                        {
                                pthread_cond_signal(&m_condition);       // 通知消费线程可以取任务
                                m_used--;
                                pthread_mutex_unlock(&m_qMutex);
                        }
                }

        }

private:
        std::queue<T> m_tq;     // 任务队列
//        std::mutex m_qMutex;   // 任务锁
        int m_used;             // 使用者数量
        std::atomic<int> m_tqSize;   // 任务量
        pthread_mutex_t m_qMutex;   // 任务锁
        pthread_cond_t m_condition;
};

#endif  // __TASK_H__
