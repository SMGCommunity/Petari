#include "Actor/LiveActor/LiveActor.h"
#include "Actor/LiveActor/AllLiveActorGroup.h"
#include "Actor/Clipping/ClippingDirector.h"
#include "Actor/NameObj/NameObjExecuteHolder.h"
#include "Actor/Shadow/ShadowController.h"
#include "MR/actor/ActorMovementUtil.h"
#include "MR/actor/ActorSensorUtil.h"
#include "MR/ModelUtil.h"
#include "MR/SoundUtil.h"

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

    this->mModelManager = 0;
    this->mAnimKeeper = 0;
    this->mSpine = 0;
    this->mSensorKeeper = 0;
    this->mBinder = 0;
    this->_5C = 0;
    this->mEffectKeeper = 0;
    this->_64 = 0;

    LiveActorFlag flags;
    this->mFlags = flags;

    this->_74 = 0;
    this->_78 = 0;
    this->_7C = 0;
    this->_80 = 0;
    this->mLightCtrl = 0;
    this->mCameraCtrl = 0;

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

    MR::tryUpdateHitSensorsAll(this);
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

    if (this->mBinder != 0)
    {
        this->mBinder->clear();
    }

    if (this->mEffectKeeper != 0)
    {
        this->mEffectKeeper->clear();
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

void LiveActor::movement()
{
    if (this->mModelManager != 0)
    {
        if (this->mFlags.mIsNotReleasedAnimFrame)
        {
            this->mModelManager->update();

            if (this->mAnimKeeper != 0)
            {
                this->mAnimKeeper->update();
            }
        }
    }

    if (MR::isCalcGravity(this))
    {
        MR::calcGravity(this);
    }

    if (this->mSensorKeeper != 0)
    {
        this->mSensorKeeper->doObjCol();
    }

    if (!this->mFlags.mIsDead)
    {
        if (this->mSpine != 0)
        {
            this->mSpine->update();
        }

        if (!this->mFlags.mIsDead)
        {
            this->control();

            if (!this->mFlags.mIsDead)
            {
                this->updateBinder();

                if (this->mEffectKeeper != 0)
                {
                    this->mEffectKeeper->update();
                }

                if (this->mCameraCtrl != 0)
                {
                    this->mCameraCtrl->update();
                }

                if (this->mLightCtrl != 0)
                {
                    MR::updateLightCtrl(this);
                }

                MR::tryUpdateHitSensorsAll(this);
                MR::actorSoundMovement(this);
                MR::requestCalcActorShadow(this);
            }
        }
    }
}

void LiveActor::calcAnim()
{
    if (!this->mFlags.mIsOnCalcAnim)
    {
        this->calcAnmMtx();
    }

    if (this->_78 != 0)
    {
        MR::setCollisionMtx(this);
    }
}

void LiveActor::calcAnmMtx()
{
    if (this->mModelManager != 0)
    {
        J3DModel* model = MR::getJ3DModel(this);
        model->setBaseScale((Vec &)mScale);

        this->calcAndSetBaseMtx();
        this->mModelManager->calcAnim();
    }
}

// for some reason, the compiler emits "cr1" for this specific function
// todo -- figure out why
void LiveActor::calcViewAndEntry()
{
    if (this->mFlags.mIsNoCalcView)
    {
        if (this->mModelManager != 0)
        {
            this->mModelManager->calcView();
        }
    }
}

void LiveActor::setNerve(const Nerve *nerve)
{
    this->mSpine->setNerve(nerve);
}