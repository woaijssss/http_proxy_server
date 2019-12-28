#include <string.h>
#include <iostream>
#include <errno.h>
#include <stdio.h>
#include <signal.h>

#include "WHPSHttpServer.h"
#include "util.h"

using namespace std;

static WHPSHttpServer* p_server = nullptr;
static WHPSHttpServer* p_sserver = nullptr;

void sigHandler(int sig)
{
        if (p_server)
        {
                p_server->stop();
        }

        if (p_sserver)
        {
                p_sserver->stop();
        }

        exit(0);
}

void func()
{
        p_sserver = new WHPSHttpServer(443);
        p_sserver->start();
}

int main(int argc, char const *argv[])
{
        signal(SIGPIPE, SIG_IGN );
        signal(SIGUSR1, sigHandler);
        signal(SIGUSR2, sigHandler);
        signal(SIGINT, sigHandler);

        initConfig();   // 初始化加载配置文件
//        p_server = &GetWHPSHttpServer();
//        p_server->start();
        p_server = new WHPSHttpServer(80);
        std::thread thrd = std::thread(func);
        thrd.detach();
        p_server->start();

        return 0;
}
