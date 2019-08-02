
#ifndef __WHPS_RESPONSE_WRAPPER_H__
#define __WHPS_RESPONSE_WRAPPER_H__

#include <map>
#include <string>

#include "WhpsWriter.h"

typedef std::map<std::string, std::string>  HttpHeader__;
typedef std::string                         HttpBody__;

#define HttpResponseHeader      HttpHeader__
#define HttpResponseBody        HttpBody__

class WHPSResponseWrapper
{
public:
        WHPSResponseWrapper();
        virtual ~WHPSResponseWrapper();

        /* 多态性，返回基类的对象，供子类使用 */
        WHPSResponseWrapper* _getResponse();

protected:      /* 标准响应头函数*/
        void _setContentLength(const std::string& length);
        void _setContentType(const std::string& type);

//-----------------------------------------------------------------------------------
protected:      /* 以下是添加自定义响应头和时间的函数 */
        /* 该方法是设置有多个值的响应头,参数name表示响应头名称,参数value表示响应头的值 
         */
        void _addHeader(const std::string& h_key, const std::string& h_value);

        /* 该方法是设置有多个值且值的类型为int类型的响应头 
         */
        void _addIntHeader(const std::string& h_key, const int& h_value);

        /* 该方法是设置只有一个值且值的类型为long类型的响应头,例如expies响应头,表示过期时间 
         */
        void _addDateHeader(const std::string& h_key, const long& h_value);

        /* 该方法是设置只有一个值的响应头,参数name表示响应头名称,参数value表示响应头的值 
         */
        void _setHeader(const std::string& h_key, const std::string& h_value);

        /* 该方法是设置只有一个值且值的类型为int类型的响应头,例如Content-Length响应头，
         * 该响应头是代表响应内容有多少字节数 
         */
        void _setIntHeader(const std::string& h_key, const int& h_value);

        /* 该方法是设置只有一个值且值的类型为long类型的响应头,例如expies响应头,表示过期时间 
         */
        void _setDateHeader(const std::string& h_key, const long& h_value);
//-----------------------------------------------------------------------------------

protected:
        std::string _getHeader();
        WhpsWriter& _getWriter();
private:
        /* 初始化响应头的默认参数 */
        void initParams();

        HttpResponseHeader      _header;                    // 响应头(K-V格式)
        std::string             _version/* = "HTTP/1.1"*/;  // 协议版本(应与请求头版本一致)
        std::string             _st_code/* = "200"*/;       // http状态码
        std::string             _status/* = "OK"*/;         // http状态信息
        HttpResponseBody        _body;                      // 响应体

        WhpsWriter              _writer;                    // 发送者   
};

#endif