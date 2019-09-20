/*
 * list.h
 *
 *  Created on: Sep 20, 2019
 *      Author: wenhan
 */

#ifndef __LIST_H__
#define __LIST_H__

#include <list>
#include <mutex>
#include <algorithm>

template <class __T>
class List: public std::list<__T>
{
        using value_type = typename std::list<__T>::value_type;
        using size_type = typename std::list<__T>::size_type;
        using iterator = typename std::list<__T>::iterator;
public:
        explicit List()
                : std::list<__T>()
        {

        }

        explicit List(size_type n)
                : std::list<__T>(n)
        {

        }

        void push_back(const value_type& val)
        {
                std::lock_guard<std::mutex> lock(__mutex);
                std::list<__T>::push_back(val);
        }

        /* 一般 front() 函数后，紧接着就是 pop_front() 函数，
         * 因此，外部使用时，返回值必须复制一份，否则可能会导致取空
         */
        const __T& front()
        {
                std::lock_guard<std::mutex> lock(__mutex);
                return std::list<__T>::front();
        }

        size_type size()
        {
                std::lock_guard<std::mutex> lock(__mutex);
                return std::list<__T>::size();
        }

        void pop_front()
        {
                std::lock_guard<std::mutex> lock(__mutex);
                std::list<__T>::pop_front();
        }

        bool erase(const value_type& v)
        {
                std::lock_guard<std::mutex> lock(__mutex);
                bool res = false;
                iterator it = std::find(std::list<__T>::begin(), std::list<__T>::end(), v);

                if (it != std::list<__T>::end())
                {
                        std::list<__T>::erase(it);
                        res = true;
                }

                return res;
        }

        void clear()
        {
                std::lock_guard<std::mutex> lock(__mutex);
                std::list<__T>::clear();
        }

private:
        std::mutex __mutex;
};

#endif /* __LIST_H__ */
