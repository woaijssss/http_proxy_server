
#include <iostream>
#include <unistd.h>
#include "WHPSTimer.h"
using namespace std;

void func(WHPSTimer& t)
{
        cout << t.id() << endl;
}

int main()
{
#if 1
        Heap<int> heap(1);
        heap.push(9);
        heap.push(11);
        heap.push(2);
        heap.push(5);
        heap.push(4);
        heap.push(8);
        heap.push(1);

        heap.print();
#else
        WHPSTimer t1, t7;
        for (int i = 10; i > 0; i--)
        {
                WHPSTimer t(std::bind(&func, std::placeholders::_1), NULL, i*1);

                if (i == 1)
                {
                        t1 = t;
                        t1.start();
                        continue;
                }

                if (i == 7)
                {
                        t7 = t;
                        t7.start();
                        continue;
                }
                t.start();
        }

        while (true)
        {
                t1.stop();
                t7.stop();
                usleep(10);
                t1.start();
                t7.start();
                // sleep(5);
        }
#endif

        return 0;
}