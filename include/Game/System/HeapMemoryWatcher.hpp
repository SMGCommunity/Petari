#pragma once

#include "Inline.hpp"
#include <revolution/types.h>

class JKRExpHeap;
class JKRHeap;
class JKRSolidHeap;

class HeapMemoryWatcher {
public:
    HeapMemoryWatcher();

    JKRHeap* getHeapNapa(const JKRHeap*);
    JKRHeap* getHeapGDDR3(const JKRHeap*);
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
    static void memoryErrorCallback(void*, u32, int);
    void checkRestMemory();

    //used in GameSystemObjHolder::initAudio()
    JKRSolidHeap* getAudSystemHeap() {
        return mAudSystemHeap;
    }

    /* 0x00 */ JKRExpHeap* mStationedHeapNapa;
    /* 0x04 */ JKRExpHeap* mStationedHeapGDDR;
    /* 0x08 */ JKRExpHeap* mGameHeapNapa;
    /* 0x0C */ JKRExpHeap* mGameHeapGDDR;
    /* 0x10 */ JKRHeap* mFileCacheHeap;
    /* 0x14 */ JKRSolidHeap* mSceneHeapNapa;
    /* 0x18 */ JKRSolidHeap* mSceneHeapGDDR;
    /* 0x1C */ JKRExpHeap* mWPadHeap;
    /* 0x20 */ JKRExpHeap* mHomeButtonLayoutHeap;
    /* 0x24 */ JKRSolidHeap* mAudSystemHeap;

    static JKRExpHeap* sRootHeapGDDR3;
};
