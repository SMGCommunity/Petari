#pragma once

#include "Game/System/WPad.h"
#include <revolution.h>

class WPadInfoChecker {
public:
    WPadInfoChecker(WPad *);

    s32 getBattery() const;
    void successGetInfo();
    void startCheck() NO_INLINE;
    void reset();
    void update();

    WPad* mPad;             // _0
    WPADInfo mPadInfo;      // _4
    u32 _1C;
    s32 mBattery;           // _20
};