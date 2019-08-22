

#if 1
#include <iostream>
#include <list>
#include <typeinfo>
#include <functional>

using namespace std;
// 最小堆
template<class T>
class Heap
{
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
                typename list<T>::iterator it = _heap.begin();
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
                T item = _heap.front();
                _heap.pop_front();

                return item;
        }

        void print()
        {
                for (auto& obj: _heap)
                {
                        cout << obj << endl;
                }
        }

protected:
        list<T>& getHeap()
        {
                return _heap;
        }

private:
        list<T> _heap;  // 方便随机删除和插入
};

typedef std::function<void()> TimerCallback;

/* 定时器 */
class Timer
{
public:
        Timer(TimerCallback cb, void* param, const int& interval)
                : _interval(interval)
        {

        }

        ~Timer() {}

        /* 返回定时器触发间隔时间 */
        const int& interval()
        {
                return _interval;
        }

        /* 设置定时器触发间隔时间 */
        const int& setInterval(const int& interval)
        {
                _interval = interval;
        }

        bool operator<(Timer& right)
        {
                cout << "<" << endl;
                return (this->interval() < right.interval());
        }

        bool operator>(Timer& right)
        {
                cout << ">" << endl;
                return (this->interval() > right.interval());
        }
private:
        int _interval;
};

/* 定时器管理器，应具备如下结构：
 * （1）该类为单例模式；
 * （2）该类管理timer池，所有新增的timer都加入池中；
 * （3）该类的单例对象，由单个独立的线程执行；
 * （4）该类应包含其他线程任务队列的句柄，timer触发的回调函数的执行，通过Task队列，传递给线程池执行
 */
class TimerManager: public Heap<Timer>
{
public:
        TimerManager()
                : Heap()
        {

        }

        virtual ~TimerManager()
        {

        }

        /* 添加一个定时器 */
        void addTimer(Timer t)
        {
                this->push(t);
        }

        void print()
        {
                for (auto& obj: getHeap())
                {
                        cout << obj.interval() << endl;
                }
        }
};

int main()
{
        TimerManager tm;
        for (int i = 10; i > 0; i--)
        {
                Timer t(nullptr, NULL, i);
                tm.addTimer(t);
        }

        tm.print();

        return 0;
}
#else
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
// 时间轮实现定时器
#define TIME_WHEEL_SIZE 8

typedef void (*func)(int data);

struct timer_node
{
    struct timer_node *next;
    int rotation;
    func proc;
    int data;
};

struct timer_wheel
{
    struct timer_node *slot[TIME_WHEEL_SIZE];
    int current;
};

struct timer_wheel timer = {{0}, 0};

void sigHandler(int sig)
{
    // 使用二级指针删进行单链表的删除
    struct timer_node **cur = &timer.slot[timer.current];
    while (*cur)
    {
        struct timer_node *curr = *cur;
        if (curr->rotation > 0)
        {
            curr->rotation--;
            cur = &curr->next;
        }
        else
        {
            curr->proc(curr->data);
            *cur = curr->next;
            free(curr);
        }
    }
    timer.current = (timer.current + 1) % TIME_WHEEL_SIZE;
    alarm(1);
}

void add_timer(int len, func action)
{
    int pos = (len + timer.current) % TIME_WHEEL_SIZE;
    struct timer_node *node = (struct timer_node*)malloc(sizeof(struct timer_node));

    // 插入到对应格子的链表头部即可, O(1)复杂度
    node->next = timer.slot[pos];
    timer.slot[pos] = node;
    node->rotation = len / TIME_WHEEL_SIZE;
    node->data = 0;
    node->proc = action;
}

// test case1: 1s循环定时器
int g_sec = 0;
void do_time1(int data)
{
    printf("timer %s, %d\n", __FUNCTION__, g_sec++);
    add_timer(1, do_time1);
}

// test case2: 2s单次定时器
void do_time2(int data)
{
    printf("timer %s\n", __FUNCTION__);
}

// test case3: 9s循环定时器
void do_time9(int data)
{
    printf("timer %s\n", __FUNCTION__);
    add_timer(9, do_time9);
}

int main()
{
    signal(SIGALRM, sigHandler);
    alarm(1); // 1s的周期心跳

    // test
    add_timer(1, do_time1);
    add_timer(2, do_time2);
    add_timer(3, do_time9);

    while(1) pause();
    return 0;
}
#endif