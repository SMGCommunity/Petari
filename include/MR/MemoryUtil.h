#pragma once

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
    bool isEqualCurrentHeap(JKRHeap *);

    void zeroMemory(void *, u32);
    s32 calcCheckSum(const void *, u32);

    JKRHeap* getAproposHeapForSceneArchive(f32);
};