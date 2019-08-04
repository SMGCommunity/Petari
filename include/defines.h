#ifndef DEFINES_H
#define DEFINES_H

#include "types.h"

static inline s32 __cntlzw(s32 val)
{
    s32 res;
    __asm("cntlzw %0, %1" : "=r"(res) : "r"(val));
    return res;
}

#endif // DEFINES_H