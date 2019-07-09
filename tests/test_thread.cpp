
#include <vector>
#include <unistd.h>

#include "ThreadPool.h"

using namespace std;

void delay(int time)
{
        while (time-- > 0)
        {
                sleep(1);
        }
}

int main()
{
        ThreadPool tp(10);
        tp.start();

        int i = 0;
        delay(5);

        return 0;
}