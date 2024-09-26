#pragma once

#include "Game/System/WPad.hpp"
#include <revolution.h>

class WPadInfoChecker {
public:
    WPadInfoChecker(WPad *);

    s32 getBattery() const;
    void successGetInfo();
    void startCheck() NO_INLINE;
    void reset();
    void update();

    WPad* mPad;             // 0x0
    WPADInfo mPadInfo;      // 0x4
    u32 _1C;
    s32 mBattery;           // 0x20
};