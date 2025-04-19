#include "Game/Util.hpp"
#include "Game/System/HeapMemoryWatcher.hpp"
#include "Game/SingletonHolder.hpp"
#include <cstdio>
#include <cstring>
#include <mem.h>
#include <size_t.h>

namespace MR {
    MEMAllocatorFunc NewDeleteAllocator::sAllocatorFunc = {
        NewDeleteAllocator::alloc, 
        NewDeleteAllocator::free, 
    };
    MEMAllocator NewDeleteAllocator::sAllocator = { &sAllocatorFunc };

    u32 calcCheckSum(const void* pPtr, u32 size) {
        u16 sum;
        u16 invSum;

        invSum = 0;
        sum = 0;

        const u16* p = static_cast<const u16*>(pPtr);
        u32 checkSize = size / sizeof(u16);

        for (int i = 0; i < checkSize; i++, p++) {
            sum += *p;
            invSum += ~(*p);
        }

        return (sum << 16) | invSum;
    }

    CurrentHeapRestorer::CurrentHeapRestorer(JKRHeap *pHeap) {
        _0 = JKRHeap::sCurrentHeap;
        OSLockMutex(&MR::MutexHolder<1>::sMutex);
        MR::becomeCurrentHeap(pHeap);
    }

    CurrentHeapRestorer::~CurrentHeapRestorer() {
        MR::becomeCurrentHeap(_0);
        OSUnlockMutex(&MR::MutexHolder<1>::sMutex);
    }

    void* NewDeleteAllocator::alloc(MEMAllocator *pAllocator, u32 size) {
        return new u8[size];
    }

    void NewDeleteAllocator::free(MEMAllocator* pAllocator, void* pPtr) {
        delete static_cast<u8*>(pPtr);
    }

    // MR::getHomeButtonLayoutAllocator

    JKRHeap* getCurrentHeap() {
        return JKRHeap::sCurrentHeap;
    }

    // MR::getAproposHeapForSceneArchive
    
    JKRExpHeap* MR::getStationedHeapNapa() {
        return SingletonHolder<HeapMemoryWatcher>::sInstance->mStationedHeapNapa;
    }

    JKRExpHeap* getStationedHeapGDDR3() {
        return SingletonHolder<HeapMemoryWatcher>::sInstance->mStationedHeapGDDR;
    }

    JKRSolidHeap* getSceneHeapNapa() {
        return SingletonHolder<HeapMemoryWatcher>::sInstance->mSceneHeapNapa;
    }

    JKRSolidHeap* getSceneHeapGDDR3() {
        return SingletonHolder<HeapMemoryWatcher>::sInstance->mSceneHeapGDDR;
    }

    // MR::getHeapNapa
    // MR::getHeapGDDR3

    void becomeCurrentHeap(JKRHeap *pHeap) {
        OSLockMutex(&MR::MutexHolder<1>::sMutex);
        pHeap->becomeCurrentHeap();
        OSUnlockMutex(&MR::MutexHolder<1>::sMutex);
    }

    bool isEqualCurrentHeap(JKRHeap *pHeap) {
        return pHeap == JKRHeap::sCurrentHeap;
    }

    // MR::adjustHeapSize
    // MR::copyMemory

    void fillMemory(void *pDest, u8 a2, size_t size) {
        if (a2 == 0) {
            MR::zeroMemory(pDest, size);
        }
        else {
            memset(pDest, a2, size);
        }
    }

    // MR::zeroMemory
    // MR::calcCheckSum

    void* allocFromWPadHeap(u32 size) {
        return SingletonHolder<HeapMemoryWatcher>::sInstance->mWPadHeap->alloc(size, 0);
    }

    u8 freeFromWPadHeap(void* pPtr) {
        SingletonHolder<HeapMemoryWatcher>::sInstance->mWPadHeap->free(pPtr);

        return 1;
    }
};
