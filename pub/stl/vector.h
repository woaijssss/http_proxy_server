
#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <vector>
#include <mutex>
#include <algorithm>

/* 线程安全的vector */
template <class __T>
class Vector : public std::vector<__T>
{
public:
        using value_type = typename std::vector<__T>::value_type;
        using size_type  = typename std::vector<__T>::size_type;
        using iterator = typename std::vector<__T>::iterator;
public:
        explicit Vector()
                : std::vector<__T>()
        {

        }

        explicit Vector(size_type n)
                : std::vector<__T>(n)
        {

        }

        void push_back(const value_type& v)
        {
                std::lock_guard<std::mutex> lock(m_mutex);
                std::vector<__T>::push_back(v);
        }

        __T& operator[](size_type i)
        {
                std::lock_guard<std::mutex> lock(m_mutex);
                return std::vector<__T>::operator[](i);
        }

        size_type capacity()
        {
                std::lock_guard<std::mutex> lock(m_mutex);
                return std::vector<__T>::capacity();
        }

        bool erase(const value_type& v)
        {
                std::lock_guard<std::mutex> lock(m_mutex);
                bool res = false;
                iterator it = std::find(std::vector<__T>::begin(), std::vector<__T>::end(), v);

                if (it != std::vector<__T>::end())
                {
                        std::vector<__T>::erase(it);
                        res = true;
                }

                return res;
        }
private:
        std::mutex m_mutex;
};

#endif  // __VECTOR_H__
