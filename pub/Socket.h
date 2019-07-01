
#ifndef __SOCKET_H__
#define __SOCKET_H__

#include <stdio.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define INVALID_SOCK    -1
#define LISTEN_SIZE     8192

/*
 * 基础socket类，该类实现对linux系统库中，socket基本功能的封装。
 */
class Socket
{
public:
        Socket();
        ~Socket();

        /* 获取当前socket句柄 */
        const int& get() const;

        /* 判断当前socket是否有效 
         * 返回：
         *      true：表示为有效socket
         *      false：表示为无效socket
         */
        bool isValid();
public:
        /* 设置socket属性 */
        int setSocketOpt();

        /* 设置非阻塞socket */
        int setNonblock();

        /* 绑定一个端口
         *      server模式：ip地址无需传递
         *      client模式：ip地址需传指定的服务器ip
         */
        int bind(const int& port, const char* ip = NULL);

        /* 启动监听 */
        int listen();

        /* 获取被监听的连接请求 */
        int accept();   // 暂时不用
        int accept(struct sockaddr_in& c_addr);         // 优先实现这个接口
        struct sockaddr_in accpet();    // 暂时不用

        /* 设置socket地址重用(主要用于服务端socket) */
        int setReuseAddr();

        /* 关闭socket */
        int close();

private:
        int _socket_fd;         // 句柄文件描述符
};

#endif  // __SOCKET_H__