
#include <cassert>
#include "vector.h"

int main()
{
	Vector<int> v(100);
	cout << v.capacity() << endl;

	v.push_back(10);
	cout << v.capacity() << endl;
	v.push_back(20);

	cout << v.capacity() << endl;
	assert(v.capacity() == 100);

	assert(v[0] == 10);
	assert(v[1] == 20);
	assert(v.size() == 2);	

	v[1] = 100;
	assert(v[1] == 100);

	return 0;
}
