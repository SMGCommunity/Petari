#include "Actor/LiveActor/LiveActor.h"
#include "Actor/LiveActor/AllLiveActorGroup.h"
#include "Actor/Clipping/ClippingDirector.h"
#include "Actor/NameObj/NameObjExecuteHolder.h"
#include "MR/actor/ActorMovementUtil.h"
#include "MR/actor/ActorSensorUtil.h"

LiveActor::LiveActor(const char *name) : NameObj(name)
{
    f32 zero = LiveActor::zero;
    f32 one = LiveActor::one;
    f32 neg = LiveActor::neg_one;

    this->mTranslation.x = zero;
    this->mTranslation.y = zero;
    this->mTranslation.z = zero;

    this->mRotation.x = zero;
    this->mRotation.y = zero;
    this->mRotation.z = zero;

    this->mScale.x = one;
    this->mScale.y = one;
    this->mScale.z = one;

    this->mVelocity.x = zero;
    this->mVelocity.y = zero;
    this->mVelocity.z = zero;

    this->mGravity.x = zero;
    this->mGravity.y = neg_one;
    this->mGravity.z = zero;

    this->_48 = 0;
    this->_4C = 0;
    this->mSpine = 0;
    this->mSensorKeeper = 0;
    this->_58 = 0;
    this->_5C = 0;
    this->_60 = 0;
    this->_64 = 0;

    LiveActorFlag flags;
    this->mFlags = flags;

    this->_74 = 0;
    this->_78 = 0;
    this->_7C = 0;
    this->_80 = 0;
    this->mLightCtrl = 0;
    this->_88 = 0;

    AllLiveActorGroup* group = MR::getAllLiveActorGroup();
    group->registerActor(this);

    ClippingDirector* director = MR::getClippingDirector();
    director->registerActor(this);
}

void LiveActor::init(const JMapInfoIter &iter) 
{ 
    return;

}

void LiveActor::appear()
{
    this->makeActorAppeared();
}

void LiveActor::makeActorAppeared()
{
    if (this->mSensorKeeper != 0)
    {
        this->mSensorKeeper->validateBySystem();
    }

    if (MR::isClipped(this))
    {
        this->endClipped();
    }

    u32* colParts = this->_78;
    this->mFlags.mIsDead = 0;

    if (colParts != 0)
    {
        MR::validateCollisionParts(this);
    }

    MR::resetPosition(this);

    if (this->mLightCtrl != 0)
    {
        this->mLightCtrl->reset();
    }

    MR::tryUpdateHitSensorAll(this);
    MR::addToClippingTarget(this);
    MR::connectToSceneTemporarily(this);
    
    if (MR::isNoEntryDrawBuffer(this))
    {
        MR::connectToDrawTemporarily(this);
    }
}

void LiveActor::kill()
{
    this->makeActorDead();
}

void LiveActor::makeActorDead()
{
    f32 zero = LiveActor::zero;

    this->mVelocity.z = zero;
    this->mVelocity.y = zero;
    this->mVelocity.x = zero;

    MR::clearHitSensors(this);

    if (this->mSensorKeeper != 0)
    {
        this->mSensorKeeper->invalidateBySystem();
    }

    if (this->_58 != 0)
    {
        // this->_58->clear()
    }

    if (this->_60 != 0)
    {
        // this->_60->clear()
    }

    if (this->_78 != 0)
    {
        MR::invalidateCollisionParts(this);
    }

    this->mFlags.mIsDead = 1;

    MR::removeFromClippingTarget(this);
    MR::disconnectToSceneTemporarily(this);
    MR::disconnectToDrawTemporarily(this);

}