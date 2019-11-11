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
        Heap(int flag = 0)
                : m_flag(flag),
                  m_heap()
        {

        }

        virtual ~Heap()
        {

        }

public:
        /* 插入一个新的节点 */
        void push(T& item)
        {
                /* 当前插入方式为从前遍历
                 * 实际堆中的数据，从生成开始就是有序的
                 * 因此，每插入一个数据，后面可以做成二分查找，提高效率
                 */
                std::lock_guard<std::mutex> lock(m_mutex);
                HeapIterator it = m_heap.begin();
                for (; it != m_heap.end(); ++it)
                {
                        /* 若使用 >= 可能存在有些节点一直取不到
                         * 仅使用 > 进行比较，提高鲁棒性
                         */
                        if (!m_flag ? item < *it : item > *it)
                        {
                                m_heap.insert(it, item);
                                /* 因为这里只插入一个节点后，break退出循环，所以不存在迭代器失效的问题 */
                                // it++;
                                return;
                        }
                }

                m_heap.push_back(item);  // 如果输入的值最大，则直接从末尾插入
        }

        /* 取出堆顶的值，并从堆中删除该节点 */
        T pop()
        {
                T item;
                std::lock_guard<std::mutex> lock(m_mutex);
                if (m_heap.size())
                {
                        item = m_heap.front();
                        m_heap.pop_front();
                }

                return item;
        }

protected:
        /* 返回堆首元素 */
        T front()
        {
                std::lock_guard<std::mutex> lock(m_mutex);
                return m_heap.front();
        }

        /* 返回当前堆的大小 */
        size_t size()
        {
                std::lock_guard<std::mutex> lock(m_mutex);
                return m_heap.size();
        }

        /* 从堆中查找指定的元素 */
        HeapIterator find(T& item)
        {
//                std::lock_guard<std::mutex> lock(m_mutex);
                // HeapIterator it = std::find(m_heap.begin(), m_heap.end(), item);
                HeapIterator it = m_heap.begin();

                for (; it != m_heap.end(); ++it)
                {
                        if (*it == item)
                        {
                                return it;
                        }
                }

                return m_heap.end();
        }

        /* 从堆中删除指定元素 */
        bool erase(T& item)
        {
                std::lock_guard<std::mutex> lock(m_mutex);
                HeapIterator it = this->find(item);

//                if (it != this->getHeap().end())
                if (it != m_heap.end())
                {
                        m_heap.erase(it);

                        return true;
                }

                return false;
        }

        /* 返回当前堆对象 */
        std::list<T>& getHeap()
        {
                return m_heap;
        }

private:
        int m_flag;          // 堆类型标识：0为最小堆(默认)，1为最大堆
        std::list<T> m_heap;  // 方便随机删除和插入
        std::mutex m_mutex;
};

#endif  // __HEAP_H__
