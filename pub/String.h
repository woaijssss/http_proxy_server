/*
 * String.h
 *
 *  Created on: Oct 10, 2018
 *      Author: wenhan
 */

#ifndef STRING_H_
#define STRING_H_

#include <vector>
#include <string>
#include <iostream>

class String: public std::string {
public:
        typedef std::vector<std::string> SVector;

        String();
        String(const std::string& str);

        ~String();

        /* 获取字符串 */
        const std::string& str() const;
        std::string Str() const;

        /* 按照sep拆分字符串 */
        SVector split(const char* sep);

        /* 过滤字符串中，带有chr的字符 */
        std::string strip(const char* chr);

        /* 使用des替换src，仅匹配一次 */
        void replace(const std::string& src, const std::string& des);

        /* 使用des替换src，匹配所有 */
        void replaceAll(const std::string& src, const std::string& des);

        /* 运算符重载，拷贝构造 string类型 */
        void operator=(const std::string s);

        /* 运算符重载，拷贝构造 char[]类型*/
        void operator=(const char* s);

        /* 统计子字符串出现的次数 */
        size_t count(const std::string& sub);

        size_t find(const char& c);
private:
        std::string _str;
};

#endif /* STRING_H_ */
