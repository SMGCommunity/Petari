#pragma once

#include "Game/System/WPad.hpp"

class WPadButton {
public:
    WPadButton(const WPad *);

    void update();
    bool testButtonUp() const;
    bool testButtonDown() const;
    bool testButtonLeft() const;
    bool testButtonRight() const;
    bool testButtonA() const;
    bool testButtonB() const;
    bool testButtonMinus() const;
    bool testButtonPlus() const;
    bool testButton1() const;
    bool testButton2() const;
    bool testButtonC() const;
    bool testButtonZ() const;
    bool testTriggerUp() const;
    bool testTriggerDown() const;
    bool testTriggerLeft() const;
    bool testTriggerRight() const;
    bool testTriggerA() const;
    bool testTriggerB() const;
    bool testTriggerMinus() const;
    bool testTriggerHome() const;
    bool testTriggerPlus() const;
    bool testTrigger1() const;
    bool testTrigger2() const;
    bool testTriggerC() const;
    bool testTriggerZ() const;
    bool testReleaseZ() const;
    bool isChangeAnyState() const;

    const WPad* mPad;       // 0x0
    u32 mHold;              // 0x4
    u32 _8;
    u32 mRelease;           // 0xC
    u32 _10;
    f32 mDelaySec;          // 0x14
    f32 mPulseSec;          // 0x18
};