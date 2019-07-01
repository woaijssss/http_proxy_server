
#include "Socket.h"

#include <string.h>
#include <iostream>
using namespace std;

int main(int argc, char const *argv[])
{
        Socket s_fd;
        cout << s_fd.setReuseAddr() << endl;
        s_fd.bind(3000);
        s_fd.listen();

        while (true)
        {
                struct sockaddr_in c_addr;
                memset(&c_addr, 0, sizeof(c_addr));
                cout << "wait..." << endl;
                int c_fd = s_fd.accept(c_addr);
                cout << "an client has been connected..." << endl;
                cout << ntohs(c_addr.sin_addr.s_addr) << endl;
        }

        return 0;
}