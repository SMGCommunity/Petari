#pragma once

#include "Game/SingletonHolder.hpp"
#include "Game/System/HeapMemoryWatcher.hpp"
#include <revolution/mem.h>

class JKRExpHeap;
class JKRHeap;
class JKRSolidHeap;

namespace MR {
    class CurrentHeapRestorer {
    public:
        CurrentHeapRestorer(JKRHeap*);
        ~CurrentHeapRestorer();

    private:
        /* 0x00 */ JKRHeap* _0;
    };

    class NewDeleteAllocator {
    public:
        static void* alloc(MEMAllocator* pAllocator, u32 size);
        static void free(MEMAllocator* pAllocator, void* pPtr);

        static MEMAllocatorFunc sAllocatorFunc;
        static MEMAllocator sAllocator;
    };

    MEMAllocator* getHomeButtonLayoutAllocator();
    JKRHeap* getCurrentHeap();
    JKRHeap* getAproposHeapForSceneArchive(f32);
    JKRExpHeap* getStationedHeapNapa();
    JKRExpHeap* getStationedHeapGDDR3();
    JKRSolidHeap* getSceneHeapNapa();
    JKRSolidHeap* getSceneHeapGDDR3();
    JKRHeap* getHeapNapa(const JKRHeap* pHeap);
    JKRHeap* getHeapGDDR3(const JKRHeap* pHeap);
    void becomeCurrentHeap(JKRHeap* pHeap);
    bool isEqualCurrentHeap(JKRHeap* pHeap);
    void adjustHeapSize(JKRExpHeap* pHeap, const char* pParam2);
    void copyMemory(void* pDst, const void* pSrc, u32 size);
    void fillMemory(void* pDst, u8 ch, u32 size);
    void zeroMemory(void* pDst, u32 size);
    u32 calcCheckSum(const void* pPtr, u32 size);
    void* allocFromWPadHeap(u32 size);
    u8 freeFromWPadHeap(void* pPtr);

    template<int N>
    class JKRHeapAllocator {
    public:
        static void* alloc(MEMAllocator* pAllocator, u32 size);
        static void free(MEMAllocator* pAllocator, void* pPtr);

        static MEMAllocator sAllocator;
        static MEMAllocatorFunc sAllocatorFunc;
        static JKRHeap* sHeap;
    };

    inline JKRSolidHeap* getAudHeap() {
        return SingletonHolder<HeapMemoryWatcher>::get()->mAudSystemHeap;
    }
};
