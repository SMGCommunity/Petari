#include "System/HeapMemoryWatcher.h"

#include "Util/MemoryUtil.h"

#include <revolution/os.h>
#include <revolution/wpad.h>

namespace
{
    /*JKRExpHeap* createExpHeap(u32 size, JKRHeap *pHeap, bool a3)
    {
        JKRExpHeap* heap;

        if (a3)
        {
            heap = JKRExpHeap::create(size, pHeap, 1);
        }
        else
        {
            void* data = new(pHeap, -4);
            heap = JKRExpHeap::create(data, size, pHeap, 1);
        }

        return heap;
    }*/

    __declspec(noinline)
    JKRSolidHeap* createSolidHeap(u32 size, JKRHeap *pHeap)
    {
        JKRSolidHeap* heap = JKRSolidHeap::create(size, pHeap, true);

        if (MR::isEqualCurrentHeap(heap))
        {
            JKRHeap::sRootHeap->becomeCurrentHeap();
        }

        return heap;
    }

    /* this does not match */
    __declspec(noinline)
    void destroyHeapAndSetNULL(JKRHeap **pHeap)
    {
        JKRHeap* heap = *pHeap;
        if (heap)
        {
            heap->destroy();
            pHeap = NULL;
        }
    }
};

JKRHeap* HeapMemoryWatcher::getHeapNapa(const JKRHeap *pHeap)
{
    if (pHeap == mStationedHeapNapa || pHeap == mStationedHeapGDDR)
        return mStationedHeapNapa;
    
    if (pHeap == mSceneHeapNapa || pHeap == mSceneHeapGDDR)
        return mSceneHeapNapa;

    return NULL;
}

JKRHeap* HeapMemoryWatcher::getHeapGDDR3(const JKRHeap *pHeap)
{
    if (pHeap == mStationedHeapNapa || pHeap == mStationedHeapGDDR)
        return mStationedHeapGDDR;

    if (pHeap == mSceneHeapNapa || pHeap == mSceneHeapGDDR)
        return mSceneHeapGDDR;

    return NULL;
}

void HeapMemoryWatcher::createFileCacheHeapOnGameHeap(u32 size)
{
    mFileCacheHeap = createSolidHeap(size, mGameHeapGDDR);
}

void HeapMemoryWatcher::createSceneHeapOnGameHeap()
{
    mSceneHeapNapa = createSolidHeap(-1, mGameHeapNapa);
    mSceneHeapGDDR = createSolidHeap(-1, mGameHeapGDDR);
}

void HeapMemoryWatcher::adjustStationedHeaps()
{
    MR::adjustHeapSize(mStationedHeapNapa, NULL);
    MR::adjustHeapSize(mStationedHeapGDDR, NULL);
}

void HeapMemoryWatcher::setCurrentHeapToStationedHeap()
{
    MR::becomeCurrentHeap(mStationedHeapNapa);
}

void HeapMemoryWatcher::setCurrentHeapToGameHeap()
{
    MR::becomeCurrentHeap(JKRHeap::sSystemHeap);
}

void HeapMemoryWatcher::setCurrentHeapToSceneHeap()
{
    MR::becomeCurrentHeap(mSceneHeapNapa);
}

void HeapMemoryWatcher::destroySceneHeap()
{
    destroyHeapAndSetNULL((JKRHeap**)&mSceneHeapNapa);
    destroyHeapAndSetNULL((JKRHeap**)&mSceneHeapGDDR);
}

void HeapMemoryWatcher::destroyGameHeap()
{
    if (mSceneHeapNapa)
        destroyHeapAndSetNULL((JKRHeap**)&mSceneHeapNapa);

    if (mSceneHeapGDDR)
        destroyHeapAndSetNULL((JKRHeap**)&mSceneHeapGDDR);

    if (mFileCacheHeap)
        destroyHeapAndSetNULL((JKRHeap**)&mFileCacheHeap);

    destroyHeapAndSetNULL((JKRHeap**)&mGameHeapNapa);
    destroyHeapAndSetNULL((JKRHeap**)&mGameHeapGDDR);

    createGameHeap();
}

/* non matching */
void HeapMemoryWatcher::createRootHeap()
{
    JKRExpHeap::createRoot(1, 1);
    void* arenaLo = OSGetMEM2ArenaLo();
    void* arenaHi = OSGetMEM2ArenaHi();
    OSSetMEM2ArenaHi(static_cast<s32*>(arenaHi) + 0xE0);
    JKRHeap::setAltAramStartAdr((u32)(arenaLo));
    JKRExpHeap* newHeap = JKRExpHeap::create(static_cast<s32*>(arenaHi) + 0xE0, (static_cast<s32*>(arenaHi) + 0xE0) - arenaLo, JKRHeap::sRootHeap, 1);

    if (!MR::isEqualCurrentHeap(newHeap))
    {
        JKRHeap::sRootHeap->becomeCurrentHeap();
    }

    HeapMemoryWatcher::sRootHeapGDDR3 = newHeap;
}

/* todo -- finish */
void HeapMemoryWatcher::createHeaps()
{
    MR::CurrentHeapRestorer heapRestorer(JKRHeap::sRootHeap);

    createExpHeap(3, JKRHeap::sRootHeap, 0)->becomeSystemHeap();
    _24 = createSolidHeap(0x1E, JKRHeap::sRootHeap);
    mStationedHeapNapa = createExpHeap(0x90, JKRHeap::sRootHeap, false);

    u32 size = WPADGetWorkMemorySize();
    //mWPadHeap = createExpHeap((size + 0x1F) & 0xFFFFFFE0 + 208, HeapMemoryWatcher::sRootHeapGDDR3, false);
}

void HeapMemoryWatcher::createGameHeap()
{
    mGameHeapNapa = createExpHeap(-1, JKRHeap::sRootHeap, false);
    mGameHeapGDDR = createExpHeap(-1, HeapMemoryWatcher::sRootHeapGDDR3, false);
}

void HeapMemoryWatcher::memoryErrorCallback(void *pFunc, u32 a2, int a3)
{
    OSPanic("HeapMemoryWatcher.cpp", 0x219, HeapMemoryWatcher::something);
}

void HeapMemoryWatcher::checkRestMemory()
{
    
}