#pragma once

#include "Game/Util/BothDirList.hpp"
#include <revolution.h>

typedef void (*UnkCallback)();

class MainLoopFrameworkAlarm {
public:
    static MR::BothDirPtrList sList;
};

class MainLoopFramework {
public:
    void prepareCopyDisp();
    void drawendXfb_single();
    void exchangeXfb_double();
    void exchangeXfb_triple();
    void copyXfb_triple();
    void preGX();
    void endGX();
    void waitForRetrace();
    void beginRender();
    void endRender();
    void endFrame();
    void waitBlanking(int);
    void setTickRateFromFrame(u16);
    void clearEfb(GXColor);
    void clearEfb(int, int, int, int, GXColor);
    void calcCombinationRatio();
    u32 frameToTick(f32);

    static MainLoopFramework* get() {
        return MainLoopFramework::sManager;
    }

    GXColor _0;
    u32 _4;
    u16 _8;
    u16 _A;
    u32 _C;
    /* 0x10 */ u16 mRetraceCount;
    /* 0x14 */ u32 mTickDuration;
    u8 _18;
    u8 _19;
    u8 _1A;
    u8 _1B;
    /* 0x1C */ f32 mCombinationRatio;
    /* 0x20 */ OSTick mTick;
    s32 _24;
    s32 _28;
    s32 _2C;
    s32 _30;
    u8 _34;
    u8 _35;
    u8 _36;
    u8 _37;
    UnkCallback _38;
    s16 _3C;
    u8 _3E;

private:
    static MainLoopFramework* sManager;
};
