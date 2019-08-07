
#ifndef __SINGLETON_REGISTER_H__
#define __SINGLETON_REGISTER_H__

/* 单例管理器
 * 1、进程唯一性；
 * 2、构造的单例对象，必须包含Get()全局接口，否则无法实例化
*/

// 可以得到函数 inline XXXX* GetXXXX();的定义
#define GET_SINGLETON_OBJECT_PTR(TypeName)                      \
        inline TypeName* Get##TypeName()                        \
        {                                                       \
                return SingletonRegister::Function<             \
                                        TypeName                \
                                        >::GetInstance();       \
        }

// 可以得到函数 inline XXXX& GetXXXX();的定义
#define GET_SINGLETON_OBJECT(TypeName)                          \
        inline TypeName& Get##TypeName()                        \
        {                                                       \
                return SingletonRegister::Function<             \
                                        TypeName                \
                                        >::GetInstanceObj();    \
        }

/* 单例构造器 */
class SingletonRegister
{
        /* 禁止拷贝构造(单例的唯一性) */
        SingletonRegister(const SingletonRegister&);
        SingletonRegister& operator=(const SingletonRegister&);
public:
        SingletonRegister() {}
        virtual ~SingletonRegister() {}

public:
        /* 单例构造函数，返回要构造的单例类型 */
        template <class Type>
        struct Function
        {
                /* 构造返回指针的构造函数 */
                static Type* GetInstance();

                /* 构造返回单例对象的构造函数 */
                static Type& GetInstanceObj();
        };

};

template<class Type>
Type* SingletonRegister::Function<Type>::GetInstance()
{
        return Type::GetInstance();
}

template<class Type>
Type& SingletonRegister::Function<Type>::GetInstanceObj()
{
        return Type::GetInstance();
}

#endif  // __SINGLETONS_REGISTER_H__
