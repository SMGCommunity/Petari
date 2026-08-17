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
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "revolution/types.h"

namespace {
    const f32 sNormalGravity = 0.2f;
    const f32 sWalkGoalRange = 50.0f;
};  // namespace

namespace NrvKabokuri {
    NEW_NERVE(KabokuriNrvWait, Kabokuri, Wait);
    NEW_NERVE(KabokuriNrvWalk, Kabokuri, Walk);
    NEW_NERVE(KabokuriNrvDropFire, Kabokuri, DropFire);
    NEW_NERVE(KabokuriNrvAttacksuccess, Kabokuri, AttackSuccess);
    NEW_NERVE(KabokuriNrvTrampled, Kabokuri, Trampled);
    NEW_NERVE(KabokuriNrvStarPieceHitted, Kabokuri, StarPieceHitted);
    NEW_NERVE(KabokuriNrvBindStarPointer, Kabokuri, BindStarPointer);
    NEW_NERVE(KabokuriNrvHitAttacked, Kabokuri, HitAttacked);
    NEW_NERVE(KabokuriNrvHipDropped, Kabokuri, HipDropped);
    NEW_NERVE(KabokuriNrvBreak, Kabokuri, Break);
};  // namespace NrvKabokuri

Kabokuri::Kabokuri(const char* pName)
    : LiveActor(pName), mKuribo(nullptr), _90(nullptr), _94(nullptr), _98(nullptr), _9C(0.0f, 0.0f, 0.0f, 1.0f), _AC(0.0f, 0.0f, 1.0f), _B8(-1),
      _BD(0) {
    KabokuriFireHolderFunc::createHolder();
}

void Kabokuri::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initRailRider(rIter);
    MR::moveCoordAndTransToNearestRailPos(this);
    MR::makeQuatAndFrontFromRotate(&_9C, &_AC, this);
    initModelManagerWithAnm("Kabokuri", nullptr, false);
    MR::connectToSceneEnemy(this);
    MR::initLightCtrl(this);
    MR::getJMapInfoArg0WithInit(rIter, &_BC);

    if (!_BC) {
        MR::onCalcGravity(this);
        MR::calcGravity(this);
    } else {
        MR::offCalcGravity(this);

        TVec3f v1;
        _9C.getYDir(v1);
        mGravity.set(-v1);
    }

    initBinder(90.0f * mScale.y, 90.0f * mScale.y, 0);
    initEffectKeeper(1, nullptr, false);
    initSound(2, false);
    initSensor();
    MR::initShadowVolumeSphere(this, 90.0f);
    initNerve(&NrvKabokuri::KabokuriNrvWait::sInstance);
    MR::initStarPointerTarget(this, 90.0f * mScale.y, TVec3f(0.0f, 90.0f * mScale.y, 0.0f));

    _94 = new AnimScaleController(nullptr);
    _94->setParamTight();

    _98 = new WalkerStateBindStarPointer(this, _94);

    // "Kabokuri kuribo"
    mKuribo = new Kuribo("カボクリクリボー");

    if (_BC) {
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

    MR::addHitSensorEnemy(this, "body", 8, 100.0f * scale, TVec3f(0.0f, 100.0f * scale, 0.0f));
    MR::addHitSensorEnemyAttack(this, "attack", 8, 70.0f * scale, TVec3f(0.0f, 70.0f * scale, 0.0f));
}

void Kabokuri::control() {
    _94->updateNerve();

    if (_BC) {
        MR::calcGravityOrZero(this);
    }

    updatePose();
    tryPointBind();
}

void Kabokuri::calcAndSetBaseMtx() {
    MR::setBaseTRMtx(this, _9C);
    MR::setBaseScale(this, _94->_C * mScale);
}

void Kabokuri::updatePose() {
    TVec3f* gravity = &mGravity;
    _AC.scaleAdd(-gravity->dot(_AC), *gravity, _AC);

    if (MR::isNearZero(_AC)) {
        _9C.getZDir(_AC);
    } else {
        MR::normalize(&_AC);
    }

    TVec3f* upVec;
    if (MR::isBindedGround(this)) {
        upVec = const_cast< TVec3f* >(MR::getGroundNormal(this));
    } else {
        TVec3f v2 = -mGravity;
        upVec = &v2;
    }

    MR::blendQuatUpFront(&_9C, *upVec, _AC, 0.1f, 0.2f);
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
    if (MR::isRailReachedHorizonCurrentPos(this, ::sWalkGoalRange) && !_BD) {
        MR::moveCoord(this, ::sWalkGoalRange);

        s32 nextRailPoint = -1;
        MR::getNextRailPointArg0WithInit(this, &nextRailPoint);

        if (nextRailPoint != 0 && MR::isRailReachedNearNextPoint(this, ::sWalkGoalRange)) {
            MR::moveCoordToRailPoint(this, MR::getNextRailPointNo(this));
            _BD = 1;
        }

        if (MR::isRailReachedEdge(this)) {
            MR::reverseRailDirection(this);
        }
    }

    TVec3f velocity;
    MR::calcVelocityMoveToTarget(&velocity, this, MR::getRailPos(this), 1.0f);
    if (velocity.dot(_AC) > 0.4f) {
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

Kabokuri::~Kabokuri() {
}
