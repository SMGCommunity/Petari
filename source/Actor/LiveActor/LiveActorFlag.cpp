#include "Actor/LiveActor/LiveActorFlag.h"

LiveActorFlag::LiveActorFlag()
{
    mIsDead = 1;
    mIsNotReleasedAnimFrame = 0;
    mIsOnCalcAnim = 0;
    mIsNoCalcView = 0;
    mIsNoEntryDrawBuffer = 0;
    mIsOnBind = 1;
    mIsCalcGravity = 0;
    _7 = 0;
    _8 = 0;
}