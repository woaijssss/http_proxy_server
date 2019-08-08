
#ifndef __WEB_SOURCE_CONFIG_H__
#define __WEB_SOURCE_CONFIG_H__

#include <memory>

#include "ImplSingleton.h"
#include "SingletonRegister.h"
#include "ImplConfig.h"

class WebSourceConfig: public ImplSingleton<WebSourceConfig>, public ImplConfig
{
public:
        using ConfigType = ImplConfig::ConfigType;
        using ConfigTypeWithSection = ImplConfig::ConfigTypeWithSection;
public:
        static WebSourceConfig& GetInstance();

        WebSourceConfig();

        virtual ~WebSourceConfig();

public:
        virtual bool readConfig();

public:
        virtual void set(const std::string& section, 
                         const std::string& key, 
                         const std::string& val);

        /* 读取指定配置
         */
        virtual const std::string& get(const std::string& section, 
                                       const std::string& key);

        /* 读取所有配置
         */
        virtual ConfigTypeWithSection& getAllConfigWithSection();

        void print();

private:
        /* 设置配置 K-V 对
         */
        virtual void set(const std::string& key, const std::string& val);

        /* 读取指定配置
         */
        virtual const std::string& get(const std::string& key);

        /* 读取所有配置
         */
        virtual ConfigType& getAllConfig();

private:
        static std::shared_ptr<WebSourceConfig> _webSourceConfig;       // 单例对象
        std::string _webConfigPath/* = "./conf/web.conf"*/;     // 配置文件名（不可以外部指定）
};

GET_SINGLETON_OBJECT(WebSourceConfig)

#endif  // __WEB_SOURCE_CONFIG_H__