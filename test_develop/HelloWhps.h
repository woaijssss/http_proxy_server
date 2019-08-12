
#ifndef __HELLO_WHPS_H__
#define __HELLO_WHPS_H__

#include "WhpsObject.h"

/* 应用接口测试（子类whps实例），使用注意如下：
 * （1）必须继承WhpsObject类
 * （2）必须提供当前类的类型给父类
 * （3）必须实现doGet和doPost方法（至少包含函数体，空的也行）
 * （4）析构函数必须为virtual
 * 不遵循以上规则，编译将会报错
 */
class HelloWhps: public WhpsObject<HelloWhps>
{
public:
        HelloWhps();
        virtual ~HelloWhps();

public:
        virtual void doGet(HttpWhpsRequest request, HttpWhpsResponse response);

        virtual void doPost(HttpWhpsRequest request, HttpWhpsResponse response);

private:
};

#endif  //