
#include "WHPSTcpServer.h"

#include <string.h>
#include <iostream>
#include <errno.h>
#include <stdio.h>
#include <signal.h>

using namespace std;

WHPSTcpServer* p_server = NULL;

void sigHandler(int sig)
{
        if (p_server)
        {
                p_server->stop();
        }

        exit(0);
}

int main(int argc, char const *argv[])
{
        signal(SIGUSR1, sigHandler);
        signal(SIGUSR2, sigHandler);
        signal(SIGINT, sigHandler); 
        signal(SIGPIPE, SIG_IGN);

        p_server = WHPSTcpServer::Get();

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
