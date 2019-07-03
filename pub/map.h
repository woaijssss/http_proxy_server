#ifndef __MAP_H__
#define __MAP_H__

#include <map>
#include <mutex>

#include <iostream>
using namespace std;

template<class KEY, class VALUE>
class Map: public std::map<KEY, VALUE> {
public:
        void insert(const KEY& key, const VALUE& value)
        {
                std::lock_guard<std::mutex> lock(__mutex);
                std::map<KEY, VALUE>::insert(std::pair<KEY, VALUE>(key, value));
        }

        void erase(const KEY& key)
        {
                std::lock_guard<std::mutex> lock(__mutex);
                std::map<KEY, VALUE>::erase(key);
        }

        VALUE& operator[](const KEY& key)
        {
                std::lock_guard<std::mutex> lock(__mutex);
                return std::map<KEY, VALUE>::operator[](key);
        }

        size_t size()
        {
                std::lock_guard<std::mutex> lock(__mutex);
                return std::map<KEY, VALUE>::size();
        }
public:
        std::mutex __mutex;
};

#endif  // __MAP_H__