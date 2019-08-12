
#include <iostream>
#include <algorithm>
#include <typeinfo>
#include <map>
#include <memory>
#include <string.h>

#include "DynamicFactory.h"

using namespace std;

class Basic111 : public WhpsObject<Basic111>
{
public:
        Basic111() {}
        virtual ~Basic111() {}

        virtual void doGet()
        {
                cout << "doGet..." << endl;
        }
};

int main()
{
        shared_ptr<HttpWhps> p1 = HttpWhpsFactory::Instance().get("Basic111");
        p1->doGet();

        return 0;
}