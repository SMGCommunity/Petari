#include "Game/Enemy/Kabokuri.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/KabokuriFireHolder.hpp"
#include "Game/Enemy/Kuribo.hpp"
#include "Game/Enemy/WalkerStateBindStarPointer.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/ModelObj.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorStateUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"

namespace {
    static const f32 sBodyHitRadius = 100.0f;
    static const f32 sBodyHitYOffset = 100.0f;
    static const f32 sAttackHitRadius = 70.0f;
    static const f32 sAttackHitYOffset = 70.0f;
    static const f32 sBinderRadius = 90.0f;
    static const f32 sShadowRadius = 90.0f;
    static const f32 sFrontVecBlendRate = 0.2f;
    static const f32 sUpVecBlendRate = 0.1f;
    static const f32 sNormalFreq = 0.9f;
    static const f32 sNormalGravity = 0.2f;
    static const f32 sRailCoordStepInterval = 50.0f;
    static const s32 sWaitTime = 60;
    static const f32 sWalkSpeed = 0.2f;
    static const f32 sWalkTurnDegree = 2.0f;
    static const f32 sWalkGoalRange = 25.0f;
    static const s32 sHitAttackedMinTime = 1;
    static const s32 sHitAttackedMaxTime = 1;
    static const f32 sHitAttackedAccelH = 15.0f;
    static const f32 sHitAttackedAccelV = 30.0f;
    static const f32 sHitAttackedGravity = 1.6f;
    static const f32 sHitAttackedFreq = 0.99f;
};  // namespace

namespace NrvKabokuri {
    NEW_NERVE(KabokuriNrvWait, Kabokuri, Wait);
    NEW_NERVE(KabokuriNrvWalk, Kabokuri, Walk);
    NEW_NERVE(KabokuriNrvDropFire, Kabokuri, DropFire);
    NEW_NERVE(KabokuriNrvAttacksuccess, Kabokuri, Attacksuccess);
    NEW_NERVE(KabokuriNrvTrampled, Kabokuri, Trampled);
    NEW_NERVE(KabokuriNrvStarPieceHitted, Kabokuri, StarPieceHitted);
    NEW_NERVE_ONEND(KabokuriNrvBindStarPointer, Kabokuri, BindStarPointer, BindStarPointer);
    NEW_NERVE(KabokuriNrvHitAttacked, Kabokuri, HitAttacked);
    NEW_NERVE(KabokuriNrvHipDropped, Kabokuri, HipDropped);
    NEW_NERVE(KabokuriNrvBreak, Kabokuri, Break);
};  // namespace NrvKabokuri

Kabokuri::Kabokuri(const char* pName)
    : LiveActor(pName), mKuribo(), mBreakModel(), mAnimeScale(), mStateBindStartPointer(), mRotationQuat(0.0f, 0.0f, 0.0f, 1.0f),
      mFrontVec(0.0f, 0.0f, 1.0f), _B8(-1), mIsValidDropFire() {
    KabokuriFireHolderFunc::createHolder();
}

void Kabokuri::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initRailRider(rIter);
    MR::moveCoordAndTransToNearestRailPos(this);
    MR::makeQuatAndFrontFromRotate(&mRotationQuat, &mFrontVec, this);
    initModelManagerWithAnm("Kabokuri", nullptr, false);
    MR::connectToSceneEnemy(this);
    MR::initLightCtrl(this);
    MR::getJMapInfoArg0WithInit(rIter, &mIsFloating);

    if (!mIsFloating) {
        MR::onCalcGravity(this);
        MR::calcGravity(this);
    } else {
        MR::offCalcGravity(this);

        TVec3f upVec;
        mRotationQuat.getYDir(upVec);
        mGravity.set(-upVec);
    }

    initBinder(::sBinderRadius * mScale.y, ::sBinderRadius * mScale.y, 0);
    initEffectKeeper(1, nullptr, false);
    initSound(2, false);
    initSensor();
    MR::initShadowVolumeSphere(this, ::sShadowRadius);
    initNerve(&NrvKabokuri::KabokuriNrvWait::sInstance);
    MR::initStarPointerTarget(this, 90.0f * mScale.y, TVec3f(0.0f, 90.0f * mScale.y, 0.0f));

    mAnimeScale = new AnimScaleController(nullptr);
    mAnimeScale->setParamTight();

    mStateBindStartPointer = new WalkerStateBindStarPointer(this, mAnimeScale);

    // "Kabokuri kuribo"
    mKuribo = new Kuribo("カボクリクリボー");

    if (mIsFloating) {
        mKuribo->onNoGravitySupport();
    }

    mKuribo->initWithoutIter();
    mKuribo->makeActorDead();

    // "Broken model"
    mBreakModel = MR::createModelObjMapObjStrongLight("壊れモデル", "KabokuriBreak", getBaseMtx());
    mBreakModel->kill();

    MR::useStageSwitchWriteDead(this, rIter);

    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
        makeActorDead();
    } else {
        makeActorAppeared();
    }
}

void Kabokuri::initSensor() {
    f32 scale = mScale.y;

    initHitSensor(2);

    MR::addHitSensorEnemy(this, "body", 8, ::sBodyHitRadius * scale, TVec3f(0.0f, ::sBodyHitYOffset * scale, 0.0f));
    MR::addHitSensorEnemyAttack(this, "attack", 8, ::sAttackHitRadius * scale, TVec3f(0.0f, ::sAttackHitYOffset * scale, 0.0f));
}

void Kabokuri::control() {
    mAnimeScale->updateNerve();

    if (mIsFloating) {
        MR::calcGravityOrZero(this);
    }

    updatePose();
    tryPointBind();
}

void Kabokuri::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, mRotationQuat);
    MR::setBaseScale(this, mAnimeScale->_C * mScale);
}

void Kabokuri::updatePose() {
    mFrontVec.orthogonalize(mGravity);

    if (MR::isNearZero(mFrontVec)) {
        mRotationQuat.getZDir(mFrontVec);
    } else {
        MR::normalize(&mFrontVec);
    }

    MR::blendQuatUpFront(&mRotationQuat, MR::isBindedGround(this) ? *MR::getGroundNormal(this) : -mGravity, mFrontVec, ::sUpVecBlendRate,
                         ::sFrontVecBlendRate);
}

void Kabokuri::addVelocityBase() {
    if (MR::isBindedGround(this)) {
        MR::attenuateVelocity(this, ::sNormalFreq);
    } else {
        MR::addVelocityToGravity(this, ::sNormalGravity);
        MR::attenuateVelocity(this, ::sNormalFreq);
    }

    MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);
}

void Kabokuri::addVelocityToRailPoint(f32 speed) {
    if (MR::isRailReachedHorizonCurrentPos(this, ::sRailCoordStepInterval) && !mIsValidDropFire) {
        MR::moveCoord(this, ::sRailCoordStepInterval);

        s32 nextRailPoint = -1;
        MR::getNextRailPointArg0WithInit(this, &nextRailPoint);

        if (nextRailPoint != 0 && MR::isRailReachedNearNextPoint(this, ::sRailCoordStepInterval)) {
            MR::moveCoordToRailPoint(this, MR::getNextRailPointNo(this));
            mIsValidDropFire = true;
        }

        if (MR::isRailReachedEdge(this)) {
            MR::reverseRailDirection(this);
        }
    }

    TVec3f velocity;
    MR::calcVelocityMoveToTarget(&velocity, this, MR::getRailPos(this), 1.0f);
    if (velocity.dot(mFrontVec) > 0.4f) {
        mVelocity.add(velocity * speed);
    }
}

void Kabokuri::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorEnemyAttack(pSender)) {
        if (!MR::isSensorPlayer(pReceiver)) {
            return;
        }

        if (!isEnableAttack()) {
            return;
        }

        if (!MR::isPlayerHipDropFalling() && MR::sendMsgEnemyAttack(pReceiver, pSender)) {
            setNerve(&NrvKabokuri::KabokuriNrvAttacksuccess::sInstance);
            return;
        }

        MR::sendMsgPush(pReceiver, pSender);
        return;
    }

    if ((MR::isSensorPlayer(pReceiver) && !isEnableAttack() || MR::isSensorEnemy(pReceiver)) && isEnablePush()) {
        MR::sendMsgPush(pReceiver, pSender);
    }
}

bool Kabokuri::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgLockOnStarPieceShoot(msg)) {
        return true;
    }

    if (MR::isMsgStarPieceAttack(msg)) {
        return requestStarPieceHitted();
    }

    if (MR::isMsgPlayerTrample(msg)) {
        return requestTrampled();
    }

    if (MR::isMsgPlayerHitAll(msg)) {
        return requestHitAttacked(pSender, pReceiver);
    }

    if (MR::isMsgPlayerHipDrop(msg)) {
        return requestHipDropped();
    }

    return false;
}

bool Kabokuri::requestTrampled() {
    if (isEnableTrampled()) {
        setNerve(&NrvKabokuri::KabokuriNrvTrampled::sInstance);
        return true;
    }

    return false;
}

bool Kabokuri::requestStarPieceHitted() {
    if (isEnableTrampled()) {
        MR::forceDeleteEffectAll(this);
        setNerve(&NrvKabokuri::KabokuriNrvStarPieceHitted::sInstance);
        return true;
    }

    return false;
}

bool Kabokuri::requestHipDropped() {
    if (isEnableTrampled()) {
        MR::zeroVelocity(this);
        setNerve(&NrvKabokuri::KabokuriNrvHipDropped::sInstance);
        MR::invalidateHitSensors(this);
        return true;
    }

    return false;
}

bool Kabokuri::requestHitAttacked(HitSensor* pSender, HitSensor* pReceived) {
    if (isEnableTrampled()) {
        MR::setVelocityBlowAttack(this, pSender, pReceived, ::sHitAttackedAccelH, ::sHitAttackedAccelV, 4);
        setNerve(&NrvKabokuri::KabokuriNrvHitAttacked::sInstance);
        MR::invalidateHitSensors(this);
        return true;
    }

    return false;
}

bool Kabokuri::tryPointBind() {
    if (isEnablePointBind() && mStateBindStartPointer->tryStartPointBind()) {
        setNerve(&NrvKabokuri::KabokuriNrvBindStarPointer::sInstance);
        return true;
    }

    return false;
}

void Kabokuri::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Wait");
        MR::zeroVelocity(this);
    }

    if (MR::isGreaterStep(this, ::sWaitTime)) {
        setNerve(&NrvKabokuri::KabokuriNrvWalk::sInstance);
    }
}

void Kabokuri::exeWalk() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Walk");
        mIsValidDropFire = false;
    }

    MR::turnDirectionToTargetDegree(this, &mFrontVec, MR::getRailPos(this), ::sWalkTurnDegree);
    addVelocityToRailPoint(::sWalkSpeed);
    addVelocityBase();

    if (mIsValidDropFire && MR::isRailReachedHorizonCurrentPos(this, ::sWalkGoalRange)) {
        setNerve(&NrvKabokuri::KabokuriNrvDropFire::sInstance);
    }
}

void Kabokuri::exeDropFire() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "DropFire");
        MR::zeroVelocity(this);
    }

    if (MR::isActionEnd(this)) {
        setNerve(&NrvKabokuri::KabokuriNrvWalk::sInstance);
        KabokuriFireHolderFunc::generateFire(mPosition, mGravity, mIsFloating);
    }
}

void Kabokuri::exeAttacksuccess() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Attacksuccess");
        MR::zeroVelocity(this);
    }

    if (MR::isActionEnd(this)) {
        setNerve(&NrvKabokuri::KabokuriNrvWalk::sInstance);
    }
}

void Kabokuri::exeTrampled() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Trampled");
        MR::startSound(this, "SE_EM_KABOKURI_TRAMPLE");
        MR::zeroVelocity(this);
    }

    if (MR::isActionEnd(this)) {
        setNerve(&NrvKabokuri::KabokuriNrvWalk::sInstance);
    }
}

void Kabokuri::exeStarPieceHitted() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "StarPieceHit");
        MR::zeroVelocity(this);
    }

    if (MR::isActionEnd(this)) {
        setNerve(&NrvKabokuri::KabokuriNrvWalk::sInstance);
    }
}

void Kabokuri::exeHipDropped() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_EM_STOMPED_S");
        mKuribo->appearHipDropped(mPosition, mRotationQuat);
        mKuribo->mGravity.set(mGravity);
        MR::invalidateClipping(this);
        setNerve(&NrvKabokuri::KabokuriNrvBreak::sInstance);
    }
}

void Kabokuri::exeHitAttacked() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Wait");

        for (int i = 0; i < 6; i++) {
            MR::turnDirectionToPlayerDegree(this, &mFrontVec, 160.0f);
            updatePose();
        }
    }

    MR::addVelocityToGravity(this, ::sHitAttackedGravity);
    MR::attenuateVelocity(this, ::sHitAttackedFreq);
    MR::reboundVelocityFromCollision(this, 0.9f);

    if (MR::isGreaterStep(this, ::sHitAttackedMaxTime) || (MR::isGreaterStep(this, ::sHitAttackedMinTime) && MR::isBinded(this))) {
        mKuribo->appearBlowed(mPosition, mRotationQuat, mVelocity);
        mKuribo->mGravity.set(mGravity);
        MR::invalidateClipping(this);
        setNerve(&NrvKabokuri::KabokuriNrvBreak::sInstance);
    }
}

void Kabokuri::exeBreak() {
    if (MR::isFirstStep(this)) {
        MR::hideModel(this);
        mBreakModel->appear();
        MR::startAction(mBreakModel, "Break");
        MR::startSound(this, "SE_EM_KABOKURI_BREAK_PUMP");
        MR::startBlowHitSound(this);
        MR::zeroVelocity(this);
    }

    if (MR::isActionEnd(mBreakModel)) {
        kill();
        mBreakModel->kill();
    }
}

bool Kabokuri::isEnableAttack() const {
    if (isNerve(&NrvKabokuri::KabokuriNrvWait::sInstance) || isNerve(&NrvKabokuri::KabokuriNrvWalk::sInstance) ||
        isNerve(&NrvKabokuri::KabokuriNrvDropFire::sInstance)) {
        return true;
    }

    return false;
}

bool Kabokuri::isEnablePointBind() const {
    if (isNerve(&NrvKabokuri::KabokuriNrvWait::sInstance) || isNerve(&NrvKabokuri::KabokuriNrvWalk::sInstance) ||
        isNerve(&NrvKabokuri::KabokuriNrvDropFire::sInstance) || isNerve(&NrvKabokuri::KabokuriNrvAttacksuccess::sInstance)) {
        return true;
    }

    return false;
}

bool Kabokuri::isEnableTrampled() const {
    if (isNerve(&NrvKabokuri::KabokuriNrvWait::sInstance) || isNerve(&NrvKabokuri::KabokuriNrvWalk::sInstance) ||
        isNerve(&NrvKabokuri::KabokuriNrvAttacksuccess::sInstance) || isNerve(&NrvKabokuri::KabokuriNrvDropFire::sInstance) ||
        isNerve(&NrvKabokuri::KabokuriNrvTrampled::sInstance) || isNerve(&NrvKabokuri::KabokuriNrvBindStarPointer::sInstance) ||
        isNerve(&NrvKabokuri::KabokuriNrvStarPieceHitted::sInstance)) {
        return true;
    }

    return false;
}

bool Kabokuri::isEnablePush() const {
    if (isNerve(&NrvKabokuri::KabokuriNrvHipDropped::sInstance) || isNerve(&NrvKabokuri::KabokuriNrvBreak::sInstance)) {
        return false;
    }

    return true;
}

void Kabokuri::endBindStarPointer() const {
    mStateBindStartPointer->kill();
}

void Kabokuri::exeBindStarPointer() {
    MR::updateActorStateAndNextNerve(this, mStateBindStartPointer, &NrvKabokuri::KabokuriNrvWalk::sInstance);
}
