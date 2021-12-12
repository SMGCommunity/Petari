#pragma once

#include <revolution.h>
#include <revolution/mem.h>
#include "JSystem/JKernel/JKRHeap.h"

class JKRHeap;

namespace MR {
    void becomeCurrentHeap(JKRHeap *);
    JKRHeap* getCurrentHeap();
    bool isEqualCurrentHeap(JKRHeap *);

    void copyMemory(void *, const void *, u32);
    void fillMemory(void *, u8, size_t);
    void zeroMemory(void *, u32);
    s32 calcCheckSum(const void *, u32);

    JKRHeap* getStationedHeapNapa();

    JKRHeap* getAproposHeapForSceneArchive(f32);

    class CurrentHeapRestorer {
    public:
        CurrentHeapRestorer(JKRHeap *);
        ~CurrentHeapRestorer();

        JKRHeap* _0;
    };

    class NewDeleteAllocator {
    public:
        static void* alloc(MEMAllocator *, u32);
        static void free(MEMAllocator *, void *);
    };

    template<int T>
    class JKRHeapAllocator {
    public:
        static void* alloc(MEMAllocator *pAllocator, u32 size) {
            return JKRHeapAllocator<T>::sHeap->alloc(size, 0);
        }

        static void free(MEMAllocator *pAllocator, void *pData) {
            JKRHeapAllocator<T>::sHeap->free(pData);
        }

        static JKRHeap* sHeap;
    };
};