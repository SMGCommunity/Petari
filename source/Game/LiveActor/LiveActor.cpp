#include "Game/LiveActor/LiveActor.h"
#include "Game/LiveActor/Binder.h"
#include "Game/LiveActor/ClippingDirector.h"
#include "Game/LiveActor/ActorPadAndCameraCtrl.h"
#include "Game/NameObj/NameObjExecuteHolder.h"
#include "Game/Util.h"

void LiveActor::init(const JMapInfoIter &) {

} 

void LiveActor::appear() {
    makeActorAppeared();
}

void LiveActor::makeActorAppeared() {
    if (mSensorKeeper) {
        mSensorKeeper->validateBySystem();
    }

    if (MR::isClipped(this)) {
        endClipped();
    }

    mFlags.mIsDead = false;
    if (mCollisionParts) {
        MR::validateCollisionParts(this);
    }

    MR::resetPosition(this);
    if (mActorLightCtrl) {
        mActorLightCtrl->reset();
    }

    MR::tryUpdateHitSensorsAll(this);
    MR::addToClippingTarget(this);
    MR::connectToSceneTemporarily(this);
    
    if (!MR::isNoEntryDrawBuffer(this)) {
        MR::connectToDrawTemporarily(this);
    }
}

void LiveActor::kill() {
    makeActorDead();
}

#ifdef NON_MATCHING
void LiveActor::makeActorDead() {
    mVelocity.z = 0.0f;
    mVelocity.y = 0.0f;
    mVelocity.x = 0.0f;
    MR::clearHitSensors(this);

    if (mSensorKeeper) {
        mSensorKeeper->invalidateBySystem();
    }

    if (mBinder) {
        mBinder->clear();
    }

    if (mEffectKeeper) {
        mEffectKeeper->clear();
    }

    if (mCollisionParts) {
        MR::invalidateCollisionParts(this);
    }

    mFlags.mIsDead = true;
    MR::removeFromClippingTarget(this);
    MR::disconnectToSceneTemporarily(this);
    MR::disconnectToDrawTemporarily(this);
}
#endif

MtxPtr LiveActor::getTakingMtx() const {
    return getBaseMtx();
}

void LiveActor::setNerve(const Nerve *pNerve) {
    mSpine->setNerve(pNerve);
}

bool LiveActor::isNerve(const Nerve *pNerve) const {
    return mSpine->getCurrentNerve() == pNerve;
}

s32 LiveActor::getNerveStep() const {
    return mSpine->mStep;
}
 
HitSensor* LiveActor::getSensor(const char *pSensorName) const {
    if (mSensorKeeper) {
        return mSensorKeeper->getSensor(pSensorName);
    }

    return 0;
}

MtxPtr LiveActor::getBaseMtx() const {
    if (!MR::getJ3DModel(this)) {
        return 0;
    }

    return MR::getJ3DModel(this)->_24;
}

#ifdef NON_MATCHING
// prologue refuses to schedule right for both of these
void LiveActor::startClipped() {
    HitSensorKeeper* keeper = mSensorKeeper;

    mFlags.onClipped();

    if (keeper != 0) {
        keeper->invalidateBySystem();
    }

    EffectKeeper* effectKeeper = mEffectKeeper;
    if (effectKeeper) {
        effectKeeper->stopEmitterOnClipped();
    }

    MR::disconnectToSceneTemporarily(this);
     
    if (!MR::isNoEntryDrawBuffer(this)) {
        MR::disconnectToDrawTemporarily(this);
    }
}

void LiveActor::endClipped() {
    mFlags.mIsClipped = false;

    if (mSensorKeeper) {
        mSensorKeeper->validateBySystem();
        MR::updateHitSensorsAll(this);
    }

    if (mEffectKeeper) {
        mEffectKeeper->playEmitterOffClipped();
    }

    MR::connectToSceneTemporarily(this);

    if (!MR::isNoEntryDrawBuffer(this)) {
        MR::connectToDrawTemporarily(this);
    }
}
#endif

void LiveActor::initModelManagerWithAnm(const char *pName, const char *a2, bool a3) {
    mModelManager = new ModelManager();
    mModelManager->init(pName, a2, a3);
    MR::getJ3DModel(this)->setBaseScale(reinterpret_cast<const Vec &>(mScale));
    LiveActor::calcAndSetBaseMtx();
    MR::calcJ3DModel(this);
    mAnimationKeeper = ActorAnimKeeper::tryCreate(this);
    mCameraCtrl = ActorPadAndCameraCtrl::tryCreate(mModelManager, &mPosition);
}

void LiveActor::initNerve(const Nerve *pNerve) {
    mSpine = new Spine(this, pNerve);
}

void LiveActor::initHitSensor(int numSensors) {
    mSensorKeeper = new HitSensorKeeper(numSensors);
}

// LiveActor::initBinder

void LiveActor::initRailRider(const JMapInfoIter &rIter) {
    mRailRider = new RailRider(rIter);
}

#ifdef NON_MATCHING
void LiveActor::initEffectKeeper(int a1, const char *a2, bool doSort) {
    EffectKeeper* keeper = new EffectKeeper(mName, MR::getModelResourceHolder(this), a1, a2);
    mEffectKeeper = keeper;

    if (doSort) {
        keeper->enableSort();
    }

    keeper->init(this);

    if (mBinder) {
        keeper->setBinder(mBinder);
    }
}
#endif

// LiveActor::initSound

void LiveActor::initShadowControllerList(u32 numShadows) {
    mShadowList = new ShadowControllerList(this, numShadows);
}

// LiveActor::initActorCollisionParts

void LiveActor::initStageSwitch(const JMapInfoIter &rIter) {
    mStageSwitchCtrl = MR::createStageSwitchCtrl(this, rIter);
}

// LiveActor::initActorStarPointerTarget

void LiveActor::initActorLightCtrl() {
    mActorLightCtrl = new ActorLightCtrl(this);
}

void LiveActor::attackSensor(HitSensor *, HitSensor *) {

}

u32 LiveActor::receiveMsgApart(HitSensor *pSensor_0, HitSensor *pSensor_1) {
    MR::setHitSensorApart(pSensor_0, pSensor_1);
    return 1;
}