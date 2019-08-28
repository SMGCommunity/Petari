#ifndef MEMORYUTIL_H
#define MEMORYUTIL_H

#include "types.h"
#include "JKR/JKRHeap.h"

namespace MR
{
    JKRHeap* getCurrentHeap();

    void zeroMemory(void *, u32);
    s32 calcCheckSum(const void *, u32);
};

#endif // MEMORYUTIL_H