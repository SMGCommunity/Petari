#ifndef MEMORYUTIL_H
#define MEMORYUTIL_H

#include <revolution.h>
#include "JKernel/JKRHeap.h"
#include "revolution/os/OSMutex.h"

namespace MR
{
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

    JKRHeap* getAproposHeapForSceneArchive(f32);
};

#endif // MEMORYUTIL_H