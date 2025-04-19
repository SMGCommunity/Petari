#pragma once

#include <revolution.h>
#include <revolution/mem.h>
#include <cstring>
#include <size_t.h>
#include "JSystem/JKernel/JKRHeap.hpp"
#include "Game/SingletonHolder.hpp"
#include "Game/System/HeapMemoryWatcher.hpp"

class JKRHeap;
class JKRExpHeap;
class JKRSolidHeap;

namespace MR {
    MEMAllocator* getHomeButtonLayoutAllocator();
    void becomeCurrentHeap(JKRHeap *);
    JKRHeap* getCurrentHeap();
    bool isEqualCurrentHeap(JKRHeap *);

    void copyMemory(void *, const void *, u32);
    void fillMemory(void *, u8, u32);
    void zeroMemory(void *, u32);
    u32 calcCheckSum(const void* pPtr, u32 size);

    void* allocFromWPadHeap(u32);
    u8 freeFromWPadHeap(void *);

    JKRExpHeap* getStationedHeapNapa();
    JKRExpHeap* getStationedHeapGDDR3();
    JKRSolidHeap* getSceneHeapNapa();
    JKRSolidHeap* getSceneHeapGDDR3();
    JKRHeap* getHeapNapa(const JKRHeap* pHeap);
    JKRHeap* getHeapGDDR3(const JKRHeap* pHeap);

    JKRHeap* getAproposHeapForSceneArchive(f32);

    void adjustHeapSize(JKRExpHeap *, const char *);

    inline JKRSolidHeap* getAudHeap() {
        return SingletonHolder<HeapMemoryWatcher>::get()->mAudSystemHeap;
    }

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

        static MEMAllocatorFunc sAllocatorFunc;

        static MEMAllocator sAllocator;
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

        static MEMAllocator sAllocator;
        static MEMAllocatorFunc sAllocatorFunc;
        static JKRHeap* sHeap;
    };
};