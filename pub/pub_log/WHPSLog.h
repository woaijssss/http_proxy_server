
#ifndef __WHPS_LOG_H__
#define __WHPS_LOG_H__

#include <stdarg.h>

enum LogLevel
{
        WHPSLOG_TRACE = 0,      // 基本不需要这个等级
        WHPSLOG_DEBUG,
        WHPSLOG_INFO,
        WHPSLOG_WARN,
        WHPSLOG_ERROR,
        WHPSLOG_FATAL
};

void WHPSLogEventEx(int log_level, const char* fmt, va_list& va);

#define IMPL_WHPSLogEvent_(log_level)                                   \
        inline void WHPSLogEvent_##log_level(const char* fmt, ...)      \
        {                                                               \
                va_list va;                                             \
                va_start(va, fmt);                                      \
                WHPSLogEventEx(log_level, fmt, va);                     \
                va_end(va);                                             \
        }

IMPL_WHPSLogEvent_(WHPSLOG_INFO)

#define WHPSLogEvent(log_level, fmt, __s)    WHPSLogEvent_##log_level(fmt, __s);

/* 日志记录函数的入口方法 */
#ifdef __DEBUG__
        #define WHPSLogDebug(fmt, __s)       WHPSLogEvent(WHPSLOG_DEBUG, fmt, __s)
#else
        #define WHPSLogDebug(fmt, __s)       ((void)0)       // release版本不会打印Debug类型日志
#endif

#define WHPSLogInfo(fmt, __s)        WHPSLogEvent(WHPSLOG_INFO, fmt, __s)
#define WHPSLogWarn(fmt, __s)        WHPSLogEvent(WHPSLOG_WARN, fmt, __s)
#define WHPSLogError(fmt, __s)       WHPSLogEvent(WHPSLOG_ERROR, fmt, __s)
#define WHPSLogFatal(fmt, __s)       WHPSLogEvent(WHPSLOG_FATAL, fmt, __s)

#endif  // __WHPS_LOG_H__