/*
 * String.cpp
 *
 *  Created on: Oct 10, 2018
 *      Author: wenhan
 */

#include <vector>
#include <iostream>

#include <string.h>

#include "String.h"

using namespace std;

String::String()
{

}

String::String(const string& str)
        : _str(str)
{

}

String::~String()
{

}

const string& String::str() const
{
        return _str;
}

string String::Str() const
{
        return _str;
}

String::SVector String::split(const char* sep)
{
        SVector v;
        size_t pos = 0;
        string s1 = _str;

        while ((pos = s1.find(sep)) != string::npos) {
                const string& s = s1.substr(0, pos);
                v.push_back(s);
                s1 = s1.substr(pos + strlen(sep), _str.size());
        }

        v.push_back(s1);

        return v;
}

string String::strip(const char* chr)
{
        string res = _str;

        for (size_t i = 0; (i = res.find('\n')) != std::string::npos;) {
                res.replace(i, 1, "");
        }

        return res;
}

string String::strip()
{
        string res = _str;

        for (size_t i = 0; (i = res.find(' ')) != std::string::npos;) {
                res.replace(i, 1, "");
        }

        return res;
}

void String::replace(const string& src, const string& des)
{
        size_t pos = _str.find(src);

        if (pos != string::npos) {
                _str.replace(pos, src.size(), des);
        }
}

void String::replaceAll(const std::string& src, const std::string& des)
{
        for (size_t i = 0; (i = _str.find(src)) != std::string::npos;) {
                _str.replace(i, src.size(), des);
        }
}

void String::operator=(const string s)
{
        _str = s;
}

void String::operator=(const char* s)
{
        _str = string(s);
}

size_t String::count(const std::string& sub)
{
        size_t size = 0;

        for (size_t i = 0; (i = _str.find(sub, i)) != std::string::npos; i++) {
                size++;
        }

        return size;
}

size_t String::find(const char& c)
{
        return _str.find(c);
}

