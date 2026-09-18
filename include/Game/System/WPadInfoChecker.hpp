#pragma once

#include <revolution/wpad.h>

class WPad;

class WPadInfoChecker {
public:
    WPadInfoChecker(WPad*);

    s32 getBattery() const;
    void successGetInfo();
    void startCheck() NO_INLINE;
    void reset();
    void update();

public:
    /* 0x00 */ WPad* mPad;
    /* 0x04 */ WPADInfo mPadInfo;
    /* 0x1C */ s32 mCheckInfoFrame;
    /* 0x20 */ s32 mBattery;
};
