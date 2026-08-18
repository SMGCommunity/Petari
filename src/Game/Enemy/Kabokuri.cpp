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
#include "revolution/types.h"

namespace {
    static const f32 sBodyHitRadius = 100.0f;
    static const f32 sBodyHitYOffset = 100.0f;
    static const f32 sAttackHitRadius = 70.0f;
    static const f32 sAttackHitYOffset = 70.0f;
    static const f32 sBinderRadius = 90.0f;
    static const f32 sShadowRadius = 90.0f;
    static const f32 sFrontVecBlendRate = 0.2f;
    static const f32 sUpVecBlendRate = 0.1f;
    static const f32 sNormalGravity = 0.2f;
    static const f32 sRailCoordStepInterval = 50.0f;
    static const u32 sWaitTime = 60;
    static const f32 sWalkSpeed = 0.2f;
    static const f32 sWalkTurnDegree = 2.0f;
    static const f32 sWalkGoalRange = 25.0f;
    static const f32 sHitAttackedAccelH = 15.0f;
    static const f32 sHitAttackedAccelV = 30.0f;
    static const f32 sHitAttackedGravity = 1.6f;
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
    : LiveActor(pName), mKuribo(nullptr), _90(nullptr), mAnimeScale(nullptr), _98(nullptr), _9C(0.0f, 0.0f, 0.0f, 1.0f), mFrontVec(0.0f, 0.0f, 1.0f),
      _B8(-1), _BD(0) {
    KabokuriFireHolderFunc::createHolder();
}

void Kabokuri::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initRailRider(rIter);
    MR::moveCoordAndTransToNearestRailPos(this);
    MR::makeQuatAndFrontFromRotate(&_9C, &mFrontVec, this);
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
        _9C.getYDir(upVec);
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

    _98 = new WalkerStateBindStarPointer(this, mAnimeScale);

    // "Kabokuri kuribo"
    mKuribo = new Kuribo("カボクリクリボー");

    if (mIsFloating) {
        mKuribo->onNoGravitySupport();
    }

    mKuribo->initWithoutIter();
    mKuribo->makeActorDead();

    // "Broken model"
    _90 = MR::createModelObjMapObjStrongLight("壊れモデル", "KabokuriBreak", getBaseMtx());
    _90->kill();

    MR::useStageSwitchWriteDead(this, rIter);

    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
        makeActorDead();
        return;
    }

    makeActorAppeared();
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
    MR::setBaseTRMtx(this, _9C);
    MR::setBaseScale(this, mAnimeScale->_C * mScale);
}

void Kabokuri::updatePose() {
    TVec3f* gravity = &mGravity;
    mFrontVec.scaleAdd(-gravity->dot(mFrontVec), *gravity, mFrontVec);

    if (MR::isNearZero(mFrontVec)) {
        _9C.getZDir(mFrontVec);
    } else {
        MR::normalize(&mFrontVec);
    }

    TVec3f* upVec;
    if (MR::isBindedGround(this)) {
        upVec = const_cast< TVec3f* >(MR::getGroundNormal(this));
    } else {
        TVec3f v2 = -mGravity;
        upVec = &v2;
    }

    MR::blendQuatUpFront(&_9C, *upVec, mFrontVec, ::sUpVecBlendRate, ::sFrontVecBlendRate);
}

void Kabokuri::addVelocityBase() {
    if (MR::isBindedGround(this)) {
        MR::attenuateVelocity(this, 0.9f);
    } else {
        MR::addVelocityToGravity(this, ::sNormalGravity);
        MR::attenuateVelocity(this, 0.9f);
    }

    MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);
}

void Kabokuri::addVelocityToRailPoint(f32 vel) {
    if (MR::isRailReachedHorizonCurrentPos(this, ::sRailCoordStepInterval) && !_BD) {
        MR::moveCoord(this, ::sRailCoordStepInterval);

        s32 nextRailPoint = -1;
        MR::getNextRailPointArg0WithInit(this, &nextRailPoint);

        if (nextRailPoint != 0 && MR::isRailReachedNearNextPoint(this, ::sRailCoordStepInterval)) {
            MR::moveCoordToRailPoint(this, MR::getNextRailPointNo(this));
            _BD = 1;
        }

        if (MR::isRailReachedEdge(this)) {
            MR::reverseRailDirection(this);
        }
    }

    TVec3f velocity;
    MR::calcVelocityMoveToTarget(&velocity, this, MR::getRailPos(this), 1.0f);
    if (velocity.dot(mFrontVec) > 0.4f) {
        mVelocity.add(velocity * vel);
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
    if (isEnablePointBind() && _98->tryStartPointBind()) {
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
        _BD = 0;
    }

    MR::turnDirectionToTargetDegree(this, &mFrontVec, MR::getRailPos(this), ::sWalkTurnDegree);
    addVelocityToRailPoint(::sWalkSpeed);
    addVelocityBase();

    if (_BD && MR::isRailReachedHorizonCurrentPos(this, ::sWalkGoalRange)) {
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
        mKuribo->appearHipDropped(mPosition, _9C);
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
    MR::attenuateVelocity(this, 0.99f);
    MR::reboundVelocityFromCollision(this, 0.9f);

    if (MR::isGreaterStep(this, 1) || (MR::isGreaterStep(this, 1) && MR::isBinded(this))) {
        mKuribo->appearBlowed(mPosition, _9C, mVelocity);
        mKuribo->mGravity.set(mGravity);
        MR::invalidateClipping(this);
        setNerve(&NrvKabokuri::KabokuriNrvBreak::sInstance);
    }
}

void Kabokuri::exeBreak() {
    if (MR::isFirstStep(this)) {
        MR::hideModel(this);
        _90->appear();
        MR::startAction(_90, "Break");
        MR::startSound(this, "SE_EM_KABOKURI_BREAK_PUMP");
        MR::startBlowHitSound(this);
        MR::zeroVelocity(this);
    }

    if (MR::isActionEnd(_90)) {
        kill();
        _90->kill();
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
    _98->kill();
}

void Kabokuri::exeBindStarPointer() {
    MR::updateActorStateAndNextNerve(this, _98, &NrvKabokuri::KabokuriNrvWalk::sInstance);
}

Kabokuri::~Kabokuri() {
}
