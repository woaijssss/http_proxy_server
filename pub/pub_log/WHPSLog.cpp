#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

#include "WHPSLog.h"
#include "String.h"
//#include "WHPSConfig.h"
#include "WHPSStdioBase.h"

using namespace std;

#define MAX_LEN 1024
bool debug_mode = true;
static vector<const char*> v_level = { "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "CRITICAL", "FATAL" };

void WHPSLogDebug(const std::string& fmt, ...)
{
        WHPSLogEvent(WHPSLOG_DEBUG)
}

void WHPSLogInfo(const std::string& fmt, ...)
{
        WHPSLogEvent(WHPSLOG_INFO)
}

void WHPSLogWarn(const std::string& fmt, ...)
{
        WHPSLogEvent(WHPSLOG_WARN)
}

void WHPSLogError(const std::string& fmt, ...)
{
        WHPSLogEvent(WHPSLOG_ERROR)
}

void WHPSLogCritical(const std::string& fmt, ...)
{
        WHPSLogEvent(WHPSLOG_CRITICAL)
}

void WHPSLogFatal(const std::string& fmt, ...)
{
        WHPSLogEvent(WHPSLOG_FATAL)
}
static std::string MSG(int log_level, const char* fmt, va_list& ap)
{
        string s_res;
//        char* message = new char[1024];       // 1024大小先写死
        char message[1024] = { 0 };
        time_t timer = time(NULL);
        strftime(message, 23, "[%Y-%m-%d %H:%M:%S] ", localtime(&timer));
        int size = 22 + sprintf(message + 22, "%s: ", v_level[log_level]);

        if (fmt)
        {
                vsprintfGet(message + size, fmt, ap);
        } else
        {
                sprintf(message + size, "%s", va_arg(ap, const char*));
        }

        s_res = string(message);
//        delete[] message;

        return s_res;
}

class FileFlusher
{
public:
        void WHPSFlushToFile(const char* message);
};
/* 外部无法实例化 */
class WHPSLog
{
public:
        WHPSLog();
        ~WHPSLog();

public:
        void printToFile(const std::string& msg);

        const std::string& getLogPath();

private:
        /* 日志文件路径不允许外部配置，打印日志的路径固定 */
        std::string _log_path;
        FileFlusher _flusher;
};

static WHPSLog logger;

void WHPSLogEventEx(int log_level, const char* fmt, va_list& va)
{
#ifndef __DEBUG__       // 非debug版本，直接返回
        if (log_level == WHPSLOG_DEBUG) // release版本不会打印Debug类型日志
        {
                return;
        }
#endif

        std::string msg = MSG(log_level, fmt, va);
        logger.printToFile(msg);
}

WHPSLog::WHPSLog()
{

}

WHPSLog::~WHPSLog()
{

}

const std::string& WHPSLog::getLogPath()
{
        return _log_path;
}

void WHPSLog::printToFile(const std::string& msg)
{
        _flusher.WHPSFlushToFile((msg + "\n").c_str());
}

void FileFlusher::WHPSFlushToFile(const char* message)
{
        static bool is_print_time = true; //是否要打印时间: 当 debug_mode 为真，且上一次是换行符结尾。
//        const string& log_path = GetWebSourceConfig().get("Server", "logPath");
//        string log_path = "./log/server.log";
        string log_path;
        int fd = -1;

        if (log_path.empty())   // 空文件，打印到终端
        {
                fd = stdin->_fileno;
        } else
        {
                fd = open(log_path.c_str(), O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);
        }

        if (fd == -1)
        {
                perror("open (log)");
        } else
        {
                if (is_print_time == false)
                {
                        if (write(fd, message + 22, strlen(message + 22)) == -1)
                        {
                                perror("lprintf");
                        }
                } else
                {
                        if (write(fd, message, strlen(message)) == -1)
                        {
                                perror("lprintf");
                        }
                }

                is_print_time = (message[strlen(message) - 1] == '\n');

                if (fd > stdout->_fileno)
                {
                        close(fd);
                }
        }
}
