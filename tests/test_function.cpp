/*
 * test_function.cpp
 *
 *  Created on: Sep 25, 2019
 *      Author: wenhan
 */

#include <functional>
using namespace std;

void func()
{

}

int main(int argc, char** argv)
{
	std::function<void()> f = std::bind(&func);
	std::function<void()> f1;
	f = nullptr;
	f1();
	
	return 0;
}
