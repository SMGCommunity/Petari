#pragma once

#include <revolution/types.h>

class WPad;

class WPadLeaveWatcher {
public:
    WPadLeaveWatcher(WPad*);

    void update();
    void start();
    void stop();
    void restart();

public:
    /* 0x00 */ WPad* mPad;
    /* 0x04 */ s32 mStep;
    /* 0x08 */ bool mIsSuspend;
};
