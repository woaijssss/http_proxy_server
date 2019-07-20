
#ifndef __WHPS_HTTP_SESSION_H__
#define __WHPS_HTTP_SESSION_H__

#include <string>

#include "WHPSTcpSession.h"

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
        using HttpSessionCB = std::function<void(const sp_HttpSession&)> ;
public:
        /* http session的实例化，必须依赖于tcp session，是一一对应的关系 */
        WHPSHttpSession(const sp_TcpSession& tcp_session);

        ~WHPSHttpSession();

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
        // 先使用这种方式测试http功能，后续引入工作线程池后，可以将发送消息的任务，加入到工作线程队列里
        const sp_TcpSession& _tcp_session;      // tcp连接对象
};

#endif  // __WHPS_HTTP_SESSION_H__