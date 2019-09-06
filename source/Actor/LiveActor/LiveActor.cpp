#include "Actor/LiveActor/LiveActor.h"
#include "Actor/LiveActor/AllLiveActorGroup.h"
#include "Actor/Clipping/ClippingDirector.h"
#include "Actor/NameObj/NameObjExecuteHolder.h"
#include "Actor/Shadow/ShadowController.h"
#include "defines.h"
#include "MR/actor/ActorMovementUtil.h"
#include "MR/actor/ActorSensorUtil.h"
#include "MR/actor/LiveActorUtil.h"
#include "MR/MemoryUtil.h"
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
    this->mRailRider = 0;
    this->mEffectKeeper = 0;
    this->mSoundObj = 0;

    LiveActorFlag flags;
    this->mFlags = flags;

    this->mShadowController = 0;
    this->_78 = 0;
    this->mStageSwitchCtrl = 0;
    this->mPointerTarget = 0;
    this->mLightCtrl = 0;
    this->mCameraCtrl = 0;

    // register our actor to the "live" actor group, since each actor, when spawned, is technically "alive"
    AllLiveActorGroup* group = MR::getAllLiveActorGroup();
    group->registerActor(this);

    // register our actor to the clipping director for drawing
    ClippingDirector* director = MR::getClippingDirector();
    director->registerActor(this);
}

// nullsub
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
    // if there is a sensor keeper, validate it for use
    if (this->mSensorKeeper != 0)
    {
        this->mSensorKeeper->validateBySystem();
    }

    // if the actor is currently clipped, end it
    if (MR::isClipped(this))
    {
        this->endClipped();
    }

    // validate our collision parts, and mark our actor as alive
    u32* colParts = this->_78;
    this->mFlags.mIsDead = 0;

    if (colParts != 0)
    {
        MR::validateCollisionParts(this);
    }

    // reset to default position
    MR::resetPosition(this);

    // reset the light control
    if (this->mLightCtrl != 0)
    {
        this->mLightCtrl->reset();
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
    this->makeActorDead();
}

void LiveActor::makeActorDead()
{
    f32 zero = LiveActor::zero;

    this->mVelocity.z = zero;
    this->mVelocity.y = zero;
    this->mVelocity.x = zero;

    // clear all of our hit sensors
    MR::clearHitSensors(this);

    // invalidate our sensor, binder, and effect keepers
    if (this->mSensorKeeper != 0)
        this->mSensorKeeper->invalidateBySystem();

    if (this->mBinder != 0)
        this->mBinder->clear();

    if (this->mEffectKeeper != 0)
        this->mEffectKeeper->clear();

    // remove collision
    if (this->_78 != 0)
    {
        MR::invalidateCollisionParts(this);
    }

    // mark our actor as dead
    this->mFlags.mIsDead = 1;

    // remove from clipping targets, and remove from the scene
    // we don't remove this fully as the game can spawn the actor again
    MR::removeFromClippingTarget(this);
    MR::disconnectToSceneTemporarily(this);
    MR::disconnectToDrawTemporarily(this);
}

void LiveActor::movement()
{
    // if we have a model manager, update our model and the animation keeper
    if (this->mModelManager != 0)
    {
        if (this->mFlags.mIsNotReleasedAnimFrame)
        {
            this->mModelManager->update();

            if (this->mAnimKeeper != 0)
                this->mAnimKeeper->update();
        }
    }

    // if we need to calculate the new gravity, do so
    if (MR::isCalcGravity(this))
        MR::calcGravity(this);

    if (this->mSensorKeeper != 0)
    {
        this->mSensorKeeper->doObjCol();
    }

    if (!this->mFlags.mIsDead)
    {
        if (this->mSpine != 0)
            this->mSpine->update();

        if (!this->mFlags.mIsDead)
        {
            this->control();

            if (!this->mFlags.mIsDead)
            {
                this->updateBinder();

                if (this->mEffectKeeper != 0)
                    this->mEffectKeeper->update();

                if (this->mCameraCtrl != 0)
                    this->mCameraCtrl->update();

                if (this->mLightCtrl != 0)
                    MR::updateLightCtrl(this);

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
        this->calcAnmMtx();

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
            this->mModelManager->calcView();
    }
}

s32 LiveActor::receiveMessage(u32 msg, HitSensor *taking, HitSensor *taken)
{
    if (msg == 0x29)
        return this->receiveMsgPush(taking, taken);

    // this flag is used to describe certain messages
    s32 flag = 0;

    if (msg != 0)
    {
        if (msg < 0x4A)
            flag = 1;
    }

    if (flag != 0)
        return this->receiveMsgPlayerAttack(msg, taking, taken);

    flag = 0;

    if (flag < 0x65)
        flag = 1;

    if (flag != 0)
        return this->receiveMsgEnemyAttack(msg, taking, taken);

    if (msg == 0x1E)
        return this->receiveMsgTake(taking, taken);

    if (msg == 0x1F)
        return this->receiveMsgTaken(taking, taken);

    if (msg == 0x22)
        return this->receiveMsgThrow(taking, taken);

    if (msg == 0x21)
        return this->receiveMsgApart(taking, taken);

    return this->receiveMsgOtherMsg(msg, taking, taken);
}

void LiveActor::calcAndSetBaseMtx()
{
    if (MR::getTaken(this) != 0)
    {
        HitSensor* taken = MR::getTaken(this);
        Mtx* takenMtx = taken->mParentActor->getBaseMtx();
        MR::setBaseTRMtx(this, *takenMtx);
    }
    else
    {
        Mtx mtx;

        if ((this->mRotation.x == LiveActor::zero) && this->mRotation.z == LiveActor::zero)
            MR::makeMtxTransRotateY(mtx, this);   
        else
            MR::makeMtxTR(mtx, this);

        // todo -- there's a setmtx call here
    }
    
}

Mtx* LiveActor::getTakingMtx() const
{
    return this->getBaseMtx();
}

void LiveActor::setNerve(const Nerve *nerve)
{
    // set the current nerve for the actor state
    this->mSpine->setNerve(nerve);
}

bool LiveActor::isNerve(const Nerve *nerve) const
{
    // this adds some weird instructions
    const Nerve* curNerve = this->mSpine->getCurrentNerve();
    s32 what = curNerve - nerve;
    s32 blah = __cntlzw(what);
    return blah >> 5;
}

u32 LiveActor::getNerveStep() const
{
    return this->mSpine->mNerveStep;
}

HitSensor* LiveActor::getSensor(const char *sensorName) const
{
    if (this->mSensorKeeper != 0)
        return this->mSensorKeeper->getSensor(sensorName);

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
    HitSensorKeeper* keeper = this->mSensorKeeper;
    this->mFlags._7 = flag;

    if (keeper != 0)
        this->mSensorKeeper->invalidateBySystem();

    if (this->mEffectKeeper != 0)
        this->mEffectKeeper->stopEmitterOnClipped();

    MR::disconnectToSceneTemporarily(this);

    if (MR::isNoEntryDrawBuffer(this) == 0)
        MR::disconnectToDrawTemporarily(this);
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
        this->mEffectKeeper->playEmitterOffClipped();

    MR::connectToSceneTemporarily(this);

    if (MR::isNoEntryDrawBuffer(this) == 0)
        MR::connectToDrawTemporarily(this);
}

void LiveActor::initModelManagerWithAnm(const char *modelName, const char *a2, bool a3)
{
    ModelManager* manager = new ModelManager();
    this->mModelManager = manager;
    mModelManager->init(modelName, a2, a3);
    
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
        this->mEffectKeeper->setBinder(this->mBinder);
}

void LiveActor::initRailRider(const JMapInfoIter &iter)
{
    this->mRailRider = new RailRider(iter);
}

void LiveActor::initEffectKeeper(s32 effectCount, const char *resName, bool enableSort)
{
    const char* objName = this->mName;
    ResourceHolder* holder = MR::getModelResourceHolder(this);
    EffectKeeper* effectKeeper = new EffectKeeper(objName, holder, effectCount, resName);
    this->mEffectKeeper = effectKeeper;

    if (enableSort)
        effectKeeper->enableSort();
    
    this->mEffectKeeper->init(this);

    if (this->mBinder != 0)
        this->mEffectKeeper->setBinder(this->mBinder);
}

void LiveActor::initSound(s32 a1, bool ignoreActorPosition)
{
    if (ignoreActorPosition != 0)
    {
        AudAnmSoundObject* soundObj = new AudAnmSoundObject(&this->mTranslation, ignoreActorPosition, MR::getCurrentHeap());
        this->mSoundObj = soundObj;
    }
    else
    {
        AudAnmSoundObject* soundObj = new AudAnmSoundObject(0, ignoreActorPosition, MR::getCurrentHeap());
        this->mSoundObj = soundObj;
    }
}

void LiveActor::initShadowControllerList(u32 listNum)
{
    this->mShadowController = new ShadowControllerList(this, listNum);
}

void LiveActor::initStageSwitch(const JMapInfoIter &iter)
{
    this->mStageSwitchCtrl = MR::createStageSwitchCtrl(this, iter);
}

void LiveActor::initActorStarPointerTarget(f32 a1, const JGeometry::TVec3<f32> *a2, Mtx *a3, JGeometry::TVec3<f32> a4)
{
    this->mPointerTarget = new StarPointerTarget(a1, a2, a3, a4);
}

void LiveActor::initActorLightCtrl()
{
    this->mLightCtrl = new ActorLightCtrl(this);
}

void LiveActor::attackSensor(HitSensor* taking, HitSensor* taken)
{
    return;
}

u32 LiveActor::receiveMsgApart(HitSensor* taking, HitSensor* taken)
{
    MR::setHitSensorApart(taking, taken);
    return 1;
}

void LiveActor::addToSoundObjHolder()
{
    this->mSoundObj->addToSoundObjHolder();
}

void LiveActor::updateBinder()
{
    if (this->mBinder == 0)
        this->mTranslation += this->mGravity;
    else
    {
        if (this->mFlags.mIsOnBind != 0)
        {
            this->mTranslation += this->mGravity;
            this->mBinder->clear();
        }
        else
        {
            JGeometry::TVec3<f32> what;
            Binder::bind(what, this->mBinder, this->mGravity);
            this->mTranslation += what;
        }
    }  
}