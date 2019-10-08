
#ifndef __WHPS_RESPONSE_H__
#define __WHPS_RESPONSE_H__

#include "WHPSResponseWrapper.h"

class WHPSHttpSession;
/* http响应类实体 */
struct HttpResponseContext: public WHPSResponseWrapper
{
public:
        using cbFunc = WHPSResponseWrapper::cbFunc;
private:
        HttpResponseContext& getResponse();

public:
        HttpResponseContext(cbFunc cb);

        virtual ~HttpResponseContext();

public:
        WhpsWriter& getWriter();

public:         /* 标准响应头函数*/
        /* 设置http响应包体长度 */
        void setContentLength(const std::string& length);

        /* 设置http响应内容类型 */
        void setContentType(const std::string& type);

public:
        void setError(const int& sc, const std::string& msg);

//-----------------------------------------------------------------------------------
public:         /* 以下是添加自定义响应头和时间的函数 */
        /* 该方法是设置有多个值的响应头,参数name表示响应头名称,参数value表示响应头的值 
         */
        void addHeader(const std::string& h_key, const std::string& h_value);

        /* 该方法是设置有多个值且值的类型为int类型的响应头 
         */
        void addIntHeader(const std::string& h_key, const int& h_value);

        /* 该方法是设置只有一个值且值的类型为long类型的响应头,例如expies响应头,表示过期时间 
         */
        void addDateHeader(const std::string& h_key, const long& h_value);

        /* 该方法是设置只有一个值的响应头,参数name表示响应头名称,参数value表示响应头的值 
         */
        void setHeader(const std::string& h_key, const std::string& h_value);

        /* 该方法是设置只有一个值且值的类型为int类型的响应头,例如Content-Length响应头，
         * 该响应头是代表响应内容有多少字节数 
         */
        void setIntHeader(const std::string& h_key, const int& h_value);

        /* 该方法是设置只有一个值且值的类型为long类型的响应头,例如expies响应头,表示过期时间 
         */
        void setDateHeader(const std::string& h_key, const long& h_value);
//-----------------------------------------------------------------------------------

public:
        std::string getHeader();
};

#endif  // __WHPS_RESPONSE_H__
