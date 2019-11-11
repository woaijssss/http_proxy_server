#ifndef __MAP_H__
#define __MAP_H__

#include <map>
#include <mutex>

/* 线程安全的map */
template<class __K, class __V>
class Map: public std::map<__K, __V> 
{
public:
        using iterator = typename Map<__K, __V>::iterator;
public:
        void insert(const __K& k, const __V& v)
        {
                std::lock_guard<std::mutex> lock(m_mutex);
                std::map<__K, __V>::insert(std::pair<__K, __V>(k, v));
        }

        void erase(const __K& k)
        {
                std::lock_guard<std::mutex> lock(m_mutex);
                std::map<__K, __V>::erase(k);
        }

        __V& operator[](const __K& k)
        {
                std::lock_guard<std::mutex> lock(m_mutex);
                return std::map<__K, __V>::operator[](k);
        }

        size_t size()
        {
                std::lock_guard<std::mutex> lock(m_mutex);
                return std::map<__K, __V>::size();
        }

        iterator find(const __K& k)
        {
                std::lock_guard<std::mutex> lock(m_mutex);
                return std::map<__K, __V>::find(k);
        }

        const __V& at(const __K& k)
        {
                std::lock_guard<std::mutex> lock(m_mutex);
                return std::map<__K, __V>::at(k);
        }

        void swap(std::map<__K, __V> x)
        {
                std::lock_guard<std::mutex> lock(m_mutex);
                std::map<__K, __V>::swap(x);
        }

        void clear()
        {
                std::lock_guard<std::mutex> lock(m_mutex);
                std::map<__K, __V>::clear();
        }

private:
        std::mutex m_mutex;
};

#endif  // __MAP_H__
