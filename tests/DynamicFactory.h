
#ifndef __DYNAMIC_FACTORY_H__
#define __DYNAMIC_FACTORY_H__
 
#ifdef __GNUC__
#include <cxxabi.h>
#endif
 
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <map>
#include <string>
#include <typeinfo>

#include <iostream>
using namespace std;
 
// 动态对象基类
class WhpsObjRegisterBase
{
public:
        WhpsObjRegisterBase() {}
        virtual ~WhpsObjRegisterBase() {}
};
 
// 动态对象创建工厂
class HttpWhpsFactory
{
public:
        typedef WhpsObjRegisterBase* (*CreateFunction)();
public:
        HttpWhpsFactory()
        {

        }
 
        static HttpWhpsFactory& Instance()
        {
                static HttpWhpsFactory fac;
                return fac;
        }
public:
        template<class objType>
        objType * Create(const std::string& type_name)
        {
                WhpsObjRegisterBase * obj = _Create(type_name);
                if (!obj)
                {
                        return NULL;
                }
                objType * real_obj = dynamic_cast<objType*>(obj);
                if (!real_obj)
                {
                        delete obj;
                        return NULL;
                }
                return real_obj;
        }

        WhpsObjRegisterBase * _Create(const std::string & type_name)
        {
                if (type_name.empty())
                {
                        return NULL;
                }

                std::map<std::string, CreateFunction>::iterator it = _create_function_map.find(type_name);
                if (it == _create_function_map.end())
                {
                        return NULL;
                }

                return it->second();
        }
 
        // 解析类型名称（转换为 A::B::C 的形式）
        static std::string ReadTypeName(const char * name)
        {
                char * real_name = abi::__cxa_demangle(name, nullptr, nullptr, nullptr);
                std::string real_name_string(real_name);
                free(real_name);
                return real_name_string;
        }
 
        bool Regist(const char * name, CreateFunction func)
        {
                if (!func)
                {
                        return false;
                }
                std::string type_name = ReadTypeName(name);
                return _create_function_map.insert(std::make_pair(type_name, func)).second;
        }
public:
 
        std::map<std::string, CreateFunction> _create_function_map;
};

class HttpWhps
{
public:
        HttpWhps() {}
        virtual ~HttpWhps() {}

public:
        virtual void doGet() {}
};
 
// 动态对象创建器
template<typename T>
class WhpsObject : public WhpsObjRegisterBase, public HttpWhps
{
public:
        static WhpsObjRegisterBase * CreateObject()
        {
                return new T();
        }
 
        struct Registor
        {
                Registor()
                {
                        if (!HttpWhpsFactory<T>::Instance().Regist(typeid(T).name(), CreateObject))
                        {
                                // assert(false);
                                string type = "Basic1";
                                HttpWhpsFactory<T>::Instance().Create(type);
                        }
                }
 
                inline void do_nothing()const { }
        };
 
        static Registor _registor;
 
public:
        WhpsObject()
        {
                // _registor.do_nothing();
        }
 
        virtual ~WhpsObject()
        {
                _registor.do_nothing();
        }
};
 
template <typename T>
typename WhpsObject<T>::Registor WhpsObject<T>::_registor;
 
#endif