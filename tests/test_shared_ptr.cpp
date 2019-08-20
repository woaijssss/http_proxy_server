
#include <memory>

using namespace std;

class A: public std::enable_shared_from_this<A>
{
public:
        A()
        {
                shared_ptr<A> p = shared_from_this();
        }

        ~A() noexcept
        {
                
        }
};

int main(int argc, char const *argv[])
{
        A a;

        return 0;
}