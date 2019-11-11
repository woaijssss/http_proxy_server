#include "HttpWhpsFactory.h"

#ifdef __GNUC__
#include <cxxabi.h>
#endif
#include <stdlib.h>

std::shared_ptr<HttpWhpsFactory> HttpWhpsFactory::m_httpWhpsFactory;

HttpWhpsFactory::HttpPtrType HttpWhpsFactory::get(const std::string& type)
{
        return m_mapPtr[type];
}

bool HttpWhpsFactory::create(const std::string& type_name)
{
        HttpPtrType obj = _create(type_name);

        if (!obj)
        {
                return false;
        }

        WHPSLogInfo("HttpWhpsFactory::create: " + type_name);
        m_mapPtr[type_name] = obj;

        return true;
}

HttpWhpsFactory::HttpPtrType HttpWhpsFactory::_create(const std::string & type_name)
{
        if (type_name.empty())
        {
                return NULL;
        }

        std::map<std::string, CreateFunction>::iterator it = m_createFunctionMap.find(type_name);

        if (it == m_createFunctionMap.end())
        {
                return NULL;
        }

        return it->second();
}

// 解析类型名称（转换为 A::B::C 的形式）
std::string HttpWhpsFactory::readTypeName(const char* name)
{
        char* real_name = abi::__cxa_demangle(name, nullptr, nullptr, nullptr);
        std::string real_name_string(real_name);
        ::free(real_name);

        return real_name_string;
}

bool HttpWhpsFactory::regist(const char * name, CreateFunction func)
{
        if (!func)
        {
                WHPSLogError("HttpWhpsFactory::regist false");

                return false;
        }

        std::string type_name = readTypeName(name);
        m_createFunctionMap.insert(type_name, func);

        return this->create(type_name);
}
