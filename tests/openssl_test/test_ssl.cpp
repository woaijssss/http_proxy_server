/*
 * test_ssl.cpp
 *
 *  Created on: Dec 20, 2019
 *      Author: wenhan
 */

// HttpsServer.cpp : 定义控制台应用程序的入口点。
//
#include <iostream>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <sys/socket.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>

#include "WHPSSslManager.h"

using namespace std;

int main()
{
        int hSocket = socket(AF_INET, SOCK_STREAM, 0);
        struct sockaddr_in so_addr;
        memset(&so_addr, 0, sizeof(so_addr));
        so_addr.sin_family = AF_INET;   // 设置地址族
        so_addr.sin_port = htons(443);         // 绑定端口
        so_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        int res = bind(hSocket, (struct sockaddr*) &so_addr, sizeof(so_addr));
        int v = 1;
        setsockopt(hSocket, SOL_SOCKET, SO_REUSEADDR, &v, sizeof(v));
        listen(hSocket, 5);

#if 0
    SSL *ssl;
    SSL_CTX *sslCtx;
    const SSL_METHOD *sslMethod;
    X509 *serverCertification;

    const char* certificateKey = "./openssl_test/ca.crt";
    const char* privateKey = "./openssl_test/ca.key";

    // 库初始化
    SSL_library_init();
    //
//    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();

    sslMethod = SSLv23_server_method();
    if (NULL == sslMethod)
    {
        printf("NULL == sslMethod");
        return 0;
    }

    sslCtx = SSL_CTX_new(sslMethod);
    if (NULL == sslCtx)
    {
        printf("NULL == sslCtx");
        return 0;
    }

    //ssl = SSL_new(sslCtx);
    //if (NULL == ssl)
    //{
    //    return 0;
    //}

    int ret = SSL_CTX_use_certificate_file(sslCtx, certificateKey, SSL_FILETYPE_PEM);
    if (ret < 0)
    {
//        ERR_print_errors_fp(stdout);
        return 0;
    }

    ret = SSL_CTX_use_PrivateKey_file(sslCtx, privateKey, SSL_FILETYPE_PEM);
    if (ret < 0)
    {
//        ERR_print_errors_fp(stdout);
        return 0;
    }

    ret = SSL_CTX_check_private_key(sslCtx);
#else
        WHPSSslManager sslMgr;
        sslMgr.init();

#endif

        while (1)
        {
                SSL* ssl;
                struct sockaddr_in clientAddr;
                int hClientSocket;
                socklen_t len = sizeof(clientAddr);

//        hClientSocket = accept(hSocket, (sockaddr*)&clientAddr, &len);
                hClientSocket = accept(hSocket, (struct sockaddr*) &clientAddr, &len);
                if (hClientSocket == -1)
                {
                        printf("无效socket");
                        continue;
                }

                printf("server: got connection from %s, port %d, socket %d\n", inet_ntoa(clientAddr.sin_addr),
                       ntohs(clientAddr.sin_port), hClientSocket);

                ssl = SSL_new(sslMgr.sslCtx());
                if (NULL == ssl)
                {
                        close(hClientSocket);
                        return 0;
                }

                SSL_set_fd(ssl, hClientSocket);
                if (SSL_accept(ssl) < 0)
                {
                        close(hClientSocket);
                        break;
                }

                char buf[40960] = { 0 };
                int iSize = SSL_read(ssl, buf, 40960);
                printf("接收消息成功 %s\n", buf);

                if (iSize < 0)
                {
                        SSL_shutdown(ssl);
                        SSL_free(ssl);
                        close(hClientSocket);

                        continue;
                }

                cout << "----------------------------------------1" << endl;
                cout << buf << endl;
                cout << "----------------------------------------2" << endl;
                iSize = SSL_write(ssl, buf, iSize);
                if (iSize < 0)
                {
                        SSL_shutdown(ssl);
                        SSL_free(ssl);
                        close(hClientSocket);

                        continue;
                }

//                iSize = SSL_write(ssl, buf, iSize);

//                SSL_shutdown(ssl);
                SSL_free(ssl);
                close(hClientSocket);
        }

        close(hSocket);

        return 0;
}

