#pragma once

#include <revolution.h>

class LayoutActorFlag {
public:
    LayoutActorFlag();

    u8 mIsDead;             // _0
    u8 mIsStopAnimFrame;    // _1
    u8 mIsHidden;           // _2
    u8 mIsOffCalcAnim;      // _3
};