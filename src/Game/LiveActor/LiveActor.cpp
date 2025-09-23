#include "Game/AudioLib/AudAnmSoundObject.hpp"
#include "Game/LiveActor/ActorAnimKeeper.hpp"
#include "Game/LiveActor/ActorLightCtrl.hpp"
#include "Game/LiveActor/ActorPadAndCameraCtrl.hpp"
#include "Game/LiveActor/AllLiveActorGroup.hpp"
#include "Game/LiveActor/Binder.hpp"
#include "Game/LiveActor/ClippingDirector.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/RailRider.hpp"
#include "Game/NameObj/NameObjExecuteHolder.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MemoryUtil.hpp"
#include "Game/Util/ModelUtil.hpp"

LiveActor::LiveActor(const char* pName) :
    NameObj(pName),
    mPosition(0.0f, 0.0f, 0.0f),
    mRotation(0.0f, 0.0f, 0.0f),
    mScale(1.0f, 1.0f, 1.0f),
    mVelocity(0.0f, 0.0f, 0.0f),
    mGravity(0.0f, -1.0f, 0.0f),
    mModelManager(nullptr),
    mAnimKeeper(nullptr),
    mSpine(nullptr),
    mSensorKeeper(nullptr),
    mBinder(nullptr),
    mRailRider(nullptr),
    mEffectKeeper(nullptr),
    mSoundObject(nullptr),
    mShadowControllerList(nullptr),
    mCollisionParts(nullptr),
    mStageSwitchCtrl(nullptr),
    mStarPointerTarget(nullptr),
    mActorLightCtrl(nullptr),
    mCameraCtrl(nullptr)
{
    MR::getAllLiveActorGroup()->registerActor(this);
    MR::getClippingDirector()->registerActor(this);
}

void LiveActor::init(const JMapInfoIter& rIter) {
    
}

void LiveActor::appear() {
    makeActorAppeared();
}

void LiveActor::makeActorAppeared() {
    if (mSensorKeeper != nullptr) {
        mSensorKeeper->validateBySystem();
    }

    if (MR::isClipped(this)) {
        endClipped();
    }

    mFlag.mIsDead = false;

    if (mCollisionParts != nullptr) {
        MR::validateCollisionParts(this);
    }

    MR::resetPosition(this);

    if (mActorLightCtrl != nullptr) {
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

// FIXME: `mBinder` should be loaded twice.
void LiveActor::makeActorDead() {
    mVelocity.z = 0.0f;
    mVelocity.y = 0.0f;
    mVelocity.x = 0.0f;

    MR::clearHitSensors(this);

    if (mSensorKeeper != nullptr) {
        mSensorKeeper->invalidateBySystem();
    }

    if (mBinder != nullptr) {
        mBinder->clear();
    }

    if (mEffectKeeper != nullptr) {
        mEffectKeeper->clear();
    }

    if (mCollisionParts != nullptr) {
        MR::invalidateCollisionParts(this);
    }

    mFlag.mIsDead = true;

    MR::removeFromClippingTarget(this);
    MR::disconnectToSceneTemporarily(this);
    MR::disconnectToDrawTemporarily(this);
}

void LiveActor::movement() {
    if (mModelManager != nullptr && !mFlag.mIsStoppedAnim) {
        mModelManager->update();

        if (mAnimKeeper != nullptr) {
            mAnimKeeper->update();
        }
    }

    if (MR::isCalcGravity(this)) {
        MR::calcGravity(this);
    }

    if (mSensorKeeper != nullptr) {
        mSensorKeeper->doObjCol();
    }

    if (mFlag.mIsDead) {
        return;
    }

    if (mSpine != nullptr) {
        mSpine->update();
    }

    if (mFlag.mIsDead) {
        return;
    }

    control();

    if (mFlag.mIsDead) {
        return;
    }

    updateBinder();

    if (mEffectKeeper != nullptr) {
        mEffectKeeper->update();
    }

    if (mCameraCtrl != nullptr) {
        mCameraCtrl->update();
    }

    if (mActorLightCtrl != nullptr) {
        MR::updateLightCtrl(this);
    }

    MR::tryUpdateHitSensorsAll(this);
    MR::actorSoundMovement(this);
    MR::requestCalcActorShadow(this);
}

void LiveActor::calcAnim() {
    if (mFlag.mIsNoCalcAnim) {
        return;
    }

    calcAnmMtx();

    if (mCollisionParts == nullptr) {
        return;
    }

    MR::setCollisionMtx(this);
}

void LiveActor::calcAnmMtx() {
    if (mModelManager == nullptr) {
        return;
    }

    MR::getJ3DModel(this)->setBaseScale(mScale);
    calcAndSetBaseMtx();
    mModelManager->calcAnim();
}

void LiveActor::calcViewAndEntry() {
    if (mFlag.mIsNoCalcView) {
        return;
    }

    if (mModelManager == nullptr) {
        return;
    }

    if (mFlag.mIsNoCalcView) {
        return;
    }

    mModelManager->calcView();
}

bool LiveActor::receiveMessage(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (msg == ACTMES_PUSH) {
        return receiveMsgPush(pSender, pReceiver);
    }

    bool isMsgPlayerAttack = msg > ACTMES_PLAYER_ATTACK_START && msg < ACTMES_PLAYER_ATTACK_END;

    if (isMsgPlayerAttack) {
        return receiveMsgPlayerAttack(msg, pSender, pReceiver);
    }

    bool isMsgEnemyAttack = msg > ACTMES_ENEMY_ATTACK_START && msg < ACTMES_ENEMY_ATTACK_END;

    if (isMsgEnemyAttack) {
        return receiveMsgEnemyAttack(msg, pSender, pReceiver);
    }

    if (msg == ACTMES_TAKE) {
        return receiveMsgTake(pSender, pReceiver);
    }

    if (msg == ACTMES_TAKEN) {
        return receiveMsgTaken(pSender, pReceiver);
    }

    if (msg == ACTMES_THROW) {
        return receiveMsgThrow(pSender, pReceiver);
    }

    if (msg == ACTMES_APART) {
        return receiveMsgApart(pSender, pReceiver);
    }

    return receiveOtherMsg(msg, pSender, pReceiver);
}

void LiveActor::calcAndSetBaseMtx() {
    if (MR::getTaken(this)) {
        MR::setBaseTRMtx(this, MR::getTaken(this)->mActor->getTakingMtx());
    }
    else {
        TPos3f mtx;

        if (mRotation.x == 0.0f && mRotation.z == 0.0f) {
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

void LiveActor::setNerve(const Nerve* pNerve) {
    mSpine->setNerve(pNerve);
}

bool LiveActor::isNerve(const Nerve* pNerve) const {
    return mSpine->getCurrentNerve() == pNerve;
}

s32 LiveActor::getNerveStep() const {
    return mSpine->mStep;
}

HitSensor* LiveActor::getSensor(const char* pSensorName) const {
    if (mSensorKeeper != nullptr) {
        return mSensorKeeper->getSensor(pSensorName);
    }

    return nullptr;
}

MtxPtr LiveActor::getBaseMtx() const {
    if (MR::getJ3DModel(this) != nullptr) {
        return MR::getJ3DModel(this)->mBaseTransformMtx;
    }

    return nullptr;
}

// FIXME: LiveActor::mSensorKeeper should be loaded twice.
void LiveActor::startClipped() {
    mFlag.mIsClipped = true;

    if (mSensorKeeper != nullptr) {
        mSensorKeeper->invalidateBySystem();
    }

    if (mEffectKeeper != nullptr) {
        mEffectKeeper->stopEmitterOnClipped();
    }

    MR::disconnectToSceneTemporarily(this);

    if (MR::isNoEntryDrawBuffer(this)) {
        return;
    }

    MR::disconnectToDrawTemporarily(this);
}

// FIXME: LiveActor::mSensorKeeper should be loaded twice.
void LiveActor::endClipped() {
    mFlag.mIsClipped = false;

    if (mSensorKeeper != nullptr) {
        mSensorKeeper->validateBySystem();
        MR::updateHitSensorsAll(this);
    }

    if (mEffectKeeper != nullptr) {
        mEffectKeeper->playEmitterOffClipped();
    }

    MR::connectToSceneTemporarily(this);

    if (MR::isNoEntryDrawBuffer(this)) {
        return;
    }

    MR::connectToDrawTemporarily(this);
}

void LiveActor::initModelManagerWithAnm(const char* pModelArcName, const char* pAnimArcName, bool a3) {
    mModelManager = new ModelManager();
    mModelManager->init(pModelArcName, pAnimArcName, a3);

    MR::getJ3DModel(this)->setBaseScale(mScale);
    LiveActor::calcAndSetBaseMtx();
    MR::calcJ3DModel(this);

    mAnimKeeper = ActorAnimKeeper::tryCreate(this);
    mCameraCtrl = ActorPadAndCameraCtrl::tryCreate(mModelManager, &mPosition);
}

void LiveActor::initNerve(const Nerve* pNerve) {
    mSpine = new Spine(this, pNerve);
}

void LiveActor::initHitSensor(int numSensors) {
    mSensorKeeper = new HitSensorKeeper(numSensors);
}

void LiveActor::initBinder(f32 a, f32 b, u32 c) {
    mBinder = new Binder(getBaseMtx(), &mPosition, &mGravity, a, b, c);

    MR::onBind(this);

    if (mEffectKeeper != nullptr) {
        mEffectKeeper->setBinder(mBinder);
    }
}

void LiveActor::initRailRider(const JMapInfoIter& rIter) {
    mRailRider = new RailRider(rIter);
}

// FIXME: There should be an extra local variable.
void LiveActor::initEffectKeeper(int a1, const char* a2, bool doSort) {
    mEffectKeeper = new EffectKeeper(mName, MR::getModelResourceHolder(this), a1, a2);

    if (doSort) {
        mEffectKeeper->enableSort();
    }

    mEffectKeeper->init(this);

    if (mBinder != nullptr) {
        mEffectKeeper->setBinder(mBinder);
    }
}

// FIXME: AudAnmSoundObject should be 116 bytes in size.
void LiveActor::initSound(int param1, bool param2) {
    if (!param2) {
        mSoundObject = new AudAnmSoundObject(&mPosition, param1, MR::getCurrentHeap());
    }
    else {
        mSoundObject = new AudAnmSoundObject(nullptr, param1, MR::getCurrentHeap());
    }
}

void LiveActor::initShadowControllerList(u32 numShadows) {
    mShadowControllerList = new ShadowControllerList(this, numShadows);
}

// LiveActor::initActorCollisionParts

void LiveActor::initStageSwitch(const JMapInfoIter& rIter) {
    mStageSwitchCtrl = MR::createStageSwitchCtrl(this, rIter);
}

// LiveActor::initActorStarPointerTarget

void LiveActor::initActorLightCtrl() {
    mActorLightCtrl = new ActorLightCtrl(this);
}

void LiveActor::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    
}

bool LiveActor::receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver) {
    return false;
}

bool LiveActor::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    return false;
}

bool LiveActor::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    return false;
}

bool LiveActor::receiveMsgTake(HitSensor* pSender, HitSensor* pReceiver) {
    return false;
}

bool LiveActor::receiveMsgTaken(HitSensor* pSender, HitSensor* pReceiver) {
    return false;
}

bool LiveActor::receiveMsgThrow(HitSensor* pSender, HitSensor* pReceiver) {
    return false;
}

bool LiveActor::receiveMsgApart(HitSensor* pSender, HitSensor* pReceiver) {
    MR::setHitSensorApart(pSender, pReceiver);

    return true;
}

bool LiveActor::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    return false;
}
