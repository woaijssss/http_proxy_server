
#ifndef __WHPS_LOG_H__
#define __WHPS_LOG_H__

#include <stdarg.h>
#include <string>

enum LogLevel
{
        WHPSLOG_TRACE = 0,      // 基本不需要这个级别
        WHPSLOG_DEBUG,
        WHPSLOG_INFO,
        WHPSLOG_WARN,
        WHPSLOG_ERROR,
        WHPSLOG_CRITICAL,
        WHPSLOG_FATAL
};

void WHPSLogEventEx(int log_level, const char* fmt, va_list& va);

/* 抽象日志事件接口
 * 通过该接口，可以定义多个日志级别对应的函数
 */
#define IMPL_WHPSLogEvent_(log_level)                                   \
        {                                                               \
                        va_list va;                                     \
                        va_start(va, fmt);                              \
                        WHPSLogEventEx(log_level, fmt.c_str(), va);     \
                        va_end(va);                                     \
        }

#define WHPSLogEvent(log_level) IMPL_WHPSLogEvent_(log_level)


/* 日志记录函数的入口方法集 */
void WHPSLogDebug(const std::string& fmt, ...);
void WHPSLogInfo(const std::string& fmt, ...);
void WHPSLogWarn(const std::string& fmt, ...);
void WHPSLogError(const std::string& fmt, ...);
void WHPSLogCritical(const std::string& fmt, ...);
void WHPSLogFatal(const std::string& fmt, ...);

#endif  // __WHPS_LOG_H__
