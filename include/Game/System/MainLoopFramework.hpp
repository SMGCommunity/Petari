#pragma once

#include "Game/Util/BothDirList.hpp"
#include "Game/Util/Color.hpp"
#include <JSystem/JUtility/JUTXfb.hpp>
#include <revolution.h>

typedef void (*UnkCallback)();

class MainLoopFrameworkAlarm : public OSAlarm {
public:
    u32 _dummy; // helps with waitDrawDoneAndSetAlarm stack frame size
    
    static MR::BothDirList<MainLoopFrameworkAlarm> sList;
};

class MainLoopFramework {
public:
    MainLoopFramework(void* param1, void* param2, void* param3, bool param4) {
        ctor_subroutine(param4);
        JUTXfb::createManager(param1, param2, param3);
    }

    ~MainLoopFramework();

    void ctor_subroutine(bool);
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
    void setForOSResetSystem();

    void setCombinationRatio(f32 ratio) {
        mCombinationRatio = ratio;
        if (mCombinationRatio > 1f) {
            mCombinationRatio = 1f;
        }
    }

    static MainLoopFramework* createManager(const GXRenderModeObj*, void*, void*, void*, bool);

    Color8 _0;
    u32 _4;
    u16 _8;
    u16 _A;
    u32 _C;
    /* 0x10 */ u16 mRetraceCount;
    /* 0x14 */ u32 mTickDuration;
    bool _18;
    bool _19;
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

    u32 _pad1;
    u32 _pad2;

    static MainLoopFramework* sManager;
};
