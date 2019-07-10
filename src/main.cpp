
#include "WHPSTcpServer.h"

#include <string.h>
#include <iostream>
#include <errno.h>
#include <stdio.h>

using namespace std;

int main(int argc, char const *argv[])
{
        WHPSTcpServer* p_server = WHPSTcpServer::Get(1024, 10);

        if (!p_server->start())         // tcp服务启动失败，则退出进程
        {
                cout << "errno: " << errno << endl;
                return -1;
        }

        p_server->startLoop();

        // while (true)
        // {
        //         struct sockaddr_in c_addr;
        //         memset(&c_addr, 0, sizeof(c_addr));
        //         cout << "wait..." << endl;
        //         int c_fd = p_socket->getSocket().accept(c_addr);
        //         cout << "an client has been connected..." << endl;
        //         cout << ntohs(c_addr.sin_addr.s_addr) << endl;
        // }

        return 0;
}
