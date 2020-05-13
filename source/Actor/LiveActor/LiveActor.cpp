#include "Actor/LiveActor/LiveActor.h"
#include "Actor/LiveActor/AllLiveActorGroup.h"
#include "Actor/NameObj/NameObjExecuteHolder.h"
#include "Actor/Shadow/ShadowController.h"
#include "MR/actor/ActorMovementUtil.h"
#include "MR/actor/ActorSensorUtil.h"
#include "MR/ClippingUtil.h"
#include "MR/actor/LiveActorUtil.h"
#include "MR/MemoryUtil.h"
#include "MR/ModelUtil.h"
#include "MR/SoundUtil.h"

LiveActor::LiveActor(const char *pName) : NameObj(pName) 
{
    mTranslation.x = 0.0f;
    mTranslation.y = 0.0f;
    mTranslation.z = 0.0f;

    mRotation.x = 0.0f;
    mRotation.y = 0.0f;
    mRotation.z = 0.0f;

    mScale.x = 1.0f;
    mScale.y = 1.0f;
    mScale.z = 1.0f;

    mVelocity.x = 0.0f;
    mVelocity.y = 0.0f;
    mVelocity.z = 0.0f;

    mGravity.x = 0.0f;
    mGravity.y = -1.0f;
    mGravity.z = 0.0f;

    mModelManager = 0;
    mAnimKeeper = 0;
    mSpine = 0;
    mSensorKeeper = 0;
    mBinder = 0;
    mRailRider = 0;
    mEffectKeeper = 0;
    mSoundObj = 0;

    LiveActorFlag mFlags();

    mShadowController = 0;
    _78 = 0;
    mStageSwitchCtrl = 0;
    mPointerTarget = 0;
    mLightCtrl = 0;
    mCameraCtrl = 0;

    // register our actor to the "live" actor group, since each actor, when spawned, is technically "alive"
    AllLiveActorGroup* pGroup = MR::getAllLiveActorGroup();
    pGroup->registerActor(this);

    // register our actor to the clipping director for drawing
    ClippingDirector* pDirector = MR::getClippingDirector();
    pDirector->registerActor(this);
}

// nullsub
void LiveActor::init(const JMapInfoIter &iter)  
{ 
    return;
}

void LiveActor::appear() 
{
    makeActorAppeared();
}

void LiveActor::makeActorAppeared() 
{
    // if there is a sensor keeper, validate it for use
    if (mSensorKeeper != 0) 
    {
        mSensorKeeper->validateBySystem();
    }

    // if the actor is currently clipped, end it
    if (MR::isClipped(this)) 
    {
        endClipped();
    }

    // validate our collision parts, and mark our actor as alive
    u32* colParts = _78;

    mFlags.mIsDead = 0;

    if (colParts != 0) 
    {
        MR::validateCollisionParts(this);
    }

    // reset to default position
    MR::resetPosition(this);

    // reset the light control
    if (mLightCtrl != 0) 
    {
        mLightCtrl->reset();
    }

    // update hit sensors to our new position, add to clipping targets, and connect to the scene
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
    makeActorDead();
}

void LiveActor::makeActorDead() 
{
    mVelocity.z = 0.0f;
    mVelocity.y = 0.0f;
    mVelocity.x = 0.0f;

    // clear all of our hit sensors
    MR::clearHitSensors(this);

    // invalidate our sensor, binder, and effect keepers
    if (mSensorKeeper != 0) 
    {
        mSensorKeeper->invalidateBySystem();
    }

    if (mBinder != 0) 
    {
        mBinder->clear();
    }

    if (mEffectKeeper != 0) 
    {
        mEffectKeeper->clear();
    }

    // remove collision
    if (_78 != 0) 
    {
        MR::invalidateCollisionParts(this);
    }

    // mark our actor as dead
    mFlags.mIsDead = 1;

    // remove from clipping targets, and remove from the scene
    // we don't remove this fully as the game can spawn the actor again
    MR::removeFromClippingTarget(this);
    MR::disconnectToSceneTemporarily(this);
    MR::disconnectToDrawTemporarily(this);
}

void LiveActor::movement()
{
    // if we have a model manager, update our model and the animation keeper
    if (mModelManager != 0)
    {
        if (mFlags.mIsNotReleasedAnimFrame)
        {
            mModelManager->update();

            if (mAnimKeeper != 0)
            {
                mAnimKeeper->update();
            }
        }
    }

    // if we need to calculate the new gravity, do so
    if (MR::isCalcGravity(this))
    {
        MR::calcGravity(this);
    }

    if (mSensorKeeper != 0)
    {
        mSensorKeeper->doObjCol();
    }

    if (!mFlags.mIsDead)
    {
        if (mSpine != 0)
        {
            mSpine->update();
        }

        if (!mFlags.mIsDead)
        {
            control();

            if (!mFlags.mIsDead)
            {
                updateBinder();

                if (mEffectKeeper != 0)
                {
                    mEffectKeeper->update();
                }

                if (mCameraCtrl != 0)
                {
                    mCameraCtrl->update();
                }

                if (mLightCtrl != 0)
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
    if (!mFlags.mIsOnCalcAnim)
    {
        calcAnmMtx();

        if (_78 != 0)
        {
            MR::setCollisionMtx(this);
        }
    }
}

void LiveActor::calcAnmMtx()
{
    if (mModelManager != 0)
    {
        J3DModel* model = MR::getJ3DModel(this);
        model->setBaseScale((Vec &)mScale);

        calcAndSetBaseMtx();
        mModelManager->calcAnim();
    }
}

// for some reason, the compiler emits "cr1" for this specific function
// todo -- figure out why
void LiveActor::calcViewAndEntry()
{
    u8 cond = mFlags.mIsNoCalcView != 0;

    if (cond)
    {
        return;
    }

    if (mModelManager != 0)
    {
        mModelManager->calcView();
    }
}

s32 LiveActor::receiveMessage(u32 msg, HitSensor *pTaking, HitSensor *pTaken)
{
    if (msg == 0x29)
    {
        return receiveMsgPush(pTaking, pTaken);
    }

    // this flag is used to describe certain messages
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
        return receiveMsgPlayerAttack(msg, pTaking, pTaken);
    }

    flag = 0;

    if (flag < 0x65)
    {
        flag = 1;
    }

    if (flag != 0)
    {
        return receiveMsgEnemyAttack(msg, pTaking, pTaken);
    }

    if (msg == 0x1E)
    {
        return receiveMsgTake(pTaking, pTaken);
    }

    if (msg == 0x1F)
    {
        return receiveMsgTaken(pTaking, pTaken);
    }

    if (msg == 0x22)
    {
        return receiveMsgThrow(pTaking, pTaken);
    }

    if (msg == 0x21)
    {
        return receiveMsgApart(pTaking, pTaken);
    }

    return receiveMsgOtherMsg(msg, pTaking, pTaken);
}

void LiveActor::calcAndSetBaseMtx()
{
    if (MR::getTaken(this) != 0)
    {
        HitSensor* pTaken = MR::getTaken(this);
        Mtx* pTakenMtx = pTaken->mParentActor->getBaseMtx();
        MR::setBaseTRMtx(this, *pTakenMtx);
    }
    else
    {
        Mtx mtx;

        if ((mRotation.x == 0.0f) && mRotation.z == 0.0f)
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
    return getBaseMtx();
}

void LiveActor::setNerve(const Nerve *pNerve)
{
    // set the current nerve for the actor state
    mSpine->setNerve(pNerve);
}

bool LiveActor::isNerve(const Nerve *pNerve) const
{
    // todo -- figure out why there are two unneeded instructions here
    return !(pNerve - mSpine->getCurrentNerve());
}

u32 LiveActor::getNerveStep() const
{
    return mSpine->mNerveStep;
}

HitSensor* LiveActor::getSensor(const char *pSensorName) const
{
    if (mSensorKeeper != 0)
    {
        return mSensorKeeper->getSensor(pSensorName);
    }

    return 0;
}

Mtx* LiveActor::getBaseMtx() const
{
    if (MR::getJ3DModel(this) != 0)
    {
        return &MR::getJ3DModel(this)->_24;
    }

    return 0;
}

void LiveActor::startClipped()
{
    bool flag = 1;
    HitSensorKeeper* pKeeper = mSensorKeeper;
    mFlags._7 = flag;

    if (pKeeper != 0)
    {
        mSensorKeeper->invalidateBySystem();
    }

    if (mEffectKeeper != 0)
    {
        mEffectKeeper->stopEmitterOnClipped();
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
    HitSensorKeeper* pKeeper = mSensorKeeper;
    mFlags._7 = flag;

    if (pKeeper != 0)
    {
        mSensorKeeper->validateBySystem();
        MR::updateHitSensorsAll(this);
    }

    if (mEffectKeeper != 0)
    {
        mEffectKeeper->playEmitterOffClipped();
    }

    MR::connectToSceneTemporarily(this);

    if (MR::isNoEntryDrawBuffer(this) == 0)
    {
        MR::connectToDrawTemporarily(this);
    }
}

void LiveActor::initModelManagerWithAnm(const char *pModelName, const char *p2, bool a3)
{
    ModelManager* pManager = new ModelManager();
    mModelManager = pManager;
    mModelManager->init(pModelName, p2, a3);
    
    J3DModel* pModel = MR::getJ3DModel(this);
    pModel->setBaseScale((Vec&)mScale);
    calcAndSetBaseMtx();
    MR::calcJ3DModel(this);

    mAnimKeeper = ActorAnimKeeper::tryCreate(this);
    mCameraCtrl = ActorPadAndCameraCtrl::tryCreate(mModelManager, &mTranslation);
}

void LiveActor::initNerve(const Nerve *pNerve)
{
    mSpine = new Spine(this, pNerve);
}

void LiveActor::initHitSensor(s32 numSensors)
{
    mSensorKeeper = new HitSensorKeeper(numSensors);
}

void LiveActor::initBinder(f32 a1, f32 a2, u32 a3)
{
    Mtx* pBaseMtx = getBaseMtx();
    mBinder = new Binder(pBaseMtx, &mTranslation, &mGravity, a1, a2, a3);
    MR::onBind(this);

    if (mEffectKeeper != 0)
        mEffectKeeper->setBinder(mBinder);
}

void LiveActor::initRailRider(const JMapInfoIter &iter)
{
    mRailRider = new RailRider(iter);
}

void LiveActor::initEffectKeeper(s32 effectCount, const char *pResName, bool enableSort)
{
    const char* pObjName = mName;
    ResourceHolder* holder = MR::getModelResourceHolder(this);
    EffectKeeper* pEffectKeeper = new EffectKeeper(pObjName, holder, effectCount, pResName);
    mEffectKeeper = pEffectKeeper;

    if (enableSort)
        pEffectKeeper->enableSort();
    
    mEffectKeeper->init(this);

    if (mBinder != 0)
        mEffectKeeper->setBinder(mBinder);
}

void LiveActor::initSound(s32 a1, bool ignoreActorPosition)
{
    if (ignoreActorPosition != 0)
    {
        AudAnmSoundObject* pSoundObj = new AudAnmSoundObject(&mTranslation, ignoreActorPosition, MR::getCurrentHeap());
        mSoundObj = pSoundObj;
    }
    else
    {
        AudAnmSoundObject* pSoundObj = new AudAnmSoundObject(0, ignoreActorPosition, MR::getCurrentHeap());
        mSoundObj = pSoundObj;
    }
}

void LiveActor::initShadowControllerList(u32 listNum)
{
    mShadowController = new ShadowControllerList(this, listNum);
}

void LiveActor::initStageSwitch(const JMapInfoIter &iter)
{
    mStageSwitchCtrl = MR::createStageSwitchCtrl(this, iter);
}

void LiveActor::initActorStarPointerTarget(f32 a1, const JGeometry::TVec3<f32> *p2, Mtx *p3, JGeometry::TVec3<f32> a4)
{
    mPointerTarget = new StarPointerTarget(a1, p2, p3, a4);
}

void LiveActor::initActorLightCtrl()
{
    mLightCtrl = new ActorLightCtrl(this);
}

void LiveActor::attackSensor(HitSensor *pTaking, HitSensor *pTaken)
{
    return;
}

u32 LiveActor::receiveMsgApart(HitSensor *pTaking, HitSensor *pTaken)
{
    MR::setHitSensorApart(pTaking, pTaken);
    return 1;
}

void LiveActor::addToSoundObjHolder()
{
    mSoundObj->addToSoundObjHolder();
}

void LiveActor::updateBinder()
{
    if (mBinder == 0)
        mTranslation += mGravity;
    else
    {
        if (mFlags.mIsOnBind != 0)
        {
            mTranslation += mGravity;
            mBinder->clear();
        }
        else
        {
            JGeometry::TVec3<f32> what;
            Binder::bind(what, mBinder, mGravity);
            mTranslation += what;
        }
    }  
}