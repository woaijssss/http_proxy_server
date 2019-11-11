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
                std::lock_guard<std::mutex> lock(m_mutex);
                std::list<__T>::push_back(val);
        }

        /* 一般 front() 函数后，紧接着就是 pop_front() 函数，
         * 因此，外部使用时，返回值必须复制一份，否则可能会导致取空
         */
//        const __T& front()
        __T front()
        {
                std::lock_guard<std::mutex> lock(m_mutex);
                __T res = nullptr;

                if (!std::list<__T>::empty())
                {
                		res = std::list<__T>::front();
                }

                return res;
        }

        size_type size()
        {
                std::lock_guard<std::mutex> lock(m_mutex);
                return std::list<__T>::size();
        }

        bool empty()
        {
            	std::lock_guard<std::mutex> lock(m_mutex);
            	return std::list<__T>::empty();
        }

        void pop_front()
        {
                std::lock_guard<std::mutex> lock(m_mutex);
                if (!std::list<__T>::empty())
                {
                    	std::list<__T>::pop_front();
                }
        }

        bool erase(const value_type& v)
        {
                std::lock_guard<std::mutex> lock(m_mutex);
                bool res = false;
//                iterator it = std::find(std::list<__T>::begin(), std::list<__T>::end(), v);

                for (iterator it = std::list<__T>::begin(); it != std::list<__T>::end(); it++)
                {
                		if (*it == v)
                		{
                				std::list<__T>::erase(it);
                				res = true;
                				break;
                		}
                }

//                if (it != std::list<__T>::end())
//                {
//                        std::list<__T>::erase(it);
//                        res = true;
//                }

                return res;
        }

        void clear()
        {
                std::lock_guard<std::mutex> lock(m_mutex);
                std::list<__T>::clear();
        }

private:
        std::mutex m_mutex;
};

#endif /* __LIST_H__ */
