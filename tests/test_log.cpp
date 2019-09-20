
#include "WHPSLog.h"
#include <string>
using namespace std;

#define enum_to_string(x) #x

enum Test
{
        A = 0,
        B,
        C,
        D
};

int main()
{
        string s = "====";
        WHPSLogInfo("%d %d %d", 1, 2, 3);
        WHPSLogInfo("%.2f", 3.1415666);
        WHPSLogInfo("asdsadasdsad");
//        WHPSLogInfo("asdsadasdsad" + s + "111111");



        return 0;
}
