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
        : m_str(str)
{

}

String::~String()
{

}

const string& String::str() const
{
        return m_str;
}

string String::Str() const
{
        return m_str;
}

String::SVector String::splitOnce(const char* sep)
{
        SVector v;
        size_t pos = 0;
        string s1 = m_str;

        pos = s1.find(sep);
        if (pos != string::npos)
        {
                const string& s = s1.substr(0, pos);
                v.push_back(s);
                s1 = s1.substr(pos + strlen(sep), m_str.size());
                v.push_back(s1);
        }

        return v;
}

String::SVector String::split(const char* sep)
{
        SVector v;
        size_t pos = 0;
        string s1 = m_str;

        while ((pos = s1.find(sep)) != string::npos)
        {
                const string& s = s1.substr(0, pos);
                v.push_back(s);
                s1 = s1.substr(pos + strlen(sep), m_str.size());
        }

        v.push_back(s1);

        return v;
}

void String::strip()
{
        for (size_t i = 0; (i = m_str.find(' ')) != std::string::npos;)
        {
                m_str.replace(i, 1, "");
        }
}

void String::strip(const char* chr)
{
        for (size_t i = 0; (i = m_str.find(chr)) != std::string::npos;)
        {
                m_str.replace(i, 1, "");
        }
}

void String::replace(const string& src, const string& des)
{
        size_t pos = m_str.find(src);

        if (pos != string::npos)
        {
                m_str.replace(pos, src.size(), des);
        }
}

void String::replaceAll(const std::string& src, const std::string& des)
{
        for (size_t i = 0; (i = m_str.find(src)) != std::string::npos;)
        {
                m_str.replace(i, src.size(), des);
        }
}

void String::operator=(const string s)
{
        m_str = s;
}

void String::operator=(const char* s)
{
        m_str = string(s);
}

size_t String::count(const std::string& sub)
{
        size_t size = 0;

        for (size_t i = 0; (i = m_str.find(sub, i)) != std::string::npos; i++)
        {
                size++;
        }

        return size;
}

size_t String::find(const char& c)
{
        return m_str.find(c);
}

size_t String::size()
{
        return m_str.size();
}

bool String::matchCase(const char* c)
{
        return m_str.find(c) != string::npos;
}

String String::substr(const size_t& begin, const size_t& end)
{
        m_str = m_str.substr(begin, end);

        return *this;
}

const string& String::encode(const string& type)
{
        m_str = Codec::encode(m_str, type);

        return m_str;
}

const string& String::decode(const string& type)
{
        m_str = Codec::decode(m_str, type);

        return m_str;
}
