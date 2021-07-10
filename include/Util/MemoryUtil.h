#pragma once

#include <revolution.h>
#include "JKernel/JKRHeap.h"
#include "revolution/os/OSMutex.h"
#include "revolution/mem.h"

class JKRSolidHeap;
class JKRExpHeap;

namespace MR
{
    class CurrentHeapRestorer
    {
    public:
        CurrentHeapRestorer(JKRHeap *);
        ~CurrentHeapRestorer();

        JKRHeap* mPreviousHeap; // _0
    };

    template<int T>
    class JKRHeapAllocator
    {
    public:
        void* alloc(MEMAllocator *, u32);
        void free(MEMAllocator *, void *);

        static JKRHeap* sHeap;
    };

    JKRHeap* getStationedHeapNapa();
    JKRHeap* getStationedHeapGDDR3();
    JKRSolidHeap* getSceneHeapNapa();
    JKRSolidHeap* getSceneHeapGDDR3();
    JKRHeap* getHeapNapa(const JKRHeap *);
    JKRHeap* getHeapGDDR3(const JKRHeap *);
    void becomeCurrentHeap(JKRHeap *);
    JKRHeap* getCurrentHeap();
    bool isEqualCurrentHeap(JKRHeap *);
    void adjustHeapSize(JKRExpHeap *, const char *);
    void copyMemory(void *, const void *, u32);
    void fillMemory(void *, u8, u32);
    void zeroMemory(void *, u32);
    s32 calcCheckSum(const void *, u32);

    void* allocFromWPadHeap(u32);
    u32 freeFromWPadHeap(void *);

    JKRHeap* getAproposHeapForSceneArchive(f32);
};