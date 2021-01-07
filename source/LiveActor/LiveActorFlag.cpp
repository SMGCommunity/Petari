#include "LiveActor/LiveActorFlag.h"

LiveActorFlag::LiveActorFlag()
{
    mIsDead = 1;
    mIsNotReleasedAnimFrame = 0;
    mIsOnCalcAnim = 0;
    mIsNoCalcView = 0;
    mIsNoEntryDrawBuffer = 0;
    mIsOnBind = 1;
    mIsCalcGravity = 0;
    mIsClipped = 0;
    mIsClippingInvalid = 0;
}