
#ifndef __IMPL_CONFIG_H__
#define __IMPL_CONFIG_H__

#include <string>

#include "map.h"

/* 配置文件缓存对象 
 * 保存WHPS实例，静态资源路径等信息
 */

/* 配置器抽象接口
 * 配置文件最终都以 K-V形式存储
 * 配置文件支持的格式：
 * （1）格式1，形如：
 *      key1=value1
 *  (2) 格式2，形如：
 *      [whps]
 *      rootDir=/var/file/html
 */
/*Abstract*/
class ImplConfig
{
public:
        /* 重定义保存器别名
         */
        using ConfigType = Map<std::string, std::string>;

        /* 支持带有Section和K-V行类型的配置文件
         * 配置文件格式形如：
         *      [whps]
 *       *      rootDir=/var/file/html
         */
        using ConfigTypeWithSection = Map<std::string, Map<std::string, std::string>>;

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

protected:      // 以下为普通类型配置文件接口
        /* 设置配置 K-V 对
         */
        virtual void set(const std::string& key, const std::string& val) = 0;

        /* 读取指定配置
         */
        virtual const std::string& get(const std::string& key) = 0;

        /* 读取所有配置
         */
        virtual ConfigType& getAllConfig() = 0;

protected:      // 以下为带有Section格式的配置文件接口
        std::string findSection(const std::string& line)
        {
                std::string section;

                if ((line[0] == '[') && (line[line.size()-2]==']'))             // 当前行是section
                {
                        section = line.substr(1, line.size()-3);
                }

                return section;
        }

        virtual void set(const std::string& section, 
                         const std::string& key, 
                         const std::string& val) = 0;

        /* 读取指定配置
         */
        virtual const std::string& get(const std::string& section, 
                                       const std::string& key) = 0;

        /* 读取所有配置
         */
        virtual ConfigTypeWithSection& getAllConfigWithSection() = 0;

protected:
        ConfigTypeWithSection& getSaver()
        {
                return _whpsSaverWithSection;
        }

protected:
        ConfigType _whpsSaver;                          // 普通配置文件保存器
        ConfigTypeWithSection _whpsSaverWithSection;    // Section格式保存器
};

#endif  // __IMPL_CONFIG_H__
