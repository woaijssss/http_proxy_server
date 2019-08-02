
class C;
class A
{
public:
	void func()
	{}
private:
	//A(){}
	A(...){}
	friend class C;
};

class B: public A
{
public:
	B():A(){}
}

class C: public A
{
public:
	C(){}
};

int main()
{
	C c;	
	c.func();

	return 0;
}
