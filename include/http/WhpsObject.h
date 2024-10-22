#ifndef _WHPS_OBJECT_H__
#define _WHPS_OBJECT_H__

#include <typeinfo>
#include <memory>

#include "WhpsObjRegisterBase.h"
#include "HttpWhpsFactory.h"

template<class T>
class WhpsObject: public WhpsObjRegisterBase, public HttpWhps
{
public:
        using HttpPtrType = HttpWhpsFactory::HttpPtrType;
public:
        // static WhpsObjRegisterBase * CreateObject()
        static HttpPtrType CreateObject()
        {
                // return new T();
                HttpPtrType ptr(new T());
                return ptr;
        }

        struct Aux
        {
                Aux()
                {
                        if (!GetHttpWhpsFactory()->regist(typeid(T).name(), CreateObject))
                        {
                                WHPSLogError("regist failed");
                        }
                }

                inline void doNothing() const
                {
                }
        };

        static Aux m_aux;        // 类名注册辅助器

public:
        WhpsObject()
        {

        }

        virtual ~WhpsObject()
        {
                m_aux.doNothing();
        }
};

template<class T>
typename WhpsObject<T>::Aux WhpsObject<T>::m_aux;

#endif  // _WHPS_OBJECT_H__
