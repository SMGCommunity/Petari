#ifndef MEMORYUTIL_H
#define MEMORYUTIL_H

#include "types.h"

namespace MR
{
    void zeroMemory(void *, u32);
    s32 calcCheckSum(const void *, u32);
};

#endif // MEMORYUTIL_H