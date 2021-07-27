#pragma once

#include "smg.h"
#include "JSystem/JKernel/JKRExpHeap.h"
#include "JSystem/JKernel/JKRSolidHeap.h"

class HeapMemoryWatcher
{
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
    void createRootHeap();
    void createHeaps();
    void createGameHeap();
    void memoryErrorCallback(void *, u32, int);
    void checkRestMemory();

    JKRExpHeap* mStationedHeapNapa; // _0
    JKRExpHeap* mStationedHeapGDDR; // _4
    JKRExpHeap* mGameHeapNapa; // _8
    JKRExpHeap* mGameHeapGDDR; // _C
    JKRSolidHeap* mFileCacheHeap; // _10
    JKRSolidHeap* mSceneHeapNapa; // _14
    JKRSolidHeap* mSceneHeapGDDR; // _18
    JKRExpHeap* mWPadHeap; // _1C
    JKRExpHeap* mHomeButtonLayoutHeap; // _20
    JKRSolidHeap* _24;
    
    static JKRExpHeap* sRootHeapGDDR3;

    static const char* something;
};

namespace
{
    JKRExpHeap* createExpHeap(u32, JKRHeap *, bool);
    JKRSolidHeap* createSolidHeap(u32, JKRHeap *);
    void destroyHeapAndSetNULL(JKRHeap **);
};