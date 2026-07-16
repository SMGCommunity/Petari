#include "Game/Enemy/Kanina.hpp"
#include "Game/LiveActor/AnimationRandomPlayer.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/BenefitItemObj.hpp"
#include "Game/Util.hpp"
#include "Game/Util/JointRumbler.hpp"
#include "revolution/wpad.h"

namespace NrvKanina {
    NEW_NERVE(HostTypeAppear, Kanina, Appear);
    NEW_NERVE(HostTypeWait, Kanina, Wait);
    NEW_NERVE(HostTypeAttack, Kanina, Attack);
    NEW_NERVE(HostTypeVauntAttackSuccess, Kanina, VauntAttackSuccess);
    NEW_NERVE(HostTypeGuard, Kanina, Guard);
    NEW_NERVE(HostTypeGuardEnd, Kanina, GuardEnd);
    NEW_NERVE(HostTypeDamageFireBall, Kanina, DamageFireBall);
    NEW_NERVE(HostTypeDamageHipDrop, Kanina, DamageHipDrop);
    NEW_NERVE(HostTypeDamageTrampleInvincivle, Kanina, DamageHipDrop);
    NEW_NERVE(HostTypeDig, Kanina, Dig);
    NEW_NERVE(HostTypeWaitUnderGround, Kanina, WaitUnderGround);
    NEW_NERVE(HostTypeFindPlayer, Kanina, FindPlayer);
    NEW_NERVE(HostTypeRunAwayBreak, Kanina, RunAwayBreak);
    NEW_NERVE(HostTypeReboundEach, Kanina, ReboundEach);
    NEW_NERVE(HostTypeTurn, Kanina, Turn);
    NEW_NERVE(HostTypeTurnEnd, Kanina, TurnEnd);
    NEW_NERVE_ONEND(HostTypePointing, Kanina, Pointing, Pointing);
    NEW_NERVE(HostTypePointingEnd, Kanina, GuardEnd);
    NEW_NERVE(HostTypeWalk, Kanina, Walk);
    NEW_NERVE(HostTypeRunAway, Kanina, RunAway);
    NEW_NERVE(HostTypeRunAwayReboundDirection, Kanina, RunAwayReboundDirection);
    NEW_NERVE(HostTypeHitWall, Kanina, HitWall);

};  // namespace NrvKanina

namespace {
    static const f32 sSensorRadius = 100.0f;
    static const f32 sBinderRadius = 100.0f;
    static const f32 sBinderOffsetY = 100.0f;
    static const f32 sShadowRadius = 80.0f;
    // static const f32 sTerritoryRadius;
    static const s32 sStarPieceNum = 3;
    static const s32 sCoinNum = 1;
    static const f32 sPoseLerpRate = 0.3f;
    static const s32 sAnimIntervalBlink = 120;
    static const f32 sSensorRadiusAttack = 80.0f;
    static const f32 sWalkSpeed = 3.0f;
    static const s32 sWalkTime = 180;
    static const s32 sWaitTime = 120;
    static const s32 sPunchDamageTime = 15;
    static const f32 sPunchDamageVelocity = 25.0f;
    static const f32 sPunchUpperVec = 25.0f;
    static const f32 sGravityAccel = 1.5f;
    static const s32 sStopSceneTimePunchBackword = 8;
    static const s32 sWaitUnderGroundTime = 120.0f;
    static const f32 sKeepUnderGroundDistance = 500.0f;
    static const f32 sSearchRadius = 1000.0f;
    static const f32 sWalkSpeedRunAway = 13.0f;
    static const f32 sAnimRateRunAway = 1.0f;
    static const f32 sSafetyDistance = 2000.0f;
    static const s32 sRunAwayRunningTime1 = 15;
    static const s32 sRunAwayRunningTime2 = 30;
    static const s32 sRunAwayRunningTime3 = 60;
    static const s32 sRunAwayRunningTime4 = 75;
    static const s32 sRunAwayIntervalTime1 = 10;
    static const s32 sRunAwayIntervalTime2 = 30;
    static const s32 sRunAwayIntervalTime3 = 60;
    static const s32 sRunAwayIntervalTime4 = 75;
    static const f32 sRunAwayDirectionAngle = 60.0f;
    static const f32 sRunAwayFaceAngle = 15.0f;
    static const s32 sHitWallTime = 10;
    static const f32 sWallHitReboundAcc = 6.0f;
    static const f32 sWallHitReboundAccY = 20.0f;
    static const f32 sGravityAccelHitWall = 2.2f;
    static const s32 sGuardTime = 90;
    static const f32 sPunchAccel = 30.0f;
    static const f32 sGuardFrec = 0.88f;
    static const f32 sBackwardAngle = 30.0f;
    static const f32 sTurnSpeed = 1.7f;
    static const s32 sTurnEndTime = 30;
    static const f32 sBackwardAngleTurn = 120.0f;
    static const s32 sHipDropDownTime = 70;
    static const f32 sKinokoOneUpShootSpeed = 15.0f;
    static const f32 sWalkSpeedRunAwayRed = 7.0f;
    static const f32 sRunAwayDirectionAngleRed = 30.0f;
    static const f32 sRunAwayFaceAngleRed = 45.0f;
    static const s32 sWalkCount = 3;
    static const f32 sUpVecBlendRate = 0.05f;
    static const f32 sFrontVecBlendRate = 0.5f;
    static const f32 sGroundFrec = 0.93f;
    static const f32 sAirFrec = 0.99f;
    static const f32 sPushAccel = 1.5f;
    // static const f32 sHitHorizontalPower;
    // static const f32 sHitVerticalPower;
    static const f32 sDownGravityAccel = 1.0f;
};  // namespace

namespace {
    u32 getRunAwayBreakTimeRandom() {
        f32 random = MR::getRandom();
        if (0.0f <= random && random < 0.25f) {
            return ::sRunAwayIntervalTime1;
        }
        if (0.25f <= random && random < 0.5f) {
            return ::sRunAwayIntervalTime2;
        }
        if (0.5f <= random && random < 0.75f) {
            return ::sRunAwayIntervalTime3;
        }
        return ::sRunAwayIntervalTime4;
    }

    u32 getRunAwayTimeRandom() {
        f32 random = MR::getRandom();
        if (0.0f <= random && random < 0.25f) {
            return ::sRunAwayRunningTime1;
        }
        if (0.25f <= random && random < 0.5f) {
            return ::sRunAwayRunningTime2;
        }
        if (0.5f <= random && random < 0.75f) {
            return ::sRunAwayRunningTime3;
        }
        return ::sRunAwayRunningTime4;
    }
};  // namespace

Kanina::Kanina(const char* pName)
    : LiveActor(pName), mJointRumbler(nullptr), mKinokoOneUp(nullptr), mAnimationRandomPlayer(nullptr), _98(gZeroVec), _B4(15), _B8(10), _BC(1.0f),
      _C0(0.0f, 0.0f, 1.0f), mType(KaninaType_Blue), _D0(0.0f, 0.0f, 1.0f), _DC(0) {
    _A4.set(0.0f, 0.0f, 0.0f, 1.0f);
}

void Kanina::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::makeQuatAndFrontFromRotate(&_A4, &_D0, this);
    _98.set(mPosition);

    const char* objectName = nullptr;

    MR::getObjectName(&objectName, rIter);
    initModelManagerWithAnm(objectName, nullptr, false);
    mAnimationRandomPlayer = new AnimationRandomPlayer(this, "Blink", "Normal", ::sAnimIntervalBlink, 0.25f);
    MR::connectToSceneEnemy(this);
    MR::initLightCtrl(this);
    initSound(4, false);
    initHitSensor(2);
    MR::addHitSensorMtxEnemy(this, "body", 8, ::sSensorRadius, MR::getJointMtx(this, "Body"), TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensorMtxEnemyAttack(this, "attack", 8, ::sSensorRadiusAttack, MR::getJointMtx(this, "Body"), TVec3f(0.0f, 0.0f, 0.0f));
    initBinder(::sBinderRadius, ::sBinderOffsetY, 8);
    initEffectKeeper(1, "Kanina", false);
    MR::addEffectHitNormal(this, nullptr);
    MR::onCalcGravity(this);
    MR::initShadowVolumeSphere(this, ::sShadowRadius);
    MR::addToAttributeGroupSearchTurtle(this);
    mJointRumbler = new JointRumbler(this, "Root", 6.0f, 0.1f, 45, true, 0);

    if (MR::isEqualString(objectName, "KaninaRed")) {
        initForType(rIter, KaninaType_Red);
    } else {
        initForType(rIter, KaninaType_Blue);
    }

    MR::initStarPointerTarget(this, ::sSensorRadius, TVec3f::makeZeroVec());
    initNerve(&NrvKanina::HostTypeAppear::sInstance);
    MR::needStageSwitchReadAppear(this, rIter);
    MR::syncStageSwitchAppear(this);
    makeActorDead();
}

void Kanina::initAfterPlacement() {
    MR::trySetMoveLimitCollision(this);
}

void Kanina::kill() {
    MR::emitEffect(this, "Death");
    MR::startSound(this, "SE_EM_EXPLODE_S");
    LiveActor::kill();
}

void Kanina::appearItemPunch() {
    if (mType == KaninaType_Red) {
        MR::appearStarPiece(this, mPosition, ::sStarPieceNum, 10.0f, 40.0f, false);
        MR::startSound(this, "SE_OJ_STAR_PIECE_BURST");
    } else if (mType == KaninaType_Blue) {
        MR::appearKinokoOneUpPop(mKinokoOneUp, getBaseMtx(), ::sKinokoOneUpShootSpeed);
    }
}

void Kanina::appearItemTrample() {
    if (mType == KaninaType_Red) {
        // most likely a copy-paste error from appearItemPunch
        MR::appearCoinPop(this, mPosition, ::sStarPieceNum);
    } else if (mType == KaninaType_Blue) {
        MR::appearKinokoOneUpPop(mKinokoOneUp, getBaseMtx(), ::sKinokoOneUpShootSpeed);
    }
}

void Kanina::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorEnemyAttack(pSender)) {
        if (!tryAttack(pSender, pReceiver)) {
            if (MR::isSensorPlayer(pReceiver)) {
                MR::sendMsgPush(pReceiver, pSender);
            } else if (tryPushEach(pSender, pReceiver)) {
                return;
            }
        }
    } else if ((MR::isSensorPlayer(pReceiver) && !isStatePossibleToAttack()) || MR::isSensorEnemy(pReceiver)) {
        MR::sendMsgPush(pReceiver, pSender);
    }
}

bool Kanina::tryAttack(HitSensor* pSender, HitSensor* pReceiver) {
    if (!MR::isSensorPlayer(pReceiver)) {
        return false;
    }

    if (!MR::isSensorPlayer(pReceiver)) {
        return false;
    }

    if (!isStatePossibleToAttack()) {
        return false;
    }

    if (isPlayerBackward(::sBackwardAngle)) {
        return false;
    }

    if (!MR::sendMsgEnemyAttack(pReceiver, pSender)) {
        return false;
    }

    MR::emitEffectHitBetweenSensors(this, pSender, pReceiver, 0.0f, nullptr);
    setNerve(&NrvKanina::HostTypeAttack::sInstance);

    return true;
}

bool Kanina::tryPushEach(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pSender)) {
        return false;
    }

    if (MR::isSensorEnemyAttack(pReceiver)) {
        return false;
    }

    if (!MR::isSensorEnemyAttack(pSender)) {
        return false;
    }

    if (isNerve(&NrvKanina::HostTypeReboundEach::sInstance)) {
        return false;
    }

    if (!MR::sendMsgPush(pReceiver, pSender)) {
        return false;
    }

    TVec3f diff(pSender->mPosition - pReceiver->mPosition);
    MR::normalizeOrZero(&diff);
    TVec3f upVec;
    MR::calcUpVec(&upVec, this);
    MR::vecKillElement(diff, upVec, &diff);

    mVelocity.add(diff * ::sPushAccel + upVec * ::sPushAccel);
    setNerve(&NrvKanina::HostTypeReboundEach::sInstance);
    return true;
}

bool Kanina::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgPlayerTrample(msg)) {
        if (MR::isPlayerElementModeInvincible()) {
            appearItemPunch();
            kill();
            return true;
        }
        if (receiveTrample(pSender, pReceiver)) {
            MR::tryRumblePad(this, "中", WPAD_CHAN0);
            return true;
        }
    }

    if (MR::isMsgFireBallAttack(msg)) {
        return receiveFireBall(pSender, pReceiver);
    }

    if (MR::isMsgPlayerHipDrop(msg)) {
        return receiveHipDrop(pSender, pReceiver);
    }

    if (MR::isMsgJetTurtleAttack(msg)) {
        return receiveFireBall(pSender, pReceiver);
    }

    if (MR::isMsgInvincibleAttack(msg)) {
        return receiveInvincibleAttack(pSender, pReceiver);
    }

    if (MR::isMsgPlayerHitAll(msg)) {
        return receivePunch(pSender, pReceiver);
    }

    if (MR::isMsgStarPieceReflect(msg)) {
        setNerve(&NrvKanina::HostTypeGuard::sInstance);
        return true;
    }

    return false;
}

bool Kanina::receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pSender)) {
        return false;
    }

    MR::addVelocityFromPush(this, ::sPushAccel, pSender, pReceiver);

    if (!pSender->isType(ATYPE_COCO_NUT)) {
        setNerve(&NrvKanina::HostTypeReboundEach::sInstance);
    }

    return true;
}

bool Kanina::receiveTrample(HitSensor*, HitSensor*) {
    if (!isNerve(&NrvKanina::HostTypeAttack::sInstance)) {
        setNerve(&NrvKanina::HostTypeGuard::sInstance);
        return true;
    }

    return false;
}

bool Kanina::receivePunch(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorEnemyAttack(pReceiver)) {
        return false;
    }

    if (isPlayerBackward(::sBackwardAngle)) {
        MR::stopScene(sStopSceneTimePunchBackword);
        doDamageFireBall(pSender, pReceiver);
        return true;
    }

    if (!isNerve(&NrvKanina::HostTypeAttack::sInstance)) {
        if (!isStateDamageNoFireBall()) {
            TVec3f vec;
            MR::calcVecFromTargetPosH(&vec, this, pSender->mPosition, nullptr);
            mVelocity.add(vec * ::sPunchAccel);
            setNerve(&NrvKanina::HostTypeGuard::sInstance);
            return true;
        }
    }

    return false;
}

bool Kanina::receiveFireBall(HitSensor* pSender, HitSensor* pReceiver) {
    if (pReceiver == getSensor("attack")) {
        return false;
    }

    doDamageFireBall(pSender, pReceiver);
    return true;
}

void Kanina::doDamageFireBall(HitSensor* pSender, HitSensor* pReceiver) {
    MR::setVelocityBlowAttack(this, pSender, pReceiver, ::sPunchDamageVelocity, ::sPunchUpperVec, 4);
    MR::isNearZero(mVelocity);
    TVec3f vec;
    MR::vecKillElement(mVelocity, -mGravity, &vec);
    MR::normalizeOrZero(&vec);
    _D0.set(-vec);
    setNerve(&NrvKanina::HostTypeDamageFireBall::sInstance);
}

void Kanina::startRun() {
    if (mType == KaninaType_Red) {
        MR::startBck(this, "ChaseRun", nullptr);
        MR::startBtp(this, "Angry");
    } else {
        MR::startBck(this, "Run", nullptr);
    }

    MR::setBckRate(this, ::sAnimRateRunAway);

    TVec3f upVec;
    MR::calcUpVec(&upVec, this);

    TVec3f vecFromPlayerH;
    MR::calcVecFromPlayerH(&vecFromPlayerH, this);

    f32 BC = _BC;
    f32 angle = -1.0f * BC * getFloatDependentType(::sRunAwayDirectionAngleRed, ::sRunAwayDirectionAngle);

    TPos3f rotate;
    rotate.identity();
    rotate.makeRotate(upVec, MR::toRadian(angle));

    TVec3f multVec(vecFromPlayerH);
    rotate.mult(multVec, multVec);

    if (mType == KaninaType_Red) {
        multVec.negate();
    }

    MR::normalizeOrZero(&multVec);

    mVelocity.set(multVec * getFloatDependentType(::sWalkSpeedRunAwayRed, ::sWalkSpeedRunAway));

    angle = -1.0f * BC *
            (getFloatDependentType(::sRunAwayFaceAngleRed, ::sRunAwayFaceAngle) +
             getFloatDependentType(::sRunAwayDirectionAngleRed, ::sRunAwayDirectionAngle));
    TPos3f rotate2;
    rotate2.identity();
    rotate2.makeRotate(upVec, MR::toRadian(angle));
    TVec3f multVec2(vecFromPlayerH);
    rotate2.mult(multVec2, multVec2);

    TQuat4f quat;
    MR::makeQuatSideUp(&quat, multVec2 * BC, upVec);
    quat.getZDir(_D0);

    _B4 = ::getRunAwayTimeRandom();
    _BC *= -1.0f;
}

void Kanina::startRunAwayLevelSound() {
    if (mType == KaninaType_Blue) {
        MR::startLevelSound(this, "SE_EM_LV_KANINA_AWAY");
    } else if (mType == KaninaType_Red && isNerve(&NrvKanina::HostTypeRunAwayReboundDirection::sInstance)) {
        MR::startLevelSound(this, "SE_EM_LV_KANINA_AWAY");
    }
}

bool Kanina::receiveHipDrop(HitSensor* pSender, HitSensor* pReceiver) {
    if (receiveTrample(pSender, pReceiver)) {
        MR::tryRumblePad(this, "中", WPAD_CHAN0);
        MR::sendMsgAwayJump(pSender, pReceiver);
        return true;
    }

    return false;
}

bool Kanina::receiveInvincibleAttack(HitSensor* pSender, HitSensor* pReceiver) {
    if (pReceiver == getSensor("attack")) {
        return false;
    }
    doDamageFireBall(pSender, pReceiver);
    return true;
}

void Kanina::calcAndSetBaseMtx() {
    TPos3f baseMtx(getBaseMtx());
    TQuat4f quat;
    baseMtx.getQuat(quat);

    if (isNerve(&NrvKanina::HostTypeDamageFireBall::sInstance)) {
        quat.set< f32 >(_A4);
    } else {
        quat.slerp(_A4, ::sPoseLerpRate);
    }

    MR::setBaseTRMtx(this, quat);
}

void Kanina::control() {
    mJointRumbler->update();
    MR::blendQuatFromGroundAndFront(&_A4, this, _D0, ::sUpVecBlendRate, ::sFrontVecBlendRate);
    updateMovement();

    if (MR::isInWater(this, TVec3f(0.0f, 0.0f, 0.0f))) {
        MR::setSeVersion(this, 1);
    } else {
        MR::setSeVersion(this, 0);
    }

    if (isStateBlink()) {
        mAnimationRandomPlayer->updateNerve();
    }
}

bool Kanina::isStateStayOnGround() const {
    if (isNerve(&NrvKanina::HostTypeWalk::sInstance)) {
        return false;
    }

    if (isNerve(&NrvKanina::HostTypeRunAway::sInstance)) {
        return false;
    }

    if (isNerve(&NrvKanina::HostTypeRunAwayReboundDirection::sInstance)) {
        return false;
    }

    if (isNerve(&NrvKanina::HostTypeDamageFireBall::sInstance)) {
        return false;
    }

    return true;
}

void Kanina::initForType(const JMapInfoIter& rIter, KaninaType type) {
    mType = type;
    if (mType == KaninaType_Red) {
        MR::declareStarPiece(this, ::sStarPieceNum);
        MR::declareCoin(this, ::sCoinNum);
    }

    if (mType == KaninaType_Blue) {
        mKinokoOneUp = MR::createKinokoOneUp();
    }
}

bool Kanina::isPlayerBackward(f32 angle) const {
    TVec3f frontVec;
    MR::calcFrontVec(&frontVec, this);
    angle = angle * 2.0f;
    return MR::isInSightFanPlayer(this, -frontVec, 800.0f, angle, 30.0f);
}

bool Kanina::isStatePossibleToAttack() const {
    if (isNerve(&NrvKanina::HostTypeAttack::sInstance)) {
        return false;
    }

    if (isStateGuard()) {
        return false;
    }

    if (isStateDamageNoFireBall()) {
        return false;
    }

    return true;
}

bool Kanina::isStateBlink() const {
    return isNerve(&NrvKanina::HostTypeWait::sInstance) || isNerve(&NrvKanina::HostTypeVauntAttackSuccess::sInstance) ||
           isNerve(&NrvKanina::HostTypeWalk::sInstance) || isNerve(&NrvKanina::HostTypeRunAway::sInstance) ||
           isNerve(&NrvKanina::HostTypeRunAwayReboundDirection::sInstance) || isNerve(&NrvKanina::HostTypeRunAwayBreak::sInstance) ||
           isNerve(&NrvKanina::HostTypeReboundEach::sInstance) || isNerve(&NrvKanina::HostTypeTurn::sInstance) ||
           isNerve(&NrvKanina::HostTypeTurnEnd::sInstance) || isNerve(&NrvKanina::HostTypeFindPlayer::sInstance);
}

bool Kanina::isStateGuard() const {
    return isNerve(&NrvKanina::HostTypeGuard::sInstance) || isNerve(&NrvKanina::HostTypeGuardEnd::sInstance);
}

bool Kanina::isStateDamageNoFireBall() const {
    return isNerve(&NrvKanina::HostTypeDamageHipDrop::sInstance) || isNerve(&NrvKanina::HostTypeDamageTrampleInvincivle::sInstance);
}

bool Kanina::isBindedGroundWaterBottom() const {
    return MR::isBindedGroundWaterBottomH(this) || MR::isBindedGroundWaterBottomM(this);
}

void Kanina::updateMovement() {
    if (!MR::isOnGround(this)) {
        f32 gravityAccel = ::sGravityAccel;
        if (isNerve(&NrvKanina::HostTypeDamageFireBall::sInstance)) {
            gravityAccel = ::sDownGravityAccel;
        } else if (isNerve(&NrvKanina::HostTypeHitWall::sInstance)) {
            gravityAccel = ::sGravityAccelHitWall;
        }
        MR::addVelocityToGravity(this, gravityAccel);
    }

    if (!isStateStayOnGround()) {
        return;
    }

    f32 frec;

    if (MR::isOnGround(this)) {
        frec = ::sGroundFrec;
    } else {
        frec = ::sAirFrec;
    }

    if (isStateGuard()) {
        frec = ::sGuardFrec;
    }

    MR::attenuateVelocity(this, frec);
}

bool Kanina::tryFindPlayer() {
    if (MR::isPlayerHidden()) {
        return false;
    }

    TVec3f front;
    MR::calcFrontVec(&front, this);
    if (!MR::isInSightFanPlayer(this, front, ::sSearchRadius, 180.0f, 30.0f)) {
        return false;
    }

    setNerve(&NrvKanina::HostTypeFindPlayer::sInstance);
    return true;
}

bool Kanina::tryHitWall() {
    if (!MR::isBindedWall(this)) {
        return false;
    }

    if (MR::isBindedWallOfMoveLimit(this)) {
        return false;
    }

    if (!MR::reboundVelocityFromCollision(this, 1.0f, 0.0f, 1.0f)) {
        return false;
    }

    _C0.set(mVelocity);
    MR::normalizeOrZero(&_C0);
    setNerve(&NrvKanina::HostTypeHitWall::sInstance);
    return true;
}

bool Kanina::tryTurn() {
    if (mType == KaninaType_Blue) {
        return false;
    }

    if (!isPlayerBackward(::sBackwardAngleTurn)) {
        return false;
    }

    setNerve(&NrvKanina::HostTypeTurn::sInstance);
    return true;
}

bool Kanina::tryPointing() {
    if (MR::isStarPointerPointing2POnPressButton(this, "弱", true, false)) {
        setNerve(&NrvKanina::HostTypePointing::sInstance);
        return true;
    }

    return false;
}

void Kanina::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Appear", nullptr);
        MR::startBtp(this, "Normal");
        MR::validateHitSensors(this);
    }

    MR::startLevelSound(this, "SE_EM_LV_KANINA_PRE_APPEAR");

    if (MR::isBckStopped(this)) {
        setNerve(&NrvKanina::HostTypeWait::sInstance);
    }
}

void Kanina::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::zeroVelocity(this);
        MR::startBck(this, "Wait", nullptr);
    }

    if (!tryFindPlayer() && !tryPointing() && MR::isStep(this, ::sWaitTime)) {
        if (isBindedGroundWaterBottom()) {
            setNerve(&NrvKanina::HostTypeDig::sInstance);
            return;
        }

        if (mType == KaninaType_Blue && _DC >= ::sWalkCount) {
            setNerve(&NrvKanina::HostTypeDig::sInstance);
            return;
        }

        setNerve(&NrvKanina::HostTypeWalk::sInstance);
    }
}

void Kanina::exeWalk() {
    if (MR::isFirstStep(this)) {
        _DC++;
        MR::startBck(this, "Walk", nullptr);
        TVec3f temp;
        TVec3f up;
        TVec3f front;
        MR::calcFrontVec(&front, this);
        MR::calcUpVec(&up, this);
        f32 angle = MR::getRandom(-HALF_PI, HALF_PI);

        TPos3f rotate;
        rotate.identity();
        rotate.makeRotate(up, angle);
        rotate.mult(front, front);

        MR::normalize(&front);
        temp.set(front);
        _D0.set(temp);

        MR::blendQuatFromGroundAndFront(&_A4, this, _D0, ::sUpVecBlendRate, ::sFrontVecBlendRate);

        TVec3f quatX;
        _A4.getXDir(quatX);
        mVelocity.set(quatX * (::sWalkSpeed * MR::getSignHalfProbability()));
    }

    if (!tryFindPlayer() && !tryPointing() && MR::isStep(this, ::sWalkTime)) {
        MR::zeroVelocity(this);
        setNerve(&NrvKanina::HostTypeWait::sInstance);
    }
}

void Kanina::exeRunAway() {
    if (MR::isFirstStep(this)) {
        startRun();
    }

    startRunAwayLevelSound();

    if (!tryHitWall() && !tryPointing()) {
        if (::sSafetyDistance < MR::calcDistanceToPlayerH(this)) {
            MR::zeroVelocity(this);
            if ((mType == KaninaType_Red && MR::isHalfProbability()) || mType == KaninaType_Blue) {
                setNerve(&NrvKanina::HostTypeDig::sInstance);
            } else {
                setNerve(&NrvKanina::HostTypeWait::sInstance);
            }
        } else if (MR::isStep(this, _B4)) {
            MR::zeroVelocity(this);
            setNerve(&NrvKanina::HostTypeRunAwayBreak::sInstance);
        }
    }
}

void Kanina::exeRunAwayReboundDirection() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Run", nullptr);
        MR::setBckRate(this, ::sAnimRateRunAway);
        TVec3f up;
        MR::calcUpVec(&up, this);
        TVec3f vec(_C0);
        MR::vecKillElement(vec, up, &vec);
        MR::normalizeOrZero(&vec);
        mVelocity.set(vec * getFloatDependentType(::sWalkSpeedRunAwayRed, ::sWalkSpeedRunAway));
        _B4 = ::getRunAwayBreakTimeRandom();
        _BC *= -1.0f;
    }

    startRunAwayLevelSound();

    if (!tryHitWall() && !tryPointing()) {
        if (::sSafetyDistance < MR::calcDistanceToPlayer(this)) {
            setNerve(&NrvKanina::HostTypeWait::sInstance);
            return;
        }

        if (MR::isStep(this, _B4)) {
            setNerve(&NrvKanina::HostTypeRunAwayBreak::sInstance);
        }
    }
}

void Kanina::exeRunAwayBreak() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait", nullptr);
        _B8 = ::getRunAwayBreakTimeRandom();
    }

    if (!tryTurn() && !tryPointing() && MR::isStep(this, _B8)) {
        if (isBindedGroundWaterBottom()) {
            setNerve(&NrvKanina::HostTypeDig::sInstance);
            return;
        }
        setNerve(&NrvKanina::HostTypeRunAway::sInstance);
    }
}

void Kanina::exeAttack() {
    if (MR::isFirstStep(this)) {
        MR::calcVecToPlayerH(&_D0, this, nullptr);
        MR::startBck(this, "Attack", nullptr);
        MR::startSound(this, "SE_EM_KANINA_ATTACK_FRONT");
    }

    if (MR::isBckStopped(this)) {
        setNerve(&NrvKanina::HostTypeVauntAttackSuccess::sInstance);
    }
}

void Kanina::exeVauntAttackSuccess() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Vaunt", nullptr);
        MR::startBtp(this, "Angry");
    }

    if (MR::isBckStopped(this)) {
        setNerve(&NrvKanina::HostTypeRunAwayBreak::sInstance);
    }
}

void Kanina::exeHitWall() {
    if (MR::isFirstStep(this)) {
        mJointRumbler->start();
        TVec3f up;
        MR::calcUpVec(&up, this);
        mVelocity.set(_C0 * ::sWallHitReboundAcc + up * ::sWallHitReboundAccY);
        TVec3f vec(_C0);
        MR::vecKillElement(vec, up, &vec);
        MR::normalizeOrZero(&vec);
        MR::makeQuatSideUp(&_A4, vec, up);
    }

    startRunAwayLevelSound();

    if (MR::isGreaterStep(this, ::sHitWallTime) && MR::isBindedGround(this)) {
        setNerve(&NrvKanina::HostTypeRunAwayReboundDirection::sInstance);
    }
}

void Kanina::exeReboundEach() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait", nullptr);
        mJointRumbler->start();
    }

    if (MR::isGreaterStep(this, ::sHitWallTime) && MR::isBindedGround(this)) {
        setNerve(&NrvKanina::HostTypeRunAwayReboundDirection::sInstance);
    }
}

void Kanina::exeGuard() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Guard", nullptr);
        MR::startSound(this, "SE_EM_KANINA_GUARD");
        MR::tryRumbleDefaultHit(this, 0);
        MR::calcVecToPlayerH(&_D0, this, nullptr);
    }

    if (MR::isStep(this, ::sGuardTime)) {
        setNerve(&NrvKanina::HostTypeGuardEnd::sInstance);
    }
}

void Kanina::exeGuardEnd() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "GuardReturn", nullptr);
    }
    if (MR::isBckStopped(this)) {
        setNerve(&NrvKanina::HostTypeRunAwayBreak::sInstance);
    }
}

void Kanina::exeDamageHipDrop() {
    if (MR::isFirstStep(this)) {
        if (isNerve(&NrvKanina::HostTypeDamageHipDrop::sInstance) || isNerve(&NrvKanina::HostTypeDamageTrampleInvincivle::sInstance)) {
            MR::startBck(this, "HipDropDown", nullptr);
        }

        MR::startBtp(this, "Swoon");
        MR::startSound(this, "SE_EM_STOMPED_S");
        MR::tryRumblePad(this, "強", WPAD_CHAN0);
        MR::invalidateHitSensors(this);
    }

    if (MR::isStep(this, ::sHipDropDownTime)) {
        appearItemTrample();
        kill();
    }
}

void Kanina::exeDamageFireBall() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "PunchDown", nullptr);
        MR::startBtp(this, "Swoon");
        MR::startBlowHitSound(this);
        MR::invalidateClipping(this);
        MR::invalidateHitSensors(this);
    }

    if (MR::isStep(this, ::sPunchDamageTime)) {
        MR::validateClipping(this);
        MR::validateHitSensors(this);
        appearItemPunch();
        kill();
    }
}

void Kanina::exeDig() {
    if (MR::isFirstStep(this)) {
        _DC = 0;
        MR::startBck(this, "Dig", nullptr);
    }

    MR::startLevelSound(this, "SE_EM_LV_KANINA_DIG");

    if (MR::isBckStopped(this)) {
        setNerve(&NrvKanina::HostTypeWaitUnderGround::sInstance);
    }
}

void Kanina::exeWaitUnderGround() {
    if (MR::isFirstStep(this)) {
        MR::zeroVelocity(this);
        MR::invalidateShadow(this, nullptr);
        mPosition.set(_98);
    }

    if (MR::isGreaterStep(this, ::sWaitUnderGroundTime) && ::sKeepUnderGroundDistance < MR::calcDistanceToPlayer(this)) {
        MR::validateShadow(this, nullptr);
        setNerve(&NrvKanina::HostTypeAppear::sInstance);
    }
}

void Kanina::exeFindPlayer() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Search", nullptr);
        MR::startSound(this, "SE_EV_KANINA_FIND");
        MR::calcVecToPlayerH(&_D0, this, nullptr);
    }

    if (MR::isBckStopped(this)) {
        setNerve(&NrvKanina::HostTypeRunAway::sInstance);
    }
}

void Kanina::exeTurn() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Turn", nullptr);
    }

    if (MR::turnDirectionToTargetUseGroundNormalDegree(this, &_D0, *MR::getPlayerPos(), ::sTurnSpeed)) {
        setNerve(&NrvKanina::HostTypeTurnEnd::sInstance);
    }
}

void Kanina::exeTurnEnd() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait", nullptr);
    }

    if (MR::isStep(this, ::sTurnEndTime)) {
        setNerve(&NrvKanina::HostTypeRunAway::sInstance);
    }
}

void Kanina::exePointing() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Guard", nullptr);
        MR::startDPDHitSound();
    }

    if (MR::isStarPointerPointing2POnPressButton(this, "弱", true, false)) {
        MR::startDPDFreezeLevelSound(this);
        MR::tryEmitEffect(this, "Touch");
    }

    if (!MR::isStarPointerPointing2POnPressButton(this, "弱", true, false)) {
        setNerve(&NrvKanina::HostTypePointingEnd::sInstance);
    }
}

void Kanina::endPointing() {
    MR::tryDeleteEffect(this, "Touch");
}

Kanina::~Kanina() {
}
