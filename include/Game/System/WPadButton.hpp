#pragma once

#include <revolution.h>

class WPad;

class WPadButton {
public:
    WPadButton(const WPad*);

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

private:
    /* 0x00 */ const WPad* mPad;
    /* 0x04 */ u32 mHold;
    /* 0x08 */ u32 mTrigger;
    /* 0x0C */ u32 mRelease;
    /* 0x10 */ u32 mRepeat;
    /* 0x14 */ f32 mDelaySec;
    /* 0x18 */ f32 mPulseSec;
};
