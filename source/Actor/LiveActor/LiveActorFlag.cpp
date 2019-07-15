#include "Actor/LiveActor/LiveActorFlag.h"

LiveActorFlag::LiveActorFlag()
{
    this->mIsDead = 1;
    this->mIsNotReleasedAnimFrame = 0;
    this->mIsOnCalcAnim = 0;
    this->mIsNoCalcView = 0;
    this->mIsNoEntryDrawBuffer = 0;
    this->mIsOnBind = 1;
    this->mIsCalcGravity = 0;
    this->_7 = 0;
    this->_8 = 0;
}