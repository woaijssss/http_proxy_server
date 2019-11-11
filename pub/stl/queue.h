/*
 * queue.h
 *
 *  Created on: Sep 20, 2019
 *      Author: wenhan
 */

#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <queue>
#include <mutex>

template <class __T>
class Queue: public std::queue<__T>
{
        using value_type = typename std::queue<__T>::value_type;
        using size_type  = typename std::queue<__T>::size_type;
public:
        explicit Queue()
                : std::queue<__T>()
        {

        }

        explicit Queue(size_type n)
                : std::queue<__T>(n)
        {

        }

        void push(const value_type& val)
        {
                std::lock_guard<std::mutex> lock(m_mutex);
                std::queue<__T>::push(val);
        }

        /* 一般 front() 函数后，紧接着就是 pop() 函数，
         * 因此，外部使用时，返回值必须复制一份，否则可能会导致取空
         */
        const __T& front()
        {
                std::lock_guard<std::mutex> lock(m_mutex);
                return std::queue<__T>::front();
        }

        void pop()
        {
                std::lock_guard<std::mutex> lock(m_mutex);
                std::queue<__T>::pop();
        }

        void clear()
        {
                std::lock_guard<std::mutex> lock(m_mutex);
                std::queue<__T>::clear();
        }
private:
        std::mutex m_mutex;
};

#endif /* __QUEUE_H__ */
