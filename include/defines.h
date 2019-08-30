#ifndef DEFINES_H
#define DEFINES_H

#include "types.h"

static inline s32 __cntlzw(s32 val)
{
    s32 ret;
    __asm("cntlzw %0, %1" : "=r"(ret) : "r"(val));
    return ret;
}

static inline f32 frsp(f32 src)
{
    f32 ret;
    __asm("frsp %0, %1" : "=f"(ret) : "f"(src));
    return ret;
}

#endif // DEFINES_H