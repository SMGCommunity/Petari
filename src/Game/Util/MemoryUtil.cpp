#include "Game/System/HeapMemoryWatcher.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/MutexHolder.hpp"
#include "Game/SingletonHolder.hpp"
#include <JSystem/JKernel/JKRHeap.hpp>
#include <mem.h>

namespace MR {
    CurrentHeapRestorer::CurrentHeapRestorer(JKRHeap* pHeap) {
        _0 = JKRHeap::sCurrentHeap;
        OSLockMutex(&MR::MutexHolder<1>::sMutex);
        MR::becomeCurrentHeap(pHeap);
    }

    CurrentHeapRestorer::~CurrentHeapRestorer() {
        MR::becomeCurrentHeap(_0);
        OSUnlockMutex(&MR::MutexHolder<1>::sMutex);
    }

    void* NewDeleteAllocator::alloc(MEMAllocator* pAllocator, u32 size) {
        return new u8[size];
    }

    void NewDeleteAllocator::free(MEMAllocator* pAllocator, void* pPtr) {
        delete static_cast<u8*>(pPtr);
    }

    MEMAllocatorFunc NewDeleteAllocator::sAllocatorFunc = {
        NewDeleteAllocator::alloc,
        NewDeleteAllocator::free,
    };
    MEMAllocator NewDeleteAllocator::sAllocator = { &sAllocatorFunc };

    MEMAllocator* getHomeButtonLayoutAllocator() {
        JKRHeapAllocator<0>::sHeap = SingletonHolder<HeapMemoryWatcher>::get()->mHomeButtonLayoutHeap;

        return &JKRHeapAllocator<0>::sAllocator;
    }

    JKRHeap* getCurrentHeap() {
        return JKRHeap::sCurrentHeap;
    }

    // MR::getAproposHeapForSceneArchive
    
    JKRExpHeap* getStationedHeapNapa() {
        return SingletonHolder<HeapMemoryWatcher>::get()->mStationedHeapNapa;
    }

    JKRExpHeap* getStationedHeapGDDR3() {
        return SingletonHolder<HeapMemoryWatcher>::get()->mStationedHeapGDDR;
    }

    JKRSolidHeap* getSceneHeapNapa() {
        return SingletonHolder<HeapMemoryWatcher>::get()->mSceneHeapNapa;
    }

    JKRSolidHeap* getSceneHeapGDDR3() {
        return SingletonHolder<HeapMemoryWatcher>::get()->mSceneHeapGDDR;
    }

    JKRHeap* getHeapNapa(const JKRHeap* pHeap) {
        return SingletonHolder<HeapMemoryWatcher>::get()->getHeapNapa(pHeap);
    }

    JKRHeap* getHeapGDDR3(const JKRHeap* pHeap) {
        return SingletonHolder<HeapMemoryWatcher>::get()->getHeapGDDR3(pHeap);
    }

    void becomeCurrentHeap(JKRHeap* pHeap) {
        OSLockMutex(&MR::MutexHolder<1>::sMutex);
        pHeap->becomeCurrentHeap();
        OSUnlockMutex(&MR::MutexHolder<1>::sMutex);
    }

    bool isEqualCurrentHeap(JKRHeap* pHeap) {
        return JKRHeap::sCurrentHeap == pHeap;
    }

    // MR::adjustHeapSize
    // MR::copyMemory

    void fillMemory(void* pDst, u8 ch, u32 size) {
        if (ch == 0) {
            zeroMemory(pDst, size);
        }
        else {
            memset(pDst, ch, size);
        }
    }

    // MR::zeroMemory

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

    void* allocFromWPadHeap(u32 size) {
        return SingletonHolder<HeapMemoryWatcher>::get()->mWPadHeap->alloc(size, 0);
    }

    u8 freeFromWPadHeap(void* pPtr) {
        SingletonHolder<HeapMemoryWatcher>::get()->mWPadHeap->free(pPtr);

        return 1;
    }

    template<int N>
    void* JKRHeapAllocator<N>::alloc(MEMAllocator* pAllocator, u32 size) {
        return JKRHeapAllocator<N>::sHeap->alloc(size, 0);
    }

    template<int N>
    void JKRHeapAllocator<N>::free(MEMAllocator* pAllocator, void* pPtr) {
        JKRHeapAllocator<N>::sHeap->free(pPtr);
    }

    template<int N>
    MEMAllocator JKRHeapAllocator<N>::sAllocator = { &sAllocatorFunc, nullptr, 4, 0 };

    template<int N>
    MEMAllocatorFunc JKRHeapAllocator<N>::sAllocatorFunc = {
        JKRHeapAllocator::alloc,
        JKRHeapAllocator::free,
    };
};
