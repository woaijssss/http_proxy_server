
#include <stdlib.h>
#include <sstream>
#include <iomanip>

#include "util.h"

using namespace std;

void exitService(int status)
{
        exit(status);
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