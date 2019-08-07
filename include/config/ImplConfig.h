
#ifndef __IMPL_CONFIG_H__
#define __IMPL_CONFIG_H__

#include <string>

#include "map.h"

/* 配置文件缓存对象 
 * 保存WHPS实例，静态资源路径等信息
 */

/* 配置器抽象接口
 * 配置文件最终都以 K-V形式存储
 */
/*Abstract*/
class ImplConfig
{
public:
        /* 重定义保存器别名
         */
        using ConfigType = Map<std::string, std::string>;

        virtual ~ImplConfig()
        {

        }

protected:
        ImplConfig()
        {

        }

        /* 配置文件读取接口
         */
        virtual bool readConfig() = 0;

        /* 设置配置 K-V 对
         */
        virtual void set(const std::string& key, const std::string& val) = 0;

        /* 读取指定配置
         */
        virtual const std::string& get(const std::string& key) = 0;

        /* 读取所有配置
         */
        virtual ConfigType& getAllConfig() = 0;

protected:
        ConfigType _whpsSaver;     // 配置文件保存器
};

#endif  // __IMPL_CONFIG_H__