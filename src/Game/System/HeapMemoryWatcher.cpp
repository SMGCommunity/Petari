#include "Game/System/HeapMemoryWatcher.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include <JSystem/JKernel/JKRExpHeap.hpp>
#include <JSystem/JKernel/JKRSolidHeap.hpp>
#include <revolution/os.h>
#include <revolution/wpad.h>

JKRExpHeap* HeapMemoryWatcher::sRootHeapGDDR3;

namespace {
    JKRExpHeap* createExpHeap(u32 size, JKRHeap *pHeap, bool a3) NO_INLINE {
        JKRExpHeap* heap;

        if (a3) {
            void* data = new(pHeap, -4) u8[size];
            heap = JKRExpHeap::create(data, size, pHeap, true);
        }
        else {
            heap = JKRExpHeap::create(size, pHeap, true);
        }

        if (MR::isEqualCurrentHeap(heap)) {
            JKRHeap::sRootHeap->becomeCurrentHeap(); 
        }

        return heap;
    }

    JKRSolidHeap* createSolidHeap(u32 size, JKRHeap *pHeap) {
        JKRSolidHeap* heap = JKRSolidHeap::create(size, pHeap, true);

        if (MR::isEqualCurrentHeap(heap)) {
            JKRHeap::sRootHeap->becomeCurrentHeap();
        }

        return heap;
    }

    void destroyHeapAndSetNULL(JKRHeap **pHeap) {
        if (*pHeap != nullptr) {
            JKRHeap::destroy(*pHeap);
            *pHeap = nullptr;
        }
    }
};

JKRHeap* HeapMemoryWatcher::getHeapNapa(const JKRHeap *pHeap) {
    if (pHeap == mStationedHeapNapa || pHeap == mStationedHeapGDDR) {
        return mStationedHeapNapa;
    }
    
    if (pHeap == mSceneHeapNapa || pHeap == mSceneHeapGDDR) {
        return mSceneHeapNapa;
    }

    return nullptr;
}

JKRHeap* HeapMemoryWatcher::getHeapGDDR3(const JKRHeap *pHeap) {
    if (pHeap == mStationedHeapNapa || pHeap == mStationedHeapGDDR) {
        return mStationedHeapGDDR;
    }

    if (pHeap == mSceneHeapNapa || pHeap == mSceneHeapGDDR) {
        return mSceneHeapGDDR;
    }

    return nullptr;
}

void HeapMemoryWatcher::createFileCacheHeapOnGameHeap(u32 size) {
    mFileCacheHeap = createSolidHeap(size, mGameHeapGDDR);
}

void HeapMemoryWatcher::createSceneHeapOnGameHeap() {
    mSceneHeapNapa = createSolidHeap(-1, mGameHeapNapa);
    mSceneHeapGDDR = createSolidHeap(-1, mGameHeapGDDR);
}

void HeapMemoryWatcher::adjustStationedHeaps() {
    MR::adjustHeapSize(mStationedHeapNapa, 0);
    MR::adjustHeapSize(mStationedHeapGDDR, 0);
}

void HeapMemoryWatcher::setCurrentHeapToStationedHeap() {
    MR::becomeCurrentHeap(mStationedHeapNapa);
}

void HeapMemoryWatcher::setCurrentHeapToGameHeap() {
    MR::becomeCurrentHeap(JKRHeap::sSystemHeap);
}

void HeapMemoryWatcher::setCurrentHeapToSceneHeap() {
    MR::becomeCurrentHeap(mSceneHeapNapa);
}

void HeapMemoryWatcher::destroySceneHeap() {
    destroyHeapAndSetNULL((JKRHeap**)&mSceneHeapNapa);
    destroyHeapAndSetNULL((JKRHeap**)&mSceneHeapGDDR);
}

void HeapMemoryWatcher::destroyGameHeap() {
    if (mSceneHeapNapa != nullptr) {
        destroyHeapAndSetNULL((JKRHeap**)&mSceneHeapNapa);
    }

    if (mSceneHeapGDDR != nullptr) {
        destroyHeapAndSetNULL((JKRHeap**)&mSceneHeapGDDR);
    }

    if (mFileCacheHeap != nullptr) {
        destroyHeapAndSetNULL((JKRHeap**)&mFileCacheHeap);
    }

    destroyHeapAndSetNULL((JKRHeap**)&mGameHeapNapa);
    destroyHeapAndSetNULL((JKRHeap**)&mGameHeapGDDR);
    createGameHeap();
}

void HeapMemoryWatcher::createRootHeap() {
    JKRExpHeap* heap;
    void* newHi;
    u32 arenaHi, arenaLo;
    
    JKRExpHeap::createRoot(1, true);
    arenaLo = (u32)OSGetMEM2ArenaLo();
    arenaHi = (u32)OSGetMEM2ArenaHi();
    newHi = (void*)(arenaLo + 0xE00000);
    OSSetMEM2ArenaHi(newHi);
    JKRHeap::setAltAramStartAdr(arenaLo);
    heap = JKRExpHeap::create(newHi, arenaHi - (u32)newHi, JKRHeap::sRootHeap, true);

    if (MR::isEqualCurrentHeap(heap)) {
        JKRHeap::sRootHeap->becomeCurrentHeap();
    }

    HeapMemoryWatcher::sRootHeapGDDR3 = heap;
}

void HeapMemoryWatcher::createHeaps() {
    MR::CurrentHeapRestorer chr = MR::CurrentHeapRestorer(JKRHeap::sRootHeap);
    createExpHeap(0x40000, JKRHeap::sRootHeap, false)->becomeSystemHeap();
    mAudSystemHeap = createSolidHeap(0x1E0000, JKRHeap::sRootHeap);
    mStationedHeapNapa = createExpHeap(0x900000, JKRHeap::sRootHeap, false);
    JKRHeap* gddr = HeapMemoryWatcher::sRootHeapGDDR3;
    u32 thing = OSRoundUp32B(WPADGetWorkMemorySize()) + 208;
    mWPadHeap = createExpHeap(thing, gddr, false);
    mHomeButtonLayoutHeap = createExpHeap(0x80000, HeapMemoryWatcher::sRootHeapGDDR3, false);
    mStationedHeapGDDR = createExpHeap(0x1400000, HeapMemoryWatcher::sRootHeapGDDR3, false);
    createGameHeap();
}

void HeapMemoryWatcher::createGameHeap() {
    mGameHeapNapa = createExpHeap(-1, JKRHeap::sRootHeap, false);
    mGameHeapGDDR = createExpHeap(-1, HeapMemoryWatcher::sRootHeapGDDR3, false);
}

HeapMemoryWatcher::HeapMemoryWatcher() :
    mStationedHeapNapa(nullptr),
    mStationedHeapGDDR(nullptr),
    mGameHeapNapa(nullptr),
    mGameHeapGDDR(nullptr),
    mFileCacheHeap(nullptr),
    mSceneHeapNapa(nullptr),
    mSceneHeapGDDR(nullptr),
    mWPadHeap(nullptr),
    mHomeButtonLayoutHeap(nullptr),
    mAudSystemHeap(nullptr)
{
    JKRHeap::setErrorHandler(HeapMemoryWatcher::memoryErrorCallback);
    createHeaps(); 
}

void HeapMemoryWatcher::memoryErrorCallback(void *, u32, int) {
    OSPanic(__FILE__, 0x219, "");
}

void HeapMemoryWatcher::checkRestMemory() {

}
