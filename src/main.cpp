
#include "WHPSTcpServer.h"

#include <string.h>
#include <iostream>
using namespace std;

int main(int argc, char const *argv[])
{
        WHPSTcpServer* p_socket = WHPSTcpServer::Get();

        while (true)
        {
                struct sockaddr_in c_addr;
                memset(&c_addr, 0, sizeof(c_addr));
                cout << "wait..." << endl;
                int c_fd = p_socket->getSocket().accept(c_addr);
                cout << "an client has been connected..." << endl;
                cout << ntohs(c_addr.sin_addr.s_addr) << endl;
        }

        return 0;
}