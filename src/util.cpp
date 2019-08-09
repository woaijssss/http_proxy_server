
#include <stdlib.h>
#include <sstream>
#include <iomanip>

#include "util.h"
#include "WHPSConfig.h"

using namespace std;

void exitService(int status)
{
        exit(status);
}

void delayMs(const unsigned int& ms)
{
        const timespec ts = {0, ms};
        nanosleep(&ts, NULL);        // 线程级别的睡眠（毫秒级）
}

string getHexString(unsigned char* s, const int& len)
{
        std::ostringstream out;
        out << std::hex << std::setw(2) << std::setfill('0');

        for (int i = 0; i < len; i++) {
                out << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(s[i]) << " ";
        }

        return out.str();
}

void initConfig()
{
        if (!GetWebSourceConfig().readConfig())
        {
                cout << "Config file load failed..." << endl;
                exit(-1);
        }

        GetWebSourceConfig().print();
}