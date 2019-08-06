
#ifndef __WHPS_CONFIG_H__
#define __WHPS_CONFIG_H__

#include <string>

/* 配置文件缓存对象 
 * 保存WHPS实例，静态资源路径等信息
 */
class WHPSConfig
{
public:
        WHPSConfig();

        ~WHPSConfig();

private:
        std::string _whpsName;  // 实例名称
        
};

#endif  // __WHPS_CONFIG_H__