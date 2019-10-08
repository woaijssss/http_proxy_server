
#include "HelloWhps.h"

#include <iostream>
using namespace std;

HelloWhps::HelloWhps()
{

}

HelloWhps::~HelloWhps()
{

}

void HelloWhps::doGet(HttpWhpsRequest& request, HttpWhpsResponse& response)
{
        cout << "HelloWhps::doGet" << endl;
}

void HelloWhps::doPost(HttpWhpsRequest& request, HttpWhpsResponse& response)
{

}
