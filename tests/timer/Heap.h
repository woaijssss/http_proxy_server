
#ifndef __HEAP_H__
#define __HEAP_H__

#include <list>
#include <mutex>
#include <algorithm>

// 最小堆
template<class T>
class Heap
{
public:
        using HeapIterator = typename std::list<T>::iterator;
public:
        Heap()
                : _heap()
        {

        }

        virtual ~Heap()
        {

        }

public:
        /* 插入一个新的节点 */
        void push(T item)
        {
                /* 当前插入方式为从前遍历
                 * 实际堆中的数据，从生成开始就是有序的
                 * 因此，每插入一个数据，后面可以做成二分查找，提高效率
                 */
                typename std::list<T>::iterator it = _heap.begin();
                for (; it != _heap.end(); ++it)
                {
                        /* 若使用 >= 可能存在有些节点一直取不到
                         * 仅使用 > 进行比较，提高鲁棒性
                         */
                        if (item < *it)
                        {
                                _heap.insert(it, item);
                                /* 因为这里只插入一个节点后，break退出循环，所以不存在迭代器失效的问题 */
                                // it++;

                                return;
                        }
                }

                _heap.push_back(item);  // 如果输入的值最大，则直接从末尾插入
        }

        /* 取出堆顶的值，并从堆中删除该节点 */
        T pop()
        {
                T item;
                // std::lock_guard<std::mutex> lock(_mutex);
                if (_heap.size())
                {
                        item = _heap.front();
                        _heap.pop_front();
                }

                return item;
        }

        void print()
        {
                for (auto& obj: _heap)
                {
                        std::cout << obj << std::endl;
                }
        }

protected:
        T front()
        {
                // std::lock_guard<std::mutex> lock(_mutex);
                return _heap.front();
        }

        size_t size()
        {
                // std::lock_guard<std::mutex> lock(_mutex);
                return _heap.size();
        }

        HeapIterator find(const T& item)
        {
                // std::lock_guard<std::mutex> lock(_mutex);
                // HeapIterator it = std::find(_heap.begin(), _heap.end(), item);
                HeapIterator it = _heap.begin();

                for (; it != _heap.end(); ++it)
                {
                        if (*it == item)
                        {
                                return it;
                        }
                }

                return _heap.end();
        }

        void erase(HeapIterator hit)
        {
                // std::lock_guard<std::mutex> lock(_mutex);
                _heap.erase(hit);
        }

        std::list<T>& getHeap()
        {
                return _heap;
        }

private:
        std::list<T> _heap;  // 方便随机删除和插入
        std::mutex _mutex;
};

#endif  // __HEAP_H__