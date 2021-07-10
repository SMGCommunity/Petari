#include "Util/MemoryUtil.h"
#include "Util/ModelUtil.h"
#include "System/HeapMemoryWatcher.h"
#include "SingletonHolder.h"

namespace MR
{
    CurrentHeapRestorer::CurrentHeapRestorer(JKRHeap *pHeap)
    {
        mPreviousHeap = JKRHeap::sCurrentHeap;
        OSLockMutex(&MutexHolder<1>::sMutex);
        MR::becomeCurrentHeap(pHeap);
    }

    CurrentHeapRestorer::~CurrentHeapRestorer()
    {
        MR::becomeCurrentHeap(mPreviousHeap);
        OSUnlockMutex(&MutexHolder<1>::sMutex);
    }

    JKRHeap* getCurrentHeap()
    {
        return JKRHeap::sCurrentHeap;
    }

    void becomeCurrentHeap(JKRHeap *pHeap)
    {
        OSMutex* mutex = &MR::MutexHolder<1>::sMutex;

        OSLockMutex(mutex);
        pHeap->becomeCurrentHeap();
        OSUnlockMutex(mutex);
    }

    bool isEqualCurrentHeap(JKRHeap *pHeap)
    {
        return JKRHeap::sCurrentHeap == pHeap;
    }

    void adjustHeapSize(JKRExpHeap *pHeap, const char *pName)
    {
        pHeap->adjustSize();
    }

    // not even close
    s32 calcCheckSum(const void *pSrc, u32 a2)
    {
        s32 v2 = 0;
        s32 v3 = 0;

        for (s32 i = a2 >> 1; i; --i)
        {
            int v5 = *static_cast<const int*>(pSrc);
            pSrc = static_cast<const int*>(pSrc) + 2;
            v3 += v5;
            v2 += ~v5;
        }

        return (v3 << 16) | v2;
    }

    void* allocFromWPadHeap(u32 size)
    {
        return SingletonHolder<HeapMemoryWatcher>::sInstance->mWPadHeap->alloc(size, 0);
    }

    u32 freeFromWPadHeap(void *pSrc)
    {
        SingletonHolder<HeapMemoryWatcher>::sInstance->mWPadHeap->free(pSrc);
        return 1;
    }
};