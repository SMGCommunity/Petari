#ifndef MEMORYUTIL_H
#define MEMORYUTIL_H

#include "types.h"
#include "JKernel/JKRHeap.h"
#include "OS/OSMutex.h"

namespace MR
{
    template<s32 T>
    class MutexHolder
    {
    public:
        static OSMutex sMutex;
    };

    class CurrentHeapRestorer
    {
    public:
        CurrentHeapRestorer(JKRHeap *);
        ~CurrentHeapRestorer();

        JKRHeap* mPreviousHeap; // _0
    };

    void becomeCurrentHeap(JKRHeap *);
    JKRHeap* getCurrentHeap();

    void zeroMemory(void *, u32);
    s32 calcCheckSum(const void *, u32);
};

#endif // MEMORYUTIL_H