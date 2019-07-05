
#ifndef __UTIL_H__
#define __UTIL_H__

#include <string>

void exitService(int status);

std::string getHexString(unsigned char* s, const int& len);

#endif  // __UTIL_H__