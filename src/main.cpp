
#include <string.h>
#include <iostream>
#include <errno.h>
#include <stdio.h>
#include <signal.h>

#include "WHPSHttpServer.h"

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
        signal(SIGUSR1, sigHandler);
        signal(SIGUSR2, sigHandler);
        signal(SIGINT, sigHandler); 
        signal(SIGPIPE, SIG_IGN);

        // p_server = WHPSTcpServer::Get();
        WHPSHttpServer& server_ptr = GetWHPSHttpServer();
        p_server = &server_ptr;

        server_ptr.start();

        return 0;
}
