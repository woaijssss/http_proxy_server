
#ifndef __WHPS_LOG_H__
#define __WHPS_LOG_H__

enum LogLevel
{
        TRACE = 0,
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL
};

class WHPSLog
{
public:
        WHPSLog();
        ~WHPSLog();

public:
        void printToFile(const char* fmt, ...);

private:
};

#endif  // __WHPS_LOG_H__