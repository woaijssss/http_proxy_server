
#ifndef __UTIL_H__
#define __UTIL_H__

#include <string>
#include<typeinfo>

void exitService(int status);

std::string getHexString(unsigned char* s, const int& len);

#define GET_PARAM_TYPE(__x) typeid(__x).name()


#endif  // __UTIL_H__
