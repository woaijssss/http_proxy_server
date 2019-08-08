
#include <iostream>
#include <thread>
#include <mutex>
#include <queue>

#include <unistd.h>

using namespace std;

std::mutex g_mutex;
std::queue<int> g_q;

void f1()
{
	while (true) {
		g_mutex.lock();
		g_mutex.lock();
		//cout << "f1: " << std::this_thread::get_id() << endl;
		int v = g_q.front();
		g_q.pop();
		cout << v << "----" << g_q.size() << endl;
		g_mutex.unlock();
	}
}

void f2()
{
	int i = 0;
	while (true) {
		cout << "f2: " << std::this_thread::get_id() << endl;
		if (i++ == 2) {
			i = 0;
			g_q.push(10);
			g_mutex.unlock();
		}
		sleep(1);
	}
	//std::bind(&f1);
}

int main(int argc, char const *argv[])
{
	std::thread t1(f1);
	std::thread t2(f2);

	t1.join();
	t2.join();

	return 0;
}