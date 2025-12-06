#pragma once

#include <revolution.h>

class Fifo {
public:
    Fifo* pop();
    u32 getLoopIdx(u32);
    u32 getCount();

    Fifo** mArray;  // 0x0
    u32 mCount;     // 0x4
    u32 mLoopIdx;   // 0x8
    u32 _C;
};

class DrawSyncManager {
public:
    DrawSyncManager(u32, s32);

    ~DrawSyncManager();

    void reset(bool);

    static DrawSyncManager* start(u32, s32);
    static void prepareReset();
    static void resetIfAborted();

    void clearFifo();
    static void end();

    void pushBreakPoint();

    void drawSyncCallback(u16);

    static DrawSyncManager* sInstance;

    u8 _0[0x28];
    OSThread mThread;       // 0x28
    OSMessageQueue mQueue;  // 0x340
    u32 _360;
    u32 _364;
    Fifo* mFifo;  // 0x368
    u16 _36C;
    u16 _36E;
    u16 _370;
    u8 _372;
    u8 _373;
    u32 _374;
};