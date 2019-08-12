
#ifndef __HTTP_WHPS_FACTORY_H__
#define __HTTP_WHPS_FACTORY_H__

#include <memory>

#include "ImplSingleton.h"
#include "SingletonRegister.h"
#include "FactoryBase.h"
#include "HttpWhps.h"
#include "WHPSConfig.h"

#include <iostream>
using namespace std;

/* HttpWhps应用层子类实例化的工厂类（动态对象创建工厂）
 * 该类只会获取一个继承自HttpWhps的子类信息，并将其实例化
 * 返回基类HttpWhps的指针
 * （该文件内容应在编译时自动生成！）
 * 单例模式
 */
class HttpWhps;
class HttpWhpsFactory: public ImplSingleton<HttpWhpsFactory>, public FactoryBase
{
public:
        using CreateFunction = std::function<std::shared_ptr<HttpWhps>()>;
        using HttpPtrType = std::shared_ptr<HttpWhps>;
public:
        static HttpWhpsFactory* GetInstance()
        {
                if (!_http_whps_factory.get())
                {
                        _http_whps_factory = std::shared_ptr<HttpWhpsFactory>(new HttpWhpsFactory());
                }

                return _http_whps_factory.get();
        }
public:
        HttpWhpsFactory()
                : ImplSingleton<HttpWhpsFactory>()
                , FactoryBase()
        {

        }

        virtual ~HttpWhpsFactory()
        {

        }

public:
        /* 创建应用层对象
         * （1）获取所有继承自HttpWhps类的子类；
         * （2）所有子类中，只实例化最新的一个；
         */
        // HttpWhps* create()
        // {
        //         // 当前测试，先写死一个子类
        //         // std::shared_ptr<HelloWhps> ptr(new HelloWhps());

        //         string whpsName = GetWebSourceConfig().get("whps", "whps-name");    // 获取whps实例的名字
        //         // 以下添加从实例注册中心获取实例对象

        //         HttpWhps* ptr = new HelloWhps();
        //         return ptr;
        // }

public:
        HttpPtrType get(const std::string& type);
public:
        bool create(const std::string& type_name);

        HttpPtrType _create(const std::string & type_name);
 
        // 解析类型名称（转换为 A::B::C 的形式）
        static std::string readTypeName(const char* name);
 
        bool regist(const char * name, CreateFunction func);

private:
        static std::shared_ptr<HttpWhpsFactory> _http_whps_factory;

        Map<std::string, CreateFunction> _create_function_map;     // 对象名和创建函数的对应关系
        Map<std::string, HttpPtrType> _map_ptr;                    // 对象名和指针句柄的对应关系
};

GET_SINGLETON_OBJECT_PTR(HttpWhpsFactory)

#endif  // __HTTP_WHPS_FACTORY_H__