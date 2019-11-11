#ifndef __WHPS_OBJ_REGISTER_BASE_H__
#define __WHPS_OBJ_REGISTER_BASE_H__

#include "ImplRegisterBase.h"

/* whps子类注册函数
 * 编译期注入子类类型，动态创建子类对象
 */
class WhpsObjRegisterBase: public ImplRegisterBase
{
public:
        using cbFunc = ImplRegisterBase::cbFunc;
public:
        WhpsObjRegisterBase();

        virtual ~WhpsObjRegisterBase();

        virtual void registObj(cbFunc cb);

        virtual cbFunc getRegistObj();
};

#endif  // __WHPS_OBJ_REGISTER_BASE_H__
