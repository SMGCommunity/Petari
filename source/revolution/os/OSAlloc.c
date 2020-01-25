#include "revolution/os/OSAlloc.h"

s32 OSSetCurrentHeap(s32 heap)
{
    s32 curHeap = __OSCurrHeap;
    __OSCurrHeap = heap;
    return curHeap;
}