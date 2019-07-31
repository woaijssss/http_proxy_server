
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

#include "WHPSLog.h"
#include "String.h"
#include "WHPSStdioBase.h"

using namespace std;

#define MAX_LEN 1024
bool debug_mode = true;

static void WHPSFlushToFile(const char* message);

static std::string MSG(const char* fmt, va_list& ap)
{
        string s_res;
        char* message = new char[1024];       // 1024大小先写死
        time_t timer = time(NULL);
        strftime(message, 23, "[%Y-%m-%d %H:%M:%S] ", localtime(&timer));

        if (fmt)
        {
                vsprintfGet(message+22, fmt, ap);
        }
        else
        {
                sprintf(message+22, "%s", va_arg(ap, const char*));
        }
        s_res = string(message);
        delete[] message;
        return s_res;
}

/* 外部无法实例化 */
class WHPSLog
{
public:
        WHPSLog();
        ~WHPSLog();

public:
        void printToFile(const std::string& msg);

private:
};

static WHPSLog whps_log;

void WHPSLogEventEx(int log_level, const char* fmt, va_list& va)
{
#ifndef __DEBUG__       // 非debug版本
        if (log_level == WHPSLOG_DEBUG)
        {
                return;
        }
#endif

        std::string msg = MSG(fmt, va);
        cout << msg << endl;
        whps_log.printToFile(msg);
}

WHPSLog::WHPSLog()
{

}

WHPSLog::~WHPSLog()
{

}

void WHPSLog::printToFile(const std::string& msg)
{
        WHPSFlushToFile((msg+"\n").c_str());
}

void WHPSFlushToFile(const char* message) 
{
        static bool print_time = true; //是否要打印时间: 当 debug_mode 为真，且上一次是换行符结尾。
        int fd = open("./log/test.log", O_WRONLY | O_CREAT | O_APPEND, S_IRUSR | S_IWUSR);

        if (fd == -1) {
                perror("open (log)");
        } else {
                if (print_time == false) {
                        if (write(fd, message + 22, strlen(message + 22)) == -1)
                        {
                                perror("lprintf");
                        }
                } else {
                        if (write(fd, message, strlen(message)) == -1)
                        {
                                perror("lprintf");
                        }
                }

                print_time = (message[strlen(message) - 1] == '\n');
                close(fd);
        }
}
