
#include <string.h>
#include <iostream>
#include <errno.h>
#include <stdio.h>
#include <signal.h>

#include "WHPSHttpServer.h"
#include "util.h"

using namespace std;

WHPSHttpServer* p_server = NULL;

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
        signal(SIGPIPE, SIG_IGN);
        signal(SIGUSR1, sigHandler);
        signal(SIGUSR2, sigHandler);
        signal(SIGINT, sigHandler); 

        initConfig();   // 初始化加载配置文件

#if 1
        WHPSHttpServer& server_ptr = GetWHPSHttpServer();
        p_server = &server_ptr;

        server_ptr.start();
#endif

        return 0;
}
