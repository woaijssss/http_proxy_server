#ifndef __WEB_SOURCE_CONFIG_H__
#define __WEB_SOURCE_CONFIG_H__

#include <memory>

#include "ImplSingleton.h"
#include "SingletonRegister.h"
#include "ImplConfig.h"
#include "WHPSLog.h"

/* 配置文件保存器
 * 支持 section 配置模式
 * 单例模式
 */
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
        ConfigTypeWithSection& get()
        {
                return getSaver();
        }

public:
        /* 配置器初始化，初始化各项配置参数，防止使用出现空指针 */
        void init();

        /* 读取配置文件 */
        virtual bool readConfig();

public:
        /* section模式设置保存一项配置 */
        virtual void set(const std::string& section, const std::string& key, const std::string& val);

        /* 读取指定配置 */
        virtual const std::string& get(const std::string& section, const std::string& key);

        /* 读取所有配置 */
        virtual ConfigTypeWithSection& getAllConfigWithSection();

        /* 打印所有配置信息 */
        void print();

private:
        /* 设置配置 K-V 对 */
        virtual void set(const std::string& key, const std::string& val);

        /* 读取指定配置 */
        virtual const std::string& get(const std::string& key);

        /* 读取所有配置 */
        virtual ConfigType& getAllConfig();

private:
        static std::shared_ptr<WebSourceConfig> m_webSourceConfig;       // 单例对象
        std::string m_webConfigPath/* = "./conf/web.conf"*/;     // 配置文件名（不可以外部指定）
};

GET_SINGLETON_OBJECT(WebSourceConfig)

#endif  // __WEB_SOURCE_CONFIG_H__
