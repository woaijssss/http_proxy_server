
#include <iostream>
#include <algorithm>
#include <typeinfo>
#include <map>
#include <memory>
#include <string.h>

#include "DynamicFactory.h"

using namespace std;

class Basic1 : public WhpsObject<Basic1>
{
public:
        Basic1() {}
        virtual ~Basic1() {}

        virtual void doGet()
        {
                cout << "doGet..." << endl;
        }
};

int main()
{
        HttpWhps* p1 = HttpWhpsFactory::Instance().get("Basic1");
        p1->doGet();

        return 0;
}