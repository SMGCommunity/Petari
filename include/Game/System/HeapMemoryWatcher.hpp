#pragma once

#include <revolution.h>
#include "JSystem/JKernel/JKRExpHeap.hpp"
#include "JSystem/JKernel/JKRSolidHeap.hpp"
#include "Inline.hpp"

namespace {
    JKRExpHeap* createExpHeap(u32, JKRHeap *, bool) NO_INLINE;
    JKRSolidHeap* createSolidHeap(u32, JKRHeap *) NO_INLINE;
    void destroyHeapAndSetNULL(JKRHeap **) NO_INLINE;
};

class HeapMemoryWatcher {
public:
    HeapMemoryWatcher();

    JKRHeap* getHeapNapa(const JKRHeap *);
    JKRHeap* getHeapGDDR3(const JKRHeap *);
    void createFileCacheHeapOnGameHeap(u32);
    void createSceneHeapOnGameHeap();
    void adjustStationedHeaps();
    void setCurrentHeapToStationedHeap();
    void setCurrentHeapToGameHeap();
    void setCurrentHeapToSceneHeap();
    void destroySceneHeap();
    void destroyGameHeap();
    static void createRootHeap();
    void createHeaps();
    void createGameHeap();
    static void memoryErrorCallback(void *, u32, int);
    void checkRestMemory();

    JKRExpHeap* mStationedHeapNapa;     // _0
    JKRExpHeap* mStationedHeapGDDR;     // _4
    JKRExpHeap* mGameHeapNapa;          // _8
    JKRExpHeap* mGameHeapGDDR;          // _C
    JKRHeap* mFileCacheHeap;            // _10
    JKRSolidHeap* mSceneHeapNapa;       // _14
    JKRSolidHeap* mSceneHeapGDDR;       // _18
    JKRExpHeap* mWPadHeap;              // _1C
    JKRExpHeap* mHomeButtonLayoutHeap;  // _20
    JKRSolidHeap* mAudSystemHeap;       // _24

    static JKRExpHeap* sRootHeapGDDR3;
};