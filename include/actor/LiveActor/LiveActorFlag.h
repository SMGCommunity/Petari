#pragma once

#include "types.h"

class LiveActorFlag
{
public:
    LiveActorFlag();

    bool mIsDead; // _0
    bool mIsNotReleasedAnimFrame; // _1
    bool mIsOnCalcAnim; // _2
    bool mIsNoCalcView; // _3
    bool mIsNoEntryDrawBuffer; // _4
    bool mIsOnBind; // _5
    bool mIsCalcGravity; // _6
    bool _7;
    bool _8;
};