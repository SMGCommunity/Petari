#pragma once

#include <revolution/types.h>

namespace nw4r {
    namespace db {
        void Panic(const char*, int, const char*, ...);
    }
}  // namespace nw4r

#ifdef NW4R_DEBUG
#define NW4R_ASSERTMSG_AT(line, exp, ...) ((void)((exp) || (nw4r::db::Panic(__FILE__, line, __VA_ARGS__), 0)))
#else
#define NW4R_ASSERTMSG_AT(line, exp, ...) ((void)0)
#endif

#define NW4R_POINTER_ASSERT_AT(line, exp)                                                                                                            \
    NW4R_ASSERTMSG_AT(line,                                                                                                                          \
                      ((reinterpret_cast< u32 >(reinterpret_cast< const void* >(exp)) & 0xFF000000) == 0x80000000) ||                                \
                          ((reinterpret_cast< u32 >(reinterpret_cast< const void* >(exp)) & 0xFF800000) == 0x81000000) ||                            \
                          ((reinterpret_cast< u32 >(reinterpret_cast< const void* >(exp)) & 0xF8000000) == 0x90000000) ||                            \
                          ((reinterpret_cast< u32 >(reinterpret_cast< const void* >(exp)) & 0xFF000000) == 0xC0000000) ||                            \
                          ((reinterpret_cast< u32 >(reinterpret_cast< const void* >(exp)) & 0xFF800000) == 0xC1000000) ||                            \
                          ((reinterpret_cast< u32 >(reinterpret_cast< const void* >(exp)) & 0xF8000000) == 0xD0000000) ||                            \
                          ((reinterpret_cast< u32 >(reinterpret_cast< const void* >(exp)) & 0xFFFFC000) == 0xE0000000),                              \
                      "NW4R:Pointer Error\n" #exp "(=%p) is not valid pointer.", (exp))

#define NW4R_ASSERT_AT(line, exp) NW4R_ASSERTMSG_AT(line, (exp), "NW4R:Failed assertion " #exp)
#define NW4R_MIN_ASSERT_AT(line, exp, min)                                                                                                           \
    NW4R_ASSERTMSG_AT(line, (exp) >= (min), #exp " is out of bounds(%d)\n%d <= " #exp " not satisfied.", static_cast< int >(exp),                    \
                      static_cast< int >(min))
#define NW4R_ALIGN2_ASSERT_AT(line, exp)                                                                                                             \
    NW4R_ASSERTMSG_AT(line, (reinterpret_cast< u32 >(reinterpret_cast< const void* >(exp)) & 1) == 0,                                                \
                      "NW4R:Alignment Error(0x%x)\n" #exp " must be aligned to 2 bytes boundary.", (exp))

#define NW4R_REFERENCE_ASSERT_AT(line, ref) NW4R_POINTER_ASSERT_AT(line, &ref)
