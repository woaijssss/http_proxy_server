
#ifndef __HELLO_WHPS_H__
#define __HELLO_WHPS_H__

#include "HttpWhps.h"

class HelloWhps: public HttpWhps
{
public:
        HelloWhps();
        virtual ~HelloWhps();

public:
        virtual void doGet(HttpWhpsRequest request, HttpWhpsResponse response);

        virtual void doPost(HttpWhpsRequest request, HttpWhpsResponse response);

private:
};

#endif  //