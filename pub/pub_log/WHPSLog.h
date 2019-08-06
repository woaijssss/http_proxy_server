
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
        inline void WHPSLogEvent_##log_level(const char* fmt, ...)      \
        {                                                               \
                va_list va;                                             \
                va_start(va, fmt);                                      \
                WHPSLogEventEx(log_level, fmt, va);                     \
                va_end(va);                                             \
        }

IMPL_WHPSLogEvent_(WHPSLOG_DEBUG)
IMPL_WHPSLogEvent_(WHPSLOG_INFO)
IMPL_WHPSLogEvent_(WHPSLOG_WARN)
IMPL_WHPSLogEvent_(WHPSLOG_ERROR)
IMPL_WHPSLogEvent_(WHPSLOG_CRITICAL)
IMPL_WHPSLogEvent_(WHPSLOG_FATAL)

#define WHPSLogEvent(log_level, fmt, ...)    WHPSLogEvent_##log_level(fmt, __VA_ARGS__);

/* 日志记录函数的入口方法 */
#ifdef __DEBUG__
        #define WHPSLogDebug(fmt, ...)       WHPSLogEvent(WHPSLOG_DEBUG, fmt, __VA_ARGS__)
#else
        #define WHPSLogDebug(fmt, ...)       ((void)0)       // release版本不会打印Debug类型日志
#endif

#define WHPSLogInfo(fmt, ...)        WHPSLogEvent(WHPSLOG_INFO, fmt, __VA_ARGS__)
#define WHPSLogWarn(fmt, ...)        WHPSLogEvent(WHPSLOG_WARN, fmt, __VA_ARGS__)
#define WHPSLogError(fmt, ...)       WHPSLogEvent(WHPSLOG_ERROR, fmt, __VA_ARGS__)
#define WHPSLogCritical(fmt, ...)       WHPSLogEvent(WHPSLOG_CRITICAL, fmt, __VA_ARGS__)
#define WHPSLogFatal(fmt, ...)       WHPSLogEvent(WHPSLOG_FATAL, fmt, __VA_ARGS__)

#endif  // __WHPS_LOG_H__