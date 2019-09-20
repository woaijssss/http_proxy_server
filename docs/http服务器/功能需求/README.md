# Web Http Proxy Server(WHPS)
c++版的http代理服务器

## Introduction
- c++做web开发较少，其主要原因还是在现存的工具不足以支撑，导致在web服务器开发中，c++开发者对于应用程序的开发工作量、难度都很大，从底层通讯到上层交互全部都需要重新实现，成本高。
- 本项目意在解决上述开发成本问题，避免开发者维护底层通讯，仅需进行少量配置，即可实现任意http协议层配置。

## Build && Installation
```
Debug:
	make
Releae:
	make BUILD=release
```

## Technology Description
- 基于epoll的I/O多路复用机制，socket采用非阻塞式模式，实现Reactor模式通讯，epoll使用ET模式；
- 由于epoll使用了ET模式，因此读、写操作都必须循环执行，直到读(写)完为止，相比LT高效；
- 参考RTII思想，实现"反射"机制，可动态识别whps对象类型；
- 引入智能指针管理单例、tcp和http session等对象，实现高效的资源管理；
- http静态资源由系统返回，应用开发者无需考虑静态资源请求问题，只需配置静态资源的根路径即可；
- 主线程完全负责接收连接和创建tcpsession实例对象，子线程池负责所有的I/O交互逻辑，通过哈希实现线程分配的负载均衡；
- 注册器：支持注册回写接口、注册回调函数等；
- http响应类中，支持创建响应发送器，方便应用开发者调用；
- tcp入口、http入口和配置管理入口，都设置为单例对象；
- 最小堆代替时间轮实现Timer定时器，由TimerManager管理定时器组
- 设计模式：
```
（1）单例模式：
	管理 TcpServer、HttpServer和WebSourceConfig
（2）工厂模式：
	结合RTII思想，管理并创建 HttpWhps 的子类
```




















