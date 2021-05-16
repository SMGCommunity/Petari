#include "Util/MemoryUtil.h"
#include "Util/ModelUtil.h"

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
};