
#ifndef __WHPS_IMPL_MACRO_BASE_H__
#define __WHPS_IMPL_MACRO_BASE_H__

/* 宏定义（入口）
 * 从字符串获取需要的内容
 */
#define Impl_GET_CONTENT(TypeName, RetType)             \
        inline RetType GetContent(const string& sstr)   \
        {                                               \
                return Get##TypeName(sstr);             \
        }

#endif  // __WHPS_IMPL_MACRO_BASE_H__