#include <string.h>
#include <iostream>
#include <errno.h>
#include <stdio.h>
#include <signal.h>

#include "WHPSHttpServer.h"
#include "util.h"

using namespace std;

static WHPSHttpServer* p_server = NULL;

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
        signal(SIGPIPE, SIG_IGN );
        signal(SIGUSR1, sigHandler);
        signal(SIGUSR2, sigHandler);
        signal(SIGINT, sigHandler);

        initConfig();   // 初始化加载配置文件
        p_server = &GetWHPSHttpServer();
        p_server->start();

        return 0;
}
