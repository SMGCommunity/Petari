#include "Game/Enemy/BombHei.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/WalkerStateBindStarPointer.hpp"
#include "Game/LiveActor/Binder.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Map/CollisionParts.hpp"

namespace {
    static const s32 hStartBrkTime = 600;
    static const s32 hBrkRateUpTime = 120;
    static const f32 hBrkSlowRate = 0.5f;
    static const f32 hBrkFastRate = 2.0f;
    static const f32 hTurnLimitRadian = 3.0f;
    static const f32 hGravity = 1.0f;
    static const f32 hGravityPhysicsOnGround = 0.5f;
    static const f32 hSpinHitJumpVel = 10.0f;
    static const f32 hPushedAcc = 2.0f;
    // static const f32 hNearDistance =  // TODO: some combination of these two is used in exeExplode
    // static const f32 hFarDistance =
    static const s32 hExplodeTime = 8;
    // static const s32 hHideTime =
    static const s32 hWaitTime = 180;
    static const s32 hWalkTime = 160;
    static const f32 hWalkVel = 0.5f;
    static const f32 hPursueVel = 1.5f;
    static const f32 hPursueVelFast = 2.0f;
    static const f32 hPrePursueJumpVel = 10.0f;
    // static const f32 hPursueHitJumpVel =
    // static const f32 hPursueHitBackVelH =
    static const s32 hPursueHitTime = 38;
    // static const f32 hRecoverRotateRate =
    static const f32 hGravityThrown = 0.75;
    static const f32 hThrownDampVel = 0.98f;
    static const f32 hThrownReboundRate = 0.8f;
    static const f32 hThrownReboundMin = 0.0f;
    // static const f32 hThrownToStopValue =
    static const f32 hTurnVecValue = 2.0f;
    static const s32 hThrownCanNotGotTime = 120;
    static const f32 hMultVelAngle = 1.0f;
    static const s32 hThrownOffCalcGravityTime = 30;
    static const f32 hGroundCosine = 0.75f;
    // static const s32 hStaticTime =
    static const f32 hStopDampValue = 0.8f;
    static const f32 hToStopVelLen = 5.0f;
    static const f32 hThrowVelH = 25.0f;
    static const f32 hThrowVelV = 18.0f;
    static const f32 hTranslateHeight = 53.0f;
    static const s32 hStopFrame = 3;
    static const s32 hSpinHitCanGetTime = 30;
    static const s32 hTrampleTime = 60;

    const BombHeiParam hNoMoveNoTurnParam = {0.0f, 2.0f, 0.8f, 0.0f};
    const BombHeiParam hWalkParam = {hWalkVel, 2.0f, 0.8f, hTurnVecValue};
    // const BombHeiParam hPursueParam =
    const BombHeiParam hPursueFarParam = {hPursueVel, 2.0f, 0.8f, hTurnVecValue + 1.0f};
    const BombHeiParam hPursueFastFarParam = {hPursueVelFast, 2.0f, 0.8f, hTurnVecValue + 1.0f};
    // const BombHeiParam hPursueNearParam =
    // const BombHeiParam hPursueWaitParam =
    static s32 hCountTimer = hStartBrkTime;
}  // namespace

namespace NrvBombHei {
    NEW_NERVE(HostTypeNrvLaunch, BombHei, Launch);
    NEW_NERVE(HostTypeNrvStarting, BombHei, Starting);
    NEW_NERVE(HostTypeNrvWait, BombHei, Wait);
    NEW_NERVE(HostTypeNrvWalk, BombHei, Walk);
    NEW_NERVE(HostTypeNrvPursue, BombHei, Pursue);
    NEW_NERVE(HostTypeNrvPursueFast, BombHei, Pursue);
    NEW_NERVE(HostTypeNrvPursueHit, BombHei, PursueHit);
    NEW_NERVE_ONEND(HostTypeNrvSpinHit, BombHei, SpinHit, SpinHit);
    NEW_NERVE(HostTypeNrvTrample, BombHei, Trample);
    NEW_NERVE(HostTypeNrvPhysics, BombHei, Physics);
    NEW_NERVE(HostTypeNrvStop, BombHei, Stop);
    NEW_NERVE_ONEND(HostTypeNrvTaken, BombHei, Taken, Taken);
    NEW_NERVE_ONEND(HostTypeNrvThrown, BombHei, Thrown, Thrown);
    NEW_NERVE(HostTypeNrvExplode, BombHei, Explode);
    NEW_NERVE_ONEND(HostTypeNrvBindStarPointer, BombHei, BindStarPointer, BindStarPointer);

}  // namespace NrvBombHei

BombHei::BombHei(const char* pName)
    : LiveActor(pName), mScaleController(nullptr), mBindStarPointerState(nullptr), mRotQuat(0.0f, 0.0f, 0.0f, 1.0f), mFront(0, 0, 1),
      mBinderOffset(0, 1, 0), mTargetFront(0, 0, 1), mCountdown(-1), mCarrySensor(nullptr), mExplodeRadius(500.0f), mBodyJoint(nullptr),
      mType(BOMB_HEI), mExplosionIgnoreCollision(false) {
}

void BombHei::init(const JMapInfoIter& rIter) {
    if (MR::isValidInfo(rIter)) {
        MR::initDefaultPos(this, rIter);

        s32 detonateTime = hStartBrkTime;
        MR::getJMapInfoArg0NoInit(rIter, &detonateTime);
        hCountTimer = detonateTime;

        s32 rangeArg = mExplodeRadius;
        MR::getJMapInfoArg1NoInit(rIter, &rangeArg);
        mExplodeRadius = rangeArg;

        s32 ignoreCollisionArg = -1;
        MR::getJMapInfoArg2NoInit(rIter, &ignoreCollisionArg);
        if (ignoreCollisionArg != -1) {
            mExplosionIgnoreCollision = true;
        }

        const char* objName = nullptr;
        MR::getObjectName(&objName, rIter);
        if (MR::isEqualString(objName, "Bomb")) {
            mType = BOMB;
        } else {
            mType = BOMB_HEI;
        }

        setGroupClipping(rIter);
    }

    MR::useStageSwitchWriteDead(this, rIter);
    MR::useStageSwitchReadA(this, rIter);

    initModelManagerWithAnm("BombHei", nullptr, false);
    MR::connectToSceneEnemy(this);
    MR::initLightCtrl(this);

    mRotQuat.set< f32 >(0.0f, 0.0f, 0.0f, 1.0f);
    mRotQuat.getZDir(mFront);
    mTargetFront.set(mFront);

    initBinder(hTranslateHeight, hTranslateHeight, 0);
    MR::setBinderOffsetVec(this, &mBinderOffset, false);
    MR::onCalcGravity(this);

    initSound(4, false);
    initEffectKeeper(1, nullptr, false);

    initHitSensor(2);
    f32 scl = mScale.y;
    MR::addHitSensor(this, "body", ATYPE_BOMBHEI, 16, scl * 60.0f, TVec3f(0.0f, scl * 60.0f, 0.0f));
    MR::addHitSensorEnemyAttack(this, "explode", 32, mExplodeRadius * scl, TVec3f(0.0f, scl * 200.0f, 0.0f));

    if (mType == BOMB) {
        initNerve(&NrvBombHei::HostTypeNrvPhysics::sInstance);
    } else {
        initNerve(&NrvBombHei::HostTypeNrvWait::sInstance);
    }

    MR::initShadowVolumeSphere(this, 60.0f);
    endCountdown();
    MR::validateClipping(this);
    MR::initStarPointerTarget(this, 70.0f, TVec3f(0.0f, 35.0f, 0.0f));

    mBodyJoint = MR::createJointDelegatorWithNullChildFunc(this, &BombHei::calcJoint, "Body");

    mScaleController = new AnimScaleController(nullptr);
    mScaleController->setParamTight();

    mBindStarPointerState = new WalkerStateBindStarPointer(this, mScaleController);

    appear();
}

void BombHei::setGroupClipping(const JMapInfoIter& rIter) {
    MR::setGroupClipping(this, rIter, 32);
}

void BombHei::initAfterPlacement() {
    MR::trySetMoveLimitCollision(this);
}

void BombHei::appear() {
    LiveActor::appear();
    getSensor("explode")->invalidate();
    getSensor("body")->validate();

    if (mType == BOMB_HEI || mType == BOMB) {
        setNerve(&NrvBombHei::HostTypeNrvLaunch::sInstance);
    }

    MR::showModel(this);
}

void BombHei::kill() {
    LiveActor::kill();
    if (MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }
    endCountdown();
}

void BombHei::startCountdown() {
    if (mCountdown == -1) {
        MR::invalidateClipping(this);
        mCountdown = hCountTimer;
        MR::emitEffect(this, "Ignite");
    }
}

void BombHei::endCountdown() {
    mCountdown = -1;
    MR::startBrk(this, "Wait");
    MR::setBrkRate(this, 1.0f);
    MR::deleteEffect(this, "Ignite");
    MR::validateClipping(this);
}

void BombHei::exeLaunch() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Stop", nullptr);
        mRotQuat.set< f32 >(0.0f, 0.0f, 0.0f, 1.0f);
    }

    TVec3f velH(0, 0, 0);
    MR::vecKillElement(mVelocity, mGravity, &velH);
    mTargetFront.set(velH);
    MR::normalizeOrZero(&mTargetFront);

    if (!MR::isNearZero(mTargetFront)) {
        MR::turnVecToVecRadian(&mFront, mFront, mTargetFront, hTurnLimitRadian, mGravity);
    }

    mVelocity.mult(0.98f);

    if (!MR::isOnGroundCos(this, hGroundCosine)) {
        mVelocity.add(mGravity.multiplyOperatorInline(hGravity));
        f32 radius = getSensor("body")->mRadius;
        MR::rotateQuatRollBall(&mRotQuat, velH.multiplyOperatorInline2(hMultVelAngle), -mGravity, radius);
    } else {
        startBoundSound();
        f32 radius = getSensor("body")->mRadius;
        MR::rotateQuatRollBall(&mRotQuat, mVelocity, *MR::getGroundNormal(this), radius);

        if (mGravity.dot(mVelocity) < 5.0f) {
            if (mType == BOMB_HEI) {
                setNerve(&NrvBombHei::HostTypeNrvStarting::sInstance);
            } else if (mType == BOMB) {
                setNerve(&NrvBombHei::HostTypeNrvPhysics::sInstance);
            }
            return;
        }
    }

    MR::reboundVelocityFromCollision(this, 0.1f, 6.0f, 1.0f);
}

void BombHei::exeStarting() {
    if (MR::isFirstStep(this)) {
        MR::validateExCollisionParts(this);
        MR::startBck(this, "Starting", nullptr);
        mVelocity.z = 0.0f;
        mVelocity.y = 0.0f;
        mVelocity.x = 0.0f;
    }

    if (!MR::isNearZero(mGravity)) {
        MR::blendQuatUpFront(&mRotQuat, -mGravity, mFront, 0.5f, 0.5f);
    }

    if (MR::isBckStopped(this)) {
        setNerve(&NrvBombHei::HostTypeNrvWait::sInstance);
    }
}

void BombHei::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait", nullptr);
    }

    MR::moveAndTurnToPlayer(this, &mFront, hNoMoveNoTurnParam.mSpeedH, hNoMoveNoTurnParam.mGravAccel, hNoMoveNoTurnParam.mFriction,
                            hNoMoveNoTurnParam.mTurnRate);

    if (MR::isGreaterStep(this, hWaitTime)) {
        setNerve(&NrvBombHei::HostTypeNrvWalk::sInstance);
        return;
    }

    if (!MR::isNearZero(mGravity)) {
        MR::blendQuatUpFront(&mRotQuat, -mGravity, mFront, 0.5f, 0.5f);
    }

    if (MR::calcDistanceToPlayer(this) < 1000.0f && (!MR::isValidSwitchA(this) || MR::isOnSwitchA(this))) {
        if (mCountdown > 0 && mCountdown < hBrkRateUpTime) {
            setNerve(&NrvBombHei::HostTypeNrvPursueFast::sInstance);
        } else {
            setNerve(&NrvBombHei::HostTypeNrvPursue::sInstance);
        }
    }
}

void BombHei::exeWalk() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Walk", nullptr);
        TVec3f randDir;
        MR::getRandomVector(&randDir, 1.0f);
        MR::vecKillElement(randDir, mGravity, &randDir);
        MR::normalizeOrZero(&randDir);
        if (!MR::isNearZero(randDir)) {
            mTargetFront.set(randDir);
        }
    }

    MR::moveAndTurnToDirection(this, &mFront, mTargetFront, hWalkParam.mSpeedH, hWalkParam.mGravAccel, hWalkParam.mFriction, hWalkParam.mTurnRate);

    if (MR::isGreaterStep(this, hWalkTime)) {
        setNerve(&NrvBombHei::HostTypeNrvWait::sInstance);
        return;
    }

    if (!MR::isNearZero(mGravity)) {
        MR::blendQuatUpFront(&mRotQuat, -mGravity, mFront, 0.5f, 0.5f);
    }

    if (MR::calcDistanceToPlayer(this) < 1000.0f && (!MR::isValidSwitchA(this) || MR::isOnSwitchA(this))) {
        if (mCountdown > 0 && mCountdown < hBrkRateUpTime) {
            setNerve(&NrvBombHei::HostTypeNrvPursueFast::sInstance);
        } else {
            setNerve(&NrvBombHei::HostTypeNrvPursue::sInstance);
        }
    }
}

void BombHei::exePursue() {
    if (MR::isFirstStep(this)) {
        startCountdown();
        MR::invalidateClipping(this);
        mVelocity.set(mGravity.multiplyOperatorInline(-hPrePursueJumpVel));
    }

    // NOTE: this is a dummy line to emit the structure, this value is not real
    if (MR::calcDistanceToPlayer(this) < 0.0f) {
    }

    if (isNerve(&NrvBombHei::HostTypeNrvPursueFast::sInstance)) {
        MR::tryStartBck(this, "CountDown", nullptr);
        MR::moveAndTurnToPlayer(this, &mFront, hPursueFastFarParam.mSpeedH, hPursueFastFarParam.mGravAccel, hPursueFastFarParam.mFriction,
                                hPursueFastFarParam.mTurnRate);
    } else {
        MR::tryStartBck(this, "Run", nullptr);
        MR::moveAndTurnToPlayer(this, &mFront, hPursueFarParam.mSpeedH, hPursueFarParam.mGravAccel, hPursueFarParam.mFriction,
                                hPursueFarParam.mTurnRate);
    }

    if (isNerve(&NrvBombHei::HostTypeNrvPursue::sInstance) && mCountdown > 0 && mCountdown < hBrkRateUpTime) {
        setNerve(&NrvBombHei::HostTypeNrvPursueFast::sInstance);
        return;
    }

    if (!MR::isNearZero(mGravity)) {
        MR::blendQuatUpFront(&mRotQuat, -mGravity, mFront, 0.5f, 0.5f);
    }
}

void BombHei::exePursueHit() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Hit", nullptr);
        MR::startSound(this, "SE_EM_BOMBHEI_HIT", -1, -1);
    }

    MR::moveAndTurnToPlayer(this, &mFront, hNoMoveNoTurnParam.mSpeedH, hNoMoveNoTurnParam.mGravAccel, hNoMoveNoTurnParam.mFriction,
                            hNoMoveNoTurnParam.mTurnRate);

    if (!MR::isNearZero(mGravity)) {
        MR::blendQuatUpFront(&mRotQuat, -mGravity, mFront, 0.5f, 0.5f);
    }

    if (MR::isGreaterStep(this, hPursueHitTime)) {
        if (mCountdown > 0 && mCountdown < hBrkRateUpTime) {
            setNerve(&NrvBombHei::HostTypeNrvPursueFast::sInstance);
        } else {
            setNerve(&NrvBombHei::HostTypeNrvPursue::sInstance);
        }
    }
}

void BombHei::exeSpinHit() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Spin", nullptr);
        MR::startBlowHitSound(this);
        mVelocity.set(mGravity.multiplyOperatorInline(-hSpinHitJumpVel));
        MR::invalidateExCollisionParts(this);
    }

    MR::moveAndTurnToPlayer(this, &mFront, hNoMoveNoTurnParam.mSpeedH, hNoMoveNoTurnParam.mGravAccel, hNoMoveNoTurnParam.mFriction,
                            hNoMoveNoTurnParam.mTurnRate);

    if (!MR::isNearZero(mGravity)) {
        MR::blendQuatUpFront(&mRotQuat, -mGravity, mFront, 0.5f, 0.5f);
    }

    if (MR::isOnGroundCos(this, hGroundCosine)) {
        if (MR::isBckStopped(this)) {
            setNerve(&NrvBombHei::HostTypeNrvPhysics::sInstance);
        }
    } else {
        mVelocity.add(mGravity.multiplyOperatorInline(hGravity));
    }
}

void BombHei::endSpinHit() {
}

void BombHei::exeTrample() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Pressed");
        MR::startSound(this, "SE_EM_BOMBHEI_TRAMPLE", -1, -1);
    }

    MR::addVelocityToGravityOrGround(this, hGravity);
    MR::killVelocityOnGroundCosH(this, hGroundCosine);

    if (MR::isGreaterStep(this, hTrampleTime)) {
        setNerve(&NrvBombHei::HostTypeNrvPursue::sInstance);
    }
}

void BombHei::exePhysics() {
    if (MR::isFirstStep(this)) {
        MR::invalidateExCollisionParts(this);
    }

    TVec3f velH(0, 0, 0);
    MR::vecKillElement(mVelocity, mGravity, &velH);
    mTargetFront.set(velH);
    MR::normalizeOrZero(&mTargetFront);

    if (!MR::isNearZero(mTargetFront) && 2.0f < velH.length()) {
        MR::turnVecToVecRadian(&mFront, mFront, mTargetFront, hTurnLimitRadian, mGravity);
    }

    mVelocity.mult(0.98f);

    if (!MR::isOnGroundCos(this, hGroundCosine)) {
        mVelocity.add(mGravity.multiplyOperatorInline(hGravity));
        f32 radius = getSensor("body")->mRadius;
        MR::rotateQuatRollBall(&mRotQuat, velH.multiplyOperatorInline2(hMultVelAngle), mGravity, radius);
    } else {
        mVelocity.add(mGravity.multiplyOperatorInline(hGravityPhysicsOnGround));
        f32 radius = getSensor("body")->mRadius;
        MR::rotateQuatRollBall(&mRotQuat, mVelocity, *MR::getGroundNormal(this), radius);

        if (MR::isBckStopped(this) && mVelocity.length() < hToStopVelLen) {
            setNerve(&NrvBombHei::HostTypeNrvStop::sInstance);
            return;
        }

        mBinder->_1EC._5 = 1;
    }

    MR::reboundVelocityFromCollision(this, 0.1f, 6.0f, 1.0f);
    MR::killVelocityOnGroundCosH(this, hGroundCosine);
}

void BombHei::exeStop() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Stop", nullptr);
    }

    MR::addVelocityToGravityOrGround(this, hGravity);
    MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);

    if (MR::isOnGround(this)) {
        MR::attenuateVelocity(this, hStopDampValue);
        f32 radius = getSensor("body")->mRadius;
        MR::rotateQuatRollBall(&mRotQuat, mVelocity, *MR::getGroundNormal(this), radius);
    } else {
        MR::attenuateVelocity(this, 0.98f);
        f32 radius = getSensor("body")->mRadius;
        MR::rotateQuatRollBall(&mRotQuat, mVelocity, -mGravity, radius);
    }

    mBinder->_1EC._5 = 1;
}

void BombHei::exeThrown() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Stop", nullptr);
        MR::startSoundPlayer("SE_PV_THROW", -1);
        MR::startSound(this, "SE_EM_BOMB_THROW", -1, -1);
        MR::invalidateExCollisionParts(this);
        MR::offCalcGravity(this);
    }

    TVec3f velH(0, 0, 0);
    MR::vecKillElement(mVelocity, mGravity, &velH);
    f32 radius = getSensor("body")->mRadius;
    MR::rotateQuatRollBall(&mRotQuat, velH.multiplyOperatorInline(hMultVelAngle), mGravity, radius);
    mTargetFront.set(velH);
    MR::normalizeOrZero(&mTargetFront);

    if (!MR::isNearZero(mTargetFront)) {
        MR::turnVecToVecRadian(&mFront, mFront, mTargetFront, hTurnLimitRadian, mGravity);
    }

    if (MR::isGreaterStep(this, hThrownOffCalcGravityTime)) {
        MR::onCalcGravity(this);
    }

    mVelocity.mult(hThrownDampVel);

    if (!MR::isOnGroundCos(this, hGroundCosine)) {
        mVelocity.add(mGravity.multiplyOperatorInline2(hGravityThrown));
    } else {
        startBoundSound();
        MR::startBck(this, "Bound", nullptr);
        setNerve(&NrvBombHei::HostTypeNrvPhysics::sInstance);
        return;
    }

    MR::reboundVelocityFromCollision(this, 0.1f, 6.0f, 1.0f);
}

void BombHei::endThrown() {
    MR::onCalcGravity(this);
}

void BombHei::exeTaken() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Carry", nullptr);
        if (MR::sendMsgTaken(mCarrySensor, getSensor("body"))) {
            getSensor("body")->invalidate();
            MR::startSoundPlayer("SE_PV_LIFT_UP", -1);
            MR::startSound(this, "SE_EM_BOMB_LIFT", -1, -1);
        } else {
            setNerve(&NrvBombHei::HostTypeNrvPhysics::sInstance);
            return;
        }
    }

    mRotQuat.setEulerDegree(mRotation.x, mRotation.y, mRotation.z);
}

void BombHei::endTaken() {
    getSensor("body")->validate();
}

void BombHei::exeExplode() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, "Explosion");
        MR::startSound(this, "SE_EM_BOMB_EXPLOSION", -1, -1);
        MR::hideModel(this);
        getSensor("explode")->validate();
        getSensor("body")->invalidate();
        if (mCarrySensor != nullptr) {
            MR::sendArbitraryMsg(ACTMES_RUSHDROP, mCarrySensor, getSensor("body"));
            MR::sendMsgEnemyAttackExplosion(mCarrySensor, getSensor("body"));
        }
        mCarrySensor = nullptr;
        endCountdown();
        MR::tryRumblePadAndCameraDistanceStrong(this, 800.0f, 1200.0f, 2000.0f);  // TODO
        MR::invalidateClipping(this);
    }

    getSensor("explode")->mRadius = getNerveStep() * mExplodeRadius / 8;

    if (MR::isGreaterStep(this, hExplodeTime)) {
        MR::validateClipping(this);
        kill();
    }
}

void BombHei::exeBindStarPointer() {
    MR::updateActorStateAndNextNerve(this, mBindStarPointerState, &NrvBombHei::HostTypeNrvWait::sInstance);
}

void BombHei::endBindStarPointer() {
    mBindStarPointerState->kill();
}

void BombHei::calcAndSetBaseMtx() {
    TPos3f mtx;
    MR::calcMtxFromGravityAndZAxis(&mtx, this, mGravity, mFront);
    MR::setBaseTRMtx(this, mtx);
    MR::setBaseScale(this, mScaleController->_C.mult(mScale));
    mtx.getYDirInline(mBinderOffset);
    mBinderOffset.mult(hTranslateHeight);
    mBodyJoint->registerCallBack();
}

bool BombHei::calcJoint(TPos3f* pMtx, const JointControllerInfo& rJointInfo) {
    TPos3f mtx;
    mtx.zeroTransInline();
    mtx.fromQuat(mRotQuat);

    // interesting way to transpose a matrix
    TPos3f mtx2;
    TVec3f x, y, z;
    mtx2.setInline(getBaseMtx());
    mtx2.getXYZDirInline(x, y, z);
    TPos3f mtx3;
    mtx3.setXYZDirInline2(x, y, z);

    TVec3f t;
    pMtx->getTransInline(t);

    pMtx->zeroTransInline2();
    pMtx->concat(mtx3, *pMtx);
    pMtx->concat(mtx, *pMtx);
    pMtx->setTransInline(t);

    return true;
}

void BombHei::control() {
    mScaleController->updateNerve();

    if (MR::isInDeath(this, TVec3f(0.0f, 0.0f, 0.0f)) && !isNerve(&NrvBombHei::HostTypeNrvExplode::sInstance)) {
        setNerve(&NrvBombHei::HostTypeNrvExplode::sInstance);
        return;
    }

    if (mCountdown > 0) {
        if (mCountdown == hCountTimer) {
            MR::startBrk(this, "CountDown");
            MR::setBrkRate(this, hBrkSlowRate);
        } else if (mCountdown == hBrkRateUpTime) {
            MR::startBrk(this, "CountDown");
            MR::setBrkRate(this, hBrkFastRate);
        }

        MR::startLevelSound(this, "SE_EM_LV_BOMB_FUSE", -1, -1, -1);

        if (mCountdown <= hBrkRateUpTime) {
            MR::startLevelSound(this, "SE_EM_LV_BOMB_ALARM_FAST", -1, -1, -1);
        } else if (mCountdown <= hStartBrkTime / 2) {
            MR::startLevelSound(this, "SE_EM_LV_BOMB_ALARM_MIDDLE", -1, -1, -1);
        } else {
            MR::startLevelSound(this, "SE_EM_LV_BOMB_ALARM_SLOW", -1, -1, -1);
        }

        if (--mCountdown == 0) {
            setNerve(&NrvBombHei::HostTypeNrvExplode::sInstance);
            return;
        }
    }

    if (mType != BOMB &&
        (isNerve(&NrvBombHei::HostTypeNrvWait::sInstance) || isNerve(&NrvBombHei::HostTypeNrvWalk::sInstance) ||
         isNerve(&NrvBombHei::HostTypeNrvPursue::sInstance) || isNerve(&NrvBombHei::HostTypeNrvPursueFast::sInstance) ||
         isNerve(&NrvBombHei::HostTypeNrvPursueHit::sInstance)) &&
        mBindStarPointerState->tryStartPointBind()) {
        setNerve(&NrvBombHei::HostTypeNrvBindStarPointer::sInstance);
    }
}

void BombHei::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (getSensor("explode") == pSender) {
        if (!mExplosionIgnoreCollision && MR::isExistCollisionParts(pReceiver->mHost) && MR::isValidCollisionParts(pReceiver->mHost)) {
            Triangle hitPoly;

            if (!MR::getFirstPolyOnLineToMap(nullptr, &hitPoly, pSender->mPosition, pReceiver->mPosition.subOtherInline(pSender->mPosition))) {
                return;
            }

            if (hitPoly.mParts->mHitSensor->mHost != pReceiver->mHost) {
                return;
            }
        }

        if (MR::isSensorPlayer(pReceiver)) {
            MR::sendMsgEnemyAttackExplosion(pReceiver, pSender);
        } else if (MR::isSensorEnemy(pReceiver)) {
            MR::sendMsgEnemyAttackExplosion(pReceiver, pSender);
        } else if (MR::isSensorMapObj(pReceiver)) {
            MR::sendMsgEnemyAttackExplosion(pReceiver, pSender);
        }
        return;
    }

    if (isNerve(&NrvBombHei::HostTypeNrvLaunch::sInstance) || isNerve(&NrvBombHei::HostTypeNrvExplode::sInstance)) {
        return;
    }

    if (getSensor("body") != pSender) {
        return;
    }

    if (MR::isSensorPlayer(pReceiver)) {
        if (MR::sendMsgPushAndKillVelocityToTarget(this, pReceiver, pSender)) {
            startCountdown();
        }
        return;
    }

    if (MR::isSensorRide(pReceiver)) {
        setNerve(&NrvBombHei::HostTypeNrvExplode::sInstance);
        return;
    }

    if (MR::isSensorNpc(pReceiver) || MR::isSensorEnemy(pReceiver)) {
        if (isNerve(&NrvBombHei::HostTypeNrvThrown::sInstance)) {
            TVec3f direction(pSender->mPosition.subOtherInline(pReceiver->mPosition));
            MR::normalizeOrZero(&direction);
            MR::calcReflectionVector(&mVelocity, direction, hThrownReboundRate, hThrownReboundMin);
        } else {
            MR::sendMsgPushAndKillVelocityToTarget(this, pReceiver, pSender);
        }
    }
}

bool BombHei::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvBombHei::HostTypeNrvExplode::sInstance)) {
        return false;
    }

    if (MR::isMsgStarPieceReflect(msg)) {
        return true;
    }

    if (MR::isMsgPlayerTrample(msg) || MR::isMsgPlayerHipDrop(msg)) {
        if (isNerve(&NrvBombHei::HostTypeNrvThrown::sInstance) || isNerve(&NrvBombHei::HostTypeNrvPhysics::sInstance) ||
            isNerve(&NrvBombHei::HostTypeNrvStop::sInstance) || isNerve(&NrvBombHei::HostTypeNrvSpinHit::sInstance)) {
            return false;
        }

        if (isNerve(&NrvBombHei::HostTypeNrvLaunch::sInstance)) {
            if (MR::isMsgPlayerHipDrop(msg)) {
                MR::sendMsgAwayJump(pSender, pReceiver);
            }
            return true;
        }

        startCountdown();
        setNerve(&NrvBombHei::HostTypeNrvTrample::sInstance);
        if (MR::isMsgPlayerHipDrop(msg)) {
            MR::sendMsgAwayJump(pSender, pReceiver);
        }
        return true;
    }

    if (MR::isMsgPlayerSpinAttack(msg) && mType == BOMB_HEI && !isNerve(&NrvBombHei::HostTypeNrvSpinHit::sInstance)) {
        setNerve(&NrvBombHei::HostTypeNrvSpinHit::sInstance);
        startCountdown();
        MR::stopScene(hStopFrame);
        return true;
    }

    return false;
}

bool BombHei::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgExplosionAttack(msg) && !isNerve(&NrvBombHei::HostTypeNrvExplode::sInstance)) {
        setNerve(&NrvBombHei::HostTypeNrvExplode::sInstance);
        return true;
    }

    if ((MR::isMsgEnemyAttack(msg) || MR::isMsgToEnemyAttackBlow(msg) || MR::isMsgToEnemyAttackTrample(msg)) &&
        !isNerve(&NrvBombHei::HostTypeNrvExplode::sInstance) && pSender->isType(ATYPE_WANWAN)) {
        setNerve(&NrvBombHei::HostTypeNrvExplode::sInstance);
        return true;
    }

    return false;
}

bool BombHei::receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver) {
    if (!MR::isSensorPlayer(pSender) && !MR::isSensorEnemy(pSender) && !MR::isSensorMapObj(pSender)) {
        return false;
    }

    if (isNerve(&NrvBombHei::HostTypeNrvSpinHit::sInstance)) {
        return false;
    }

    MR::addVelocityFromPush(this, hPushedAcc, pSender, pReceiver);
    return true;
}

bool BombHei::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgItemGet(msg) && getSensor("body") == pReceiver) {
        if (!isNerve(&NrvBombHei::HostTypeNrvStop::sInstance) && !isNerve(&NrvBombHei::HostTypeNrvPhysics::sInstance) &&
            !isNerve(&NrvBombHei::HostTypeNrvThrown::sInstance) &&
            !(isNerve(&NrvBombHei::HostTypeNrvSpinHit::sInstance) && MR::isGreaterStep(this, hSpinHitCanGetTime))) {
            return false;
        }

        if (isNerve(&NrvBombHei::HostTypeNrvThrown::sInstance) && MR::isLessEqualStep(this, hThrownCanNotGotTime)) {
            return false;
        }

        mCarrySensor = pSender;
        setNerve(&NrvBombHei::HostTypeNrvTaken::sInstance);
        startCountdown();
        return true;
    }

    if (msg == ACTMES_DAMAGEDROP || msg == ACTMES_RUSHDROP || msg == ACTMES_ATTACKDROP) {
        if (isNerve(&NrvBombHei::HostTypeNrvTaken::sInstance)) {
            setNerve(&NrvBombHei::HostTypeNrvThrown::sInstance);
        }
        return true;
    }

    if (msg == ACTMES_TAKE_TOUCH) {
        MR::startBck(this, "Carry", nullptr);
        return true;
    }

    return false;
}

bool BombHei::receiveMsgThrow(HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvBombHei::HostTypeNrvExplode::sInstance)) {
        return true;
    }

    setNerve(&NrvBombHei::HostTypeNrvThrown::sInstance);
    mCarrySensor = nullptr;
    TVec3f throwVec;
    MR::getPlayerThrowVec(&throwVec);
    mVelocity.set(throwVec.multInLine(hThrowVelH));
    mVelocity.add(mGravity.multInLine2(-hThrowVelV));
    return true;
}

void BombHei::startBoundSound() {
    s32 level = mGravity.dot(mVelocity) * 100.0f;
    MR::startSound(this, "SE_EM_BOMB_BOUND", level, -1);
}
