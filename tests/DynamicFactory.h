
#ifndef __DYNAMIC_FACTORY_H__
#define __DYNAMIC_FACTORY_H__
 
#ifdef __GNUC__
#include <cxxabi.h>
#endif
 
#include <string.h>
#include <stdlib.h>
#include <map>
#include <string>
#include <typeinfo>
#include <memory>
#include <functional>

#include <iostream>
using namespace std;
 
// 动态对象基类
class WhpsObjRegisterBase
{
public:
        WhpsObjRegisterBase() {}
        virtual ~WhpsObjRegisterBase() {}
};
 
class HttpWhps;
// 动态对象创建工厂
class HttpWhpsFactory
{
public:
        // typedef std::shared_ptr<HttpWhps> CreateFunction();
        using CreateFunction = std::function<std::shared_ptr<HttpWhps>()>;
public:
        HttpWhpsFactory()
        {

        }
 
        static HttpWhpsFactory& Instance()
        {
                static HttpWhpsFactory fac;
                return fac;
        }

        inline std::shared_ptr<HttpWhps> get(const std::string& type)
        {
                return _map_ptr[type];
        }
public:
        bool Create(const std::string& type_name)
        {
                std::shared_ptr<HttpWhps> obj = _Create(type_name);
                if (!obj)
                {
                        return false;
                }

                // objType * real_obj = dynamic_cast<objType*>(obj);
                // if (!real_obj)
                // {
                //         delete obj;
                //         return NULL;
                // }

                cout << type_name << endl;
                _map_ptr[type_name] = obj;

                return true;
        }

        std::shared_ptr<HttpWhps> _Create(const std::string & type_name)
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
        static std::string readTypeName(const char * name)
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
                        cout << "false" << endl;
                        return false;
                }
                std::string type_name = readTypeName(name);
                _create_function_map.insert(std::make_pair(type_name, func)).second;

                return this->Create(type_name);
        }
public:
        std::map<std::string, CreateFunction> _create_function_map;
        std::map<std::string, std::shared_ptr<HttpWhps>> _map_ptr;
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
        // static WhpsObjRegisterBase * CreateObject()
        static std::shared_ptr<HttpWhps> CreateObject()
        {
                // return new T();
                std::shared_ptr<HttpWhps> ptr(new T());
                return ptr;
        }
 
        struct Aux
        {
                Aux()
                {
                        if (!HttpWhpsFactory::Instance().Regist(typeid(T).name(), CreateObject))
                        {
                                cout << "regist failed" << endl;
                        }
                }
 
                inline void do_nothing()const { }
        };
 
        static Aux _aux;
 
public:
        WhpsObject()
        {
                // _Aux.do_nothing();
        }
 
        virtual ~WhpsObject()
        {
                _aux.do_nothing();
        }
};
 
template <typename T>
typename WhpsObject<T>::Aux WhpsObject<T>::_aux;
 
#endif
