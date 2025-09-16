#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/AllLiveActorGroup.hpp"
#include "Game/LiveActor/Binder.hpp"
#include "Game/LiveActor/ClippingDirector.hpp"
#include "Game/LiveActor/ActorPadAndCameraCtrl.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/RailRider.hpp"
#include "Game/NameObj/NameObjExecuteHolder.hpp"
#include "Game/Util.hpp"

LiveActor::LiveActor(const char* pName) : NameObj(pName), 
    mPosition(0.0f), mRotation(0.0f), mScale(1.0f), mVelocity(0.0f), mGravity(0.0f, -1.0f, 0.0f),
    mModelManager(nullptr), mAnimationKeeper(nullptr), mSpine(nullptr),
    mSensorKeeper(nullptr), mBinder(nullptr), mRailRider(nullptr),
    mEffectKeeper(nullptr), mSoundObject(nullptr) {

        mShadowList = nullptr;
        mCollisionParts = nullptr;
        mStageSwitchCtrl = nullptr;
        mStarPointerTarget = nullptr;
        mActorLightCtrl = nullptr;
        mCameraCtrl = nullptr;

        MR::getAllLiveActorGroup()->registerActor(this);
        MR::getClippingDirector()->registerActor(this);
}

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

void LiveActor::movement() {
    if (mModelManager && !mFlags.mIsStoppedAnim) {
        mModelManager->update();
        if (mAnimationKeeper) {
            mAnimationKeeper->update();
        }
    }
    if (MR::isCalcGravity(this)) {
        MR::calcGravity(this);
    }
    if (mSensorKeeper) {
        mSensorKeeper->doObjCol();
    }
    if (mFlags.mIsDead) return;
    if (mSpine) {
        mSpine->update();
    }
    if (mFlags.mIsDead) return;
    control();
    if (mFlags.mIsDead) return;
    updateBinder();
    if (mEffectKeeper) {
        mEffectKeeper->update();
    }
    if (mCameraCtrl) {
        mCameraCtrl->update();
    }
    if (mActorLightCtrl) {
        MR::updateLightCtrl(this);
    }
    MR::tryUpdateHitSensorsAll(this);
    MR::actorSoundMovement(this);
    MR::requestCalcActorShadow(this);
}

void LiveActor::calcAnim() {
    if (!mFlags.mIsNoCalcAnim) {
        calcAnmMtx();
        if (mCollisionParts) {
            MR::setCollisionMtx(this);
        }
    }
}

void LiveActor::calcAnmMtx() {
    if (mModelManager) {
        MR::getJ3DModel(this)->setBaseScale(mScale);
        calcAndSetBaseMtx();
        mModelManager->calcAnim();
    }
}

void LiveActor::calcViewAndEntry() {
    if (!mFlags.mIsNoCalcView && mModelManager && !mFlags.mIsNoCalcView) {
        mModelManager->calcView();
    }
}

void LiveActor::calcAndSetBaseMtx() {
    if (MR::getTaken(this)) {
        MR::setBaseTRMtx(this, MR::getTaken(this)->mActor->getTakingMtx());
    } else {
        TPos3f mtx;
        float zero = 0.0;
        if (zero == mRotation.x && zero == mRotation.z) {
            MR::makeMtxTransRotateY(mtx, this);
        } else {
            MR::makeMtxTR(mtx, this);
        }
        MR::setBaseTRMtx(this, mtx);
    }
}

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

    return MR::getJ3DModel(this)->mBaseTransformMtx;
}

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

void LiveActor::setValidClipping(bool a) {
    mFlags.mIsClipped = a;
}

void LiveActor::endClipped() {
    setValidClipping(false);

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
