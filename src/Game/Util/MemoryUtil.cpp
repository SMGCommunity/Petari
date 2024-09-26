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

    #ifdef NON_MATCHING
    // shrug
    s32 calcCheckSum(const void *pData, u32 a2) {
        u32 v2 = 0;
        u32 v3 = 0;
        
        for (int i = a2 >> 1; i; i--) {
            u16 curData = *(u16*)pData;
            pData = (s8*)pData + 2;
            v3 += curData;
            v2 += ~curData;
        }

        return (v3 << 16) | v2;
    }
    #endif

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

    void NewDeleteAllocator::free(MEMAllocator *pAllocator, void *pData) {
        delete[] (u8*)pData;
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
    // MR::allocFromWPadHeap
    // MR::freeFromWPadHeap
};
