#pragma once

#include <revolution.h>

class LayoutActorFlag {
public:
    LayoutActorFlag();

    u8 mIsDead;             // 0x0
    u8 mIsStopAnimFrame;    // 0x1
    u8 mIsHidden;           // 0x2
    u8 mIsOffCalcAnim;      // 0x3
};