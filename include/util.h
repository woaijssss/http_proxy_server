
#ifndef __UTIL_H__
#define __UTIL_H__

#include <string>
#include<typeinfo>

void exitService(int status);

/* 毫秒级延迟 
 */
void delayMs(const unsigned int& ms);

/* string字符型转成十六进制，用于打印原始数据
 */
std::string getHexString(unsigned char* s, const int& len);

/* 初始化所有配置文件
 */
void initConfig();

#define GET_PARAM_TYPE(__x) typeid(__x).name()


#endif  // __UTIL_H__
