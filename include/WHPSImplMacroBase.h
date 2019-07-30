
#ifndef __WHPS_IMPL_MACRO_BASE_H__
#define __WHPS_IMPL_MACRO_BASE_H__

/* 宏定义 */
/*
 * 从字符串获取需要的内容
 */
#define Impl_GET_CONTENT(TypeName, ResType)                     \
        inline ResType GetContent(const std::string& sstr)      \
        {                                                       \
                return Get##TypeName(sstr);                     \
        }

#define Impl_GET_KV_SPLIT(__sep, KvType)                                \
        inline void GetSplit(KvType& st, const std::string& sstr)       \
        {                                                               \
                GetKvSplit(st, sstr, __sep);                            \
        }

#endif  // __WHPS_IMPL_MACRO_BASE_H__