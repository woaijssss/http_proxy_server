
#ifndef __WHPS_HTTP_SESSION_H__
#define __WHPS_HTTP_SESSION_H__

#include <string>

#include "WHPSTcpSession.h"
#include "WHPSHttpParser.h"
#include "HttpWhpsFactory.h"
#include "HttpWhps.h"   // for aplication layer call
#include "HttpWriterRegistser.h"

/* http session：单个http连接，基于tcp session的tcp连接，二者为强依赖关系
 * HttpSession的实例化，必须依赖于TcpSession；
 * 
 * （1）应用层的数据处理，入口应是：
 *      WHPSHttpSession::onHttpRecv()
 */
class WHPSHttpSession
{
public:
        using sp_TcpSession = WHPSTcpSession::sp_TcpSession;
        using sp_HttpSession = std::shared_ptr<WHPSHttpSession>;
        // using HttpSessionCB = std::function<void(const sp_HttpSession&)>;
        using HttpSessionCB = std::function<void(const sp_TcpSession&)>;
        using WriterFunc = HttpWriterRegistser::cbFunc;
public:
        /* http session的实例化，必须依赖于tcp session，是一一对应的关系 */
        WHPSHttpSession(const sp_TcpSession& tcp_session);

        ~WHPSHttpSession();

public:
        /* 获取当前http连接对应的tcp连接 */
        const sp_TcpSession& getTcpSession() const;

        /* 设置http连接回调函数，提供HttpServer当前释放的http session */
        void setHttpCloseCallback(HttpSessionCB cb);

private:
        // 统一格式类型，参数全部采用tcp session
        /* http接收消息回调 */
        void onHttpMessage(const sp_TcpSession& tcp_session);

        /* http发送消息回调 */
        void onHttpSend(const sp_TcpSession& tcp_session);

        /* http连接关闭回调 */
        void onHttpClose(const sp_TcpSession& tcp_session);

        /* http异常错误回调 */
        void onHttpError(const sp_TcpSession& tcp_session);

private:
        /* http层根据Connection字段值，来判断是否通知tcp层
         * 发送数据后断开连接，作通知功能。
         */
        void notifyToClose(const sp_TcpSession& tcp_session);

private:        // writer
        /* 客户端回写接口
         */
        void sendHttpMessage(const std::string& msg);

private:
        // 先使用这种方式测试http功能，后续引入工作线程池后，可以将发送消息的任务，加入到工作线程队列里
        const sp_TcpSession _tcp_session;      // tcp连接对象
        HttpSessionCB _http_closeCB;            // http连接断开回调函数

        WHPSHttpParser _http_parser;            // http解析器

private:
        HttpWhpsFactory* _http_whps_factory;    // 应用层实例化工厂类对象
        HttpWhps* _http_whps;                   // 应用层（业务层）调用句柄

        WriterFunc _writer_func;
        HttpWriterRegistser _writer;   // 数据发送注册器
};

#endif  // __WHPS_HTTP_SESSION_H__
