
#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <vector>
#include <mutex>

/* 线程安全的vector */
template <class __T>
class Vector : public std::vector<__T>
{
public:
        using value_type = typename std::vector<__T>::value_type;
        using size_type  = typename std::vector<__T>::size_type;
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
                std::lock_guard<std::mutex> lock(__mutex);
                std::vector<__T>::push_back(v);
        }

        __T& operator[](size_type i)
        {
                std::lock_guard<std::mutex> lock(__mutex);
                return std::vector<__T>::operator[](i);
        }

        size_type capacity()
        {
                std::lock_guard<std::mutex> lock(__mutex);
                return std::vector<__T>::capacity();
        }
private:
        std::mutex __mutex;
};

#endif  // __VECTOR_H__