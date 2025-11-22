#pragma once

#include "Game/Util/BothDirList.hpp"
#include "Game/Util/Color.hpp"
#include <JSystem/JUtility/JUTXfb.hpp>
#include <revolution.h>

typedef void (*PreRenderCallback)();

class MainLoopFrameworkAlarm : public OSAlarm {
public:
    u32 _dummy;  // helps with waitDrawDoneAndSetAlarm stack frame size

    static MR::BothDirList< MainLoopFrameworkAlarm > sList;
};

class MainLoopFramework {
public:
    MainLoopFramework(void* xfb1, void* xfb2, void* xfb3, bool useAlpha) : mClearColor(0xffffffff) {
        ctor_subroutine(useAlpha);
        JUTXfb::createManager(xfb1, xfb2, xfb3);
    }

    ~MainLoopFramework();

    static MainLoopFramework* createManager(const GXRenderModeObj*, void*, void*, void*, bool);
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
    static u32 frameToTick(f32);
    static void setForOSResetSystem();

    /* 0x0 */ Color8 mClearColor;
    /* 0x4 */ u32 mClearZ;
    /* 0x8 */ u16 mDispCopyGamma;
    /* 0xA */ u16 mCopyClamp;
    u32 _C;
    /* 0x10 */ u16 mRetraceCount;
    /* 0x14 */ u32 mTickDuration;
    /* 0x18 */ bool mUseAlpha;
    /* 0x19 */ bool mUseVFilter;
    u8 _1A;
    u8 _1B;
    /* 0x1C */ f32 mCombinationRatio;
    /* 0x20 */ OSTick mTick;
    /* 0x24 */ s32 mLastFrameTime;
    /* 0x28 */ s32 mLastVideoTickDelta;
    /* 0x2C */ s32 mRenderInterval;
    /* 0x30 */ s32 mRenderCounter;
    /* 0x34 */ u8 mDoRenderFrame;
    u8 _35;
    u8 _36;
    u8 _37;
    /* 0x38 */ PreRenderCallback mPreRenderCallback;
    /* 0x3C */ s16 mSingleBufferIndex;
    u8 _3E;

    u32 _pad1;
    u32 _pad2;

    static MainLoopFramework* sManager;
};
