#include "Actor/LiveActor/LiveActor.h"
#include "Actor/LiveActor/AllLiveActorGroup.h"
#include "Actor/Clipping/ClippingDirector.h"
#include "Actor/NameObj/NameObjExecuteHolder.h"
#include "Actor/Shadow/ShadowController.h"
#include "MR/actor/ActorMovementUtil.h"
#include "MR/actor/ActorSensorUtil.h"
#include "MR/actor/LiveActorUtil.h"
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

s32 LiveActor::receiveMessage(u32 msg, HitSensor *taking, HitSensor *taken)
{
    if (msg == 0x29)
    {
        return this->receiveMsgPush(taking, taken);
    }

    s32 flag = 0;

    if (msg != 0)
    {
        if (msg < 0x4A)
        {
            flag = 1;
        }
    }

    if (flag != 0)
    {
        return this->receiveMsgPlayerAttack(msg, taking, taken);
    }

    flag = 0;

    if (flag < 0x65)
    {
        flag = 1;
    }

    if (flag != 0)
    {
        return this->receiveMsgEnemyAttack(msg, taking, taken);
    }

    if (msg == 0x1E)
    {
        return this->receiveMsgTake(taking, taken);
    }

    if (msg == 0x1F)
    {
        return this->receiveMsgTaken(taking, taken);
    }

    if (msg == 0x22)
    {
        return this->receiveMsgThrow(taking, taken);
    }

    if (msg == 0x21)
    {
        return this->receiveMsgApart(taking, taken);
    }

    return this->receiveMsgOtherMsg(msg, taking, taken);
}

void LiveActor::calcAndSetBaseMtx()
{
    if (MR::getTaken(this) != 0)
    {
        HitSensor* taken = MR::getTaken(this);
        Mtx* takenMtx = taken->mActor->getBaseMtx();
        MR::setBaseTRMtx(this, *takenMtx);
    }
    else
    {
        Mtx mtx;

        if ((this->mRotation.x == LiveActor::zero) && this->mRotation.z == LiveActor::zero)
        {
            MR::makeMtxTransRotateY(mtx, this);   
        }
        else
        {
            MR::makeMtxTR(mtx, this);
        }

        // todo -- there's a setmtx call here
    }
    
}

Mtx* LiveActor::getTakingMtx() const
{
    return this->getBaseMtx();
}

void LiveActor::setNerve(const Nerve *nerve)
{
    this->mSpine->setNerve(nerve);
}

// LiveActor::isNerve()

u32 LiveActor::getNerveStep() const
{
    return this->mSpine->mNerveStep;
}

HitSensor* LiveActor::getSensor(const char *sensorName) const
{
    if (this->mSensorKeeper != 0)
    {
        return this->mSensorKeeper->getSensor(sensorName);
    }

    return 0;
}

// LiveActor::getBaseMtx()

void LiveActor::startClipped()
{
    bool flag = 1;
    HitSensorKeeper* keeper = this->mSensorKeeper;
    this->mFlags._7 = flag;

    if (keeper != 0)
    {
        this->mSensorKeeper->invalidateBySystem();
    }

    if (this->mEffectKeeper != 0)
    {
        this->mEffectKeeper->stopEmitterOnClipped();
    }

    MR::disconnectToSceneTemporarily(this);

    if (MR::isNoEntryDrawBuffer(this) == 0)
    {
        MR::disconnectToDrawTemporarily(this);
    }
}

void LiveActor::endClipped()
{
    bool flag = 0;
    HitSensorKeeper* keeper = this->mSensorKeeper;
    this->mFlags._7 = flag;

    if (keeper != 0)
    {
        this->mSensorKeeper->validateBySystem();
        MR::updateHitSensorsAll(this);
    }

    if (this->mEffectKeeper != 0)
    {
        this->mEffectKeeper->playEmitterOffClipped();
    }

    MR::connectToSceneTemporarily(this);

    if (MR::isNoEntryDrawBuffer(this) == 0)
    {
        MR::connectToDrawTemporarily(this);
    }
}

void LiveActor::initModelManagerWithAnm(const char *a1, const char *a2, bool a3)
{
    ModelManager* manager = new ModelManager();
    this->mModelManager = manager;
    mModelManager->init(a1, a2, a3);
    
    J3DModel* model = MR::getJ3DModel(this);
    model->setBaseScale((Vec&)this->mScale);
    this->calcAndSetBaseMtx();
    MR::calcJ3DModel(this);

    this->mAnimKeeper = ActorAnimKeeper::tryCreate(this);
    this->mCameraCtrl = ActorPadAndCameraCtrl::tryCreate(this->mModelManager, &this->mTranslation);
}

void LiveActor::initNerve(const Nerve *nerve)
{
    this->mSpine = new Spine(this, nerve);
}

void LiveActor::initHitSensor(s32 numSensors)
{
    this->mSensorKeeper = new HitSensorKeeper(numSensors);
}

void LiveActor::initBinder(f32 a1, f32 a2, u32 a3)
{
    Mtx* baseMtx = this->getBaseMtx();
    this->mBinder = new Binder(baseMtx, &this->mTranslation, &this->mGravity, a1, a2, a3);
    MR::onBind(this);

    if (this->mEffectKeeper != 0)
    {
        this->mEffectKeeper->setBinder(this->mBinder);
    }
}