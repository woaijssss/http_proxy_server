
#include <iostream>
#include <algorithm>
#include <typeinfo>
#include <map>
#include <memory>
#include <string.h>

using namespace std;

class Abstract
{
public:
        virtual ~Abstract() {}
        virtual void foo() = 0;
};

// 写成类的原因是函数内部可以定义类，不能定义函数
#define GETCLASS(__x) \
        class N \
        { \
        public: \
                static Abstract* create() \
                { \
                        return (new __x()); \
                } \
        };

class Basic: public Abstract
{
public:
        Basic() {}
        virtual ~Basic() {}

        // 重写父类方法
        virtual void foo()
        {
                cout << "Basic::foo" << endl;
        }

private:
        int i, j, k, l, s, p;
};

void func(string className)
{
        // GETCLASS(Basic)         // 写在这里的目的，是为了运行期实例化，而不是编译期
        // GETCLASS("Basic")
        // GETCLASS()

        // Abstract* p = N::create(Basic);
        shared_ptr<Abstract> p1;
        {
                shared_ptr<Abstract> p(new Basic());
                p1 = p;
        }

        // std::copy(p1, p, sizeof(Basic));

        p1->foo();
}

int main()
{
        func("Basic");
        return 0;
}