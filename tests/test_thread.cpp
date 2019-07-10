
#include <vector>
#include <string>
#include <iostream>
#include <unistd.h>

#include "ThreadPool.h"

using namespace std;

void func(int i, const string& s)
{
        thread__id this_id = std::hash<std::thread::id>()(std::this_thread::get_id());
        cout << i << "---" << s << "=====" << this_id << endl;
}

void delay(ThreadPool& tp, int time)
{
        while (time-- > 0)
        {
                tp.testTask(std::bind(&func, 100, "123123"));
                sleep(1);
        }
}

int main()
{
        ThreadPool tp(5);
        tp.start();
        delay(tp, 3);

        return 0;
}