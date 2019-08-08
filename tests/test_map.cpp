
#include <iostream>
#include <cassert>
#include "map.h"

using namespace std;

int main()
{
	Map<int, int> m;
	m[10] = 1000;
	assert(m[10] == 1000);

	Map<int, int>::iterator it = m.find(10);
	assert(it->first == 10);

	assert(m.size() == 1);
	m.erase(10);
	assert(m.size() == 0);

	return 0;
}
