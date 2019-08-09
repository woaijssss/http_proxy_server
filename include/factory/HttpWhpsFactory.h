
#ifndef __HTTP_WHPS_FACTORY_H__
#define __HTTP_WHPS_FACTORY_H__

#include <memory>

#include "ImplSingleton.h"
#include "SingletonRegister.h"
#include "FactoryBase.h"
#include "HttpWhps.h"
#include "WHPSConfig.h"

#include "HelloWhps.h"  // for function create() test（测试）

#include <iostream>
using namespace std;

/* HttpWhps应用层子类实例化的工厂类（动态对象创建工厂）
 * 该类只会获取一个继承自HttpWhps的子类信息，并将其实例化
 * 返回基类HttpWhps的指针
 * （该文件内容应在编译时自动生成！）
 * 单例模式
 */
class HttpWhpsFactory: public ImplSingleton<HttpWhpsFactory>, public FactoryBase
{
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
        HttpWhps* create()
        {
                // 当前测试，先写死一个子类
                // std::shared_ptr<HelloWhps> ptr(new HelloWhps());

                string whpsName = GetWebSourceConfig().get("whps", "whps-name");    // 获取whps实例的名字
                // 以下添加从实例注册中心获取实例对象

                HttpWhps* ptr = new HelloWhps();
                return ptr;
        }

private:
        static std::shared_ptr<HttpWhpsFactory> _http_whps_factory;
};

GET_SINGLETON_OBJECT_PTR(HttpWhpsFactory)

#endif  // __HTTP_WHPS_FACTORY_H__