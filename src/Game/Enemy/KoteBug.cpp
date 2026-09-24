#include "Game/Enemy/KoteBug.hpp"
#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/AnimStampController.hpp"
#include "Game/Enemy/SpinHitController.hpp"
#include "Game/Enemy/WalkerStateBindStarPointer.hpp"
#include "Game/LiveActor/Binder.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"
#include "Game/Util/JointController.hpp"

void KoteBug_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)-1.0f;
    (void)3.1415927f;
    (void)2.0f;
}

namespace {
    const f32 hCommonGravity = 1.5f;
    const f32 hGroundDamp = 0.8f;
    const f32 hAirDampH = 0.96f;
    const f32 hAirDampV = 0.93f;
    // hOnGroundTime
    const f32 hPushedAcc = 2.0f;
    const f32 hSearchDistance = 1000.0f;
    const f32 hFlyDistance = 2000.0f;
    const f32 hFlyPursueVel = 0.2f;
    const f32 hFlyPursueLastSubVel = 0.01f;
    // hFlyRateV
    const f32 hFlyHeight = 100.0f;
    const s32 hFlyStartTime = 20;
    const s32 hFlyPursueTime = 200;
    const f32 hKeepAcc = 1.8f;
    const f32 hFreqRange = 40.0f;
    const f32 hOverturnFallJumpVelV = 30.0f;
    const s32 hOverturnTime = 240;
    const f32 hRecoverJumpVelV = 24.0f;
    const volatile f32 hDistNoCalcWait = 1000.0f;
    const s32 hSeachTime = 50;
    const s32 hEscapeSeachTime = 20;
    const f32 hPreEscapeTurnLimitDegree = 9.0f;
    const s32 hFlyPursueLastTime = 90;
    const f32 hFlyJitabataHeightBase = 60.0f;
    const f32 hFlyJitabataHeight = 150.0f;
    const f32 hJitabataPostureRate = 0.02f;
    const f32 hJitabataAngle = -1.8f;
    const f32 hFlyPursueLastAddRate = 1.0f;
    const f32 hHitToPlayerJumpVelV = 10.0f;
    const f32 hHitToPlayerJumpVelH = -10.0f;
    const f32 hHitScaleYSpeed = 0.1f;
    // hStampFallVel
    const s32 hStampDeathTime = 20;
    // hHitBlowGravity
    // hTimeHitPunch
    // hPunchHitVelV
    // hPunchHitVelH
    // hCoinGenPunchTime
    const s32 hBlowStopSceneTime = 3;
    const f32 hSearchTurnLimitDegree = 5.7f;
    const f32 hFlyTurnLimitDegree = 5.7f;
    const f32 hFlyEscapeTurnLimitDegree = 11.5f;
    const f32 hOverturnRotateRate = 0.15f;
    const f32 hRecoverRotateRate = 0.2f;
    const f32 hBlowRotateAngle = -1.0f;
    // hStampDeathOffset
}  // namespace

namespace NrvKoteBug {
    NEW_NERVE_ONEND(HostTypeNrvNoCalcWait, KoteBug, NoCalcWait, NoCalcWait);
    NEW_NERVE_ONEND(HostTypeNrvWait, KoteBug, Wait, Wait);
    NEW_NERVE(HostTypeNrvSearch, KoteBug, Search);
    NEW_NERVE(HostTypeNrvEscapeSearch, KoteBug, EscapeSearch);
    NEW_NERVE(HostTypeNrvPreEscape, KoteBug, PreEscape);
    NEW_NERVE(HostTypeNrvFlyStart, KoteBug, FlyStart);
    NEW_NERVE(HostTypeNrvFlyPursue, KoteBug, FlyPursue);
    NEW_NERVE(HostTypeNrvFlyEscape, KoteBug, FlyPursue);
    NEW_NERVE(HostTypeNrvFlyPursueLast, KoteBug, FlyPursueLast);
    NEW_NERVE(HostTypeNrvOverturnFall, KoteBug, OverturnFall);
    NEW_NERVE(HostTypeNrvOverturn, KoteBug, Overturn);
    NEW_NERVE(HostTypeNrvPreRecover, KoteBug, PreRecover);
    NEW_NERVE(HostTypeNrvRecover, KoteBug, Recover);
    NEW_NERVE(HostTypeNrvHitToPlayer, KoteBug, HitToPlayer);
    NEW_NERVE(HostTypeNrvBlow, KoteBug, Blow);
    NEW_NERVE(HostTypeNrvStampDeath, KoteBug, StampDeath);
    NEW_NERVE_ONEND(HostTypeNrvBindStarPointer, KoteBug, BindStarPointer, BindStarPointer);
}  // namespace NrvKoteBug

KoteBug::KoteBug(const char* pName)
    : LiveActor(pName), mScaleController(), mStampController(), mSpinHitController(), mStarPointer(), mFront(0, 0, 1), mPosture(0, 0, 0, 1),
      mJointController() {
}

void KoteBug::init(const JMapInfoIter& rIter) {
    if (MR::isValidInfo(rIter)) {
        MR::initDefaultPos(this, rIter);
        MR::useStageSwitchWriteDead(this, rIter);
        if (MR::useStageSwitchReadAppear(this, rIter)) {
            MR::syncStageSwitchAppear(this);
        }
    }

    initModelManagerWithAnm("KoteBug", nullptr, false);
    TPos3f mtx;
    mtx.set(getBaseMtx());
    mtx.getZDir(mFront);

    MR::connectToSceneEnemy(this);
    MR::initLightCtrl(this);
    f32 scale = mScale.y;
    initBinder(70.0f * scale, 70.0f * scale, 0);
    initHitSensor(1);
    MR::addHitSensorAtJointEnemy(this, "body", "Center", 32, 100.0f * scale, TVec3f(0.0f, 20.0f * scale, 0.0f));
    initEffectKeeper(0, nullptr, false);
    initSound(4, false);
    initNerve(GET_NERVE(KoteBug, HostTypeNrvWait));
    MR::initStarPointerTarget(this, 110.0f, TVec3f(0.0f, 80.0f, 0.0f));
    MR::declareStarPiece(this, 3);
    MR::declareCoin(this, 1);
    mSpinHitController = new SpinHitController(this, 3, false);
    mScaleController = new AnimScaleController(nullptr);
    mStampController = new AnimStampController();
    mStarPointer = new WalkerStateBindStarPointer(this, mScaleController);
    MR::onCalcGravity(this);
    MR::initShadowVolumeSphere(this, 70.0f * mScale.y);
    MR::validateClipping(this);
    mJointController = MR::createJointDelegatorWithNullChildFunc(this, &KoteBug::calcJoint, "Center");
    if (MR::isValidSwitchAppear(this)) {
        makeActorDead();
    } else {
        makeActorAppeared();
    }
}

void KoteBug::appear() {
    LiveActor::appear();
    MR::emitEffect(this, "Appear");
}

void KoteBug::kill() {
    LiveActor::kill();
    MR::emitEffect(this, "Death");
    if (MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }
}

void KoteBug::control() {
    if (MR::isInDeath(this, TVec3f(0.0f, 0.0f, 0.0f)) || MR::isInWater(mPosition)) {
        kill();
        return;
    }

    mScaleController->updateNerve();
    mStampController->updateNerve();
    if (isNerve(GET_NERVE(KoteBug, HostTypeNrvOverturn)) || isNerve(GET_NERVE(KoteBug, HostTypeNrvOverturnFall)) ||
        isNerve(GET_NERVE(KoteBug, HostTypeNrvPreRecover)) || isNerve(GET_NERVE(KoteBug, HostTypeNrvStampDeath))) {
        TQuat4f rotation;
        rotation.setRotate(TVec3f(1, 0, 0), 3.1415927f);
        mPosture.slerp(rotation, ::hOverturnRotateRate);
    } else if (isNerve(GET_NERVE(KoteBug, HostTypeNrvFlyPursueLast))) {
        TQuat4f rotation;
        rotation.setRotate(TVec3f(1, 0, 0), ::hJitabataAngle);
        mPosture.slerp(rotation, ::hJitabataPostureRate);
    } else {
        mPosture.slerp(TQuat4f(0, 0, 0, 1), ::hRecoverRotateRate);
    }

    if (isNerve(GET_NERVE(KoteBug, HostTypeNrvNoCalcWait)) || isNerve(GET_NERVE(KoteBug, HostTypeNrvWait)) ||
        isNerve(GET_NERVE(KoteBug, HostTypeNrvSearch)) || isNerve(GET_NERVE(KoteBug, HostTypeNrvEscapeSearch)) ||
        isNerve(GET_NERVE(KoteBug, HostTypeNrvPreEscape)) || isNerve(GET_NERVE(KoteBug, HostTypeNrvFlyStart)) ||
        isNerve(GET_NERVE(KoteBug, HostTypeNrvFlyPursue)) || isNerve(GET_NERVE(KoteBug, HostTypeNrvFlyEscape))) {
        if (mStarPointer->tryStartPointBind()) {
            setNerve(GET_NERVE(KoteBug, HostTypeNrvBindStarPointer));
        }
    }
}

void KoteBug::exeNoCalcWait() {
    if (MR::isFirstStep(this)) {
        MR::validateClipping(this);
        MR::offBind(this);
        MR::offCalcShadow(this, nullptr);
        MR::offCalcGravity(this);
    }

    mVelocity.zero();
    if (MR::calcDistanceToPlayer(mPosition) < ::hDistNoCalcWait) {
        setNerve(GET_NERVE(KoteBug, HostTypeNrvWait));
    }
}

void KoteBug::endNoCalcWait() {
    MR::onBind(this);
    MR::onCalcShadow(this, nullptr);
    MR::onCalcGravity(this);
}

void KoteBug::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::validateClipping(this);
        MR::startAction(this, "Wait");
    }

    f32 distance = MR::calcDistanceToPlayer(this);
    if (MR::isOnGround(this) && distance < ::hSearchDistance) {
        bool escape = !MR::isPlayerElementModeBee();
        if (escape) {
            setNerve(GET_NERVE(KoteBug, HostTypeNrvEscapeSearch));
        } else {
            setNerve(GET_NERVE(KoteBug, HostTypeNrvSearch));
        }
    } else if (MR::isOnGround(this)) {
        setNerve(GET_NERVE(KoteBug, HostTypeNrvNoCalcWait));
    } else {
        MR::applyVelocityDampAndGravity(this, ::hCommonGravity, ::hGroundDamp, ::hAirDampH, ::hAirDampV, 1.0f);
    }
}

void KoteBug::endWait() {
    MR::invalidateClipping(this);
}

void KoteBug::exeSearch() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Syaka");
    }

    MR::turnDirectionToPlayerDegreeHorizon(this, &mFront, ::hSearchTurnLimitDegree);
    if (MR::isGreaterStep(this, ::hSeachTime)) {
        setNerve(GET_NERVE(KoteBug, HostTypeNrvFlyPursue));
    } else if (::hSearchDistance < MR::calcDistanceToPlayer(this)) {
        setNerve(GET_NERVE(KoteBug, HostTypeNrvWait));
    } else {
        MR::applyVelocityDampAndGravity(this, ::hCommonGravity, ::hGroundDamp, ::hAirDampH, ::hAirDampV, 1.0f);
    }
}

void KoteBug::exeEscapeSearch() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Syaka");
    }

    MR::turnDirectionToPlayerDegreeHorizon(this, &mFront, ::hPreEscapeTurnLimitDegree);
    if (MR::isGreaterStep(this, ::hEscapeSeachTime)) {
        bool escape = !MR::isPlayerElementModeBee();
        if (escape) {
            setNerve(GET_NERVE(KoteBug, HostTypeNrvPreEscape));
        } else {
            setNerve(GET_NERVE(KoteBug, HostTypeNrvFlyPursue));
        }
    } else if (::hSearchDistance < MR::calcDistanceToPlayer(this)) {
        setNerve(GET_NERVE(KoteBug, HostTypeNrvWait));
    } else {
        MR::applyVelocityDampAndGravity(this, ::hCommonGravity, ::hGroundDamp, ::hAirDampH, ::hAirDampV, 1.0f);
    }
}

void KoteBug::exePreEscape() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "PreEscape");
        MR::startSound(this, "SE_EM_KOTEBUG_FIND");
    }

    MR::applyVelocityDampAndGravity(this, ::hCommonGravity, ::hGroundDamp, ::hAirDampH, ::hAirDampV, 1.0f);
    MR::turnDirectionToPlayerDegreeHorizon(this, &mFront, ::hPreEscapeTurnLimitDegree);
    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(KoteBug, HostTypeNrvFlyEscape));
    }
}

void KoteBug::exeFlyStart() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Wait");
    }

    MR::startLevelSound(this, "SE_EM_LV_KOTEBUG_FLY");
    MR::applyVelocityDampAndGravity(this, ::hCommonGravity, ::hGroundDamp, ::hAirDampH, ::hAirDampV, 1.0f);
    MR::turnDirectionToPlayerDegreeHorizon(this, &mFront, ::hFlyTurnLimitDegree);
    if (MR::isGreaterStep(this, ::hFlyStartTime)) {
        setNerve(GET_NERVE(KoteBug, HostTypeNrvFlyPursue));
    }
}

void KoteBug::exeFlyPursue() {
    if (MR::isFirstStep(this)) {
        if (isNerve(GET_NERVE(KoteBug, HostTypeNrvFlyEscape))) {
            MR::startAction(this, "Jitabata");
        } else {
            MR::startAction(this, "Fly");
        }
    }

    MR::startLevelSound(this, "SE_EM_LV_KOTEBUG_FLY");
    MR::applyVelocityDampAndGravity(this, ::hCommonGravity, ::hGroundDamp, ::hAirDampH, ::hAirDampV, 1.0f);
    if (isNerve(GET_NERVE(KoteBug, HostTypeNrvFlyEscape))) {
        MR::turnDirectionFromPlayerDegreeHorizon(this, &mFront, ::hFlyEscapeTurnLimitDegree);
    } else {
        MR::turnDirectionToPlayerDegreeHorizon(this, &mFront, ::hFlyTurnLimitDegree);
    }

    mVelocity.add(mFront * ::hFlyPursueVel);
    if (MR::isShadowProjected(this, nullptr)) {
        MR::addVelocityKeepHeightUseShadow(this, ::hFlyHeight, 0.0f, ::hKeepAcc, ::hFreqRange, nullptr);
    }

    bool escape = !MR::isPlayerElementModeBee();
    if (escape) {
        if (isNerve(GET_NERVE(KoteBug, HostTypeNrvFlyPursue))) {
            setNerve(GET_NERVE(KoteBug, HostTypeNrvFlyEscape));
            return;
        }
    } else if (isNerve(GET_NERVE(KoteBug, HostTypeNrvFlyEscape))) {
        setNerve(GET_NERVE(KoteBug, HostTypeNrvFlyPursue));
        return;
    }

    if (::hFlyDistance < MR::calcDistanceToPlayer(this)) {
        setNerve(GET_NERVE(KoteBug, HostTypeNrvWait));
    } else if (isNerve(GET_NERVE(KoteBug, HostTypeNrvFlyEscape)) && MR::isGreaterStep(this, ::hFlyPursueTime)) {
        setNerve(GET_NERVE(KoteBug, HostTypeNrvFlyPursueLast));
    }
}

void KoteBug::exeFlyPursueLast() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Jitabata");
    }

    MR::startLevelSound(this, "SE_EM_LV_KOTEBUG_PURSUE_LAST");
    MR::applyVelocityDampAndGravity(this, ::hCommonGravity, ::hGroundDamp, ::hAirDampH, ::hAirDampV, 1.0f);
    f32 rate = getNerveStep() / f32(::hFlyPursueLastTime);
    MR::setBckRate(this, ::hFlyPursueLastAddRate + rate);
    mVelocity.add(mFront * (::hFlyPursueVel - ::hFlyPursueLastSubVel * rate));
    if (MR::isShadowProjected(this, nullptr)) {
        MR::addVelocityKeepHeightUseShadow(this, ::hFlyJitabataHeightBase + ::hFlyJitabataHeight * rate, 0.0f, ::hKeepAcc, ::hFreqRange, nullptr);
    }

    if (::hFlyDistance < MR::calcDistanceToPlayer(this)) {
        setNerve(GET_NERVE(KoteBug, HostTypeNrvWait));
    } else if (MR::isGreaterStep(this, ::hFlyPursueLastTime)) {
        setNerve(GET_NERVE(KoteBug, HostTypeNrvOverturnFall));
    }
}

void KoteBug::exeOverturnFall() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Pote");
        MR::startSound(this, "SE_EM_KOTEBUG_OVERTURN_FALL");
        MR::addVelocityJump(this, ::hOverturnFallJumpVelV);
    }

    MR::applyVelocityDampAndGravity(this, ::hCommonGravity, ::hGroundDamp, ::hAirDampH, ::hAirDampV, 1.0f);
    if (MR::isOnGroundCos(this, 0.75f)) {
        mScaleController->startAndAddScaleVelocityY(::hHitScaleYSpeed);
        mScaleController->startAnim();
        setNerve(GET_NERVE(KoteBug, HostTypeNrvOverturn));
        MR::emitEffect(this, "PoteSmoke");
        MR::startSound(this, "SE_EM_KOTEBUG_FALL_LAND");
    }
}

void KoteBug::exeOverturn() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Jitabata2");
    }

    MR::startLevelSound(this, "SE_EM_LV_KOTEBUG_OVERTURN");
    MR::applyVelocityDampAndGravity(this, ::hCommonGravity, ::hGroundDamp, ::hAirDampH, ::hAirDampV, 1.0f);
    MR::killVelocityOnGroundH(this);
    mBinder->_1EC._5 = true;
    if (MR::isGreaterStep(this, ::hOverturnTime)) {
        setNerve(GET_NERVE(KoteBug, HostTypeNrvPreRecover));
    }
}

void KoteBug::exePreRecover() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Gu");
    }

    MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);
    MR::applyVelocityDampAndGravity(this, ::hCommonGravity, ::hGroundDamp, ::hAirDampH, ::hAirDampV, 1.0f);
    MR::killVelocityOnGroundH(this);
    if (MR::isBckStopped(this)) {
        setNerve(GET_NERVE(KoteBug, HostTypeNrvRecover));
    }
}

void KoteBug::exeRecover() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Pon");
        MR::startSound(this, "SE_EM_KOTEBUG_RECOVER");
        MR::addVelocityJump(this, ::hRecoverJumpVelV);
    }

    MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);
    MR::applyVelocityDampAndGravity(this, ::hCommonGravity, ::hGroundDamp, ::hAirDampH, ::hAirDampV, 1.0f);
    if (MR::isOnGround(this)) {
        MR::startSound(this, "SE_EM_KOTEBUG_LAND");
        mScaleController->startAndAddScaleVelocityY(::hHitScaleYSpeed);
        mScaleController->startAnim();
        setNerve(GET_NERVE(KoteBug, HostTypeNrvWait));
    }
}

void KoteBug::exeHitToPlayer() {
    if (MR::isFirstStep(this)) {
        mScaleController->startAndAddScaleVelocityY(::hHitScaleYSpeed);
        mScaleController->startAnim();
        MR::startAction(this, "Wait");
        MR::addVelocityJump(this, ::hHitToPlayerJumpVelV);
        mVelocity.add(mFront * ::hHitToPlayerJumpVelH);
    }

    MR::reboundVelocityFromCollision(this, 0.0f, 0.0f, 1.0f);
    MR::applyVelocityDampAndGravity(this, ::hCommonGravity, ::hGroundDamp, ::hAirDampH, ::hAirDampV, 1.0f);
    if (MR::isOnGround(this)) {
        setNerve(GET_NERVE(KoteBug, HostTypeNrvFlyStart));
    }
}

void KoteBug::exeBlow() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Fly");
        MR::startBlowHitSound(this);
        MR::stopScene(::hBlowStopSceneTime);
        MR::onCalcShadow(this, nullptr);
        MR::onCalcGravity(this);
    }

    TQuat4f rotation;
    rotation.setRotate(TVec3f(1, 0, 0), ::hBlowRotateAngle);
    mPosture.mult(mPosture, rotation);

    if (mSpinHitController->execute(this)) {
        MR::startSound(this, "SE_EM_EXPLODE_S");
    }
}

void KoteBug::exeStampDeath() {
    if (MR::isFirstStep(this)) {
        MR::stopBck(this);
        MR::invalidateClipping(this);
        mVelocity.zero();
        mStampController->startAnim(mGravity);
        MR::setBinderRadius(this, 10.0f);
        MR::forceBindOnGround(this, 0.0f, 1.0f);
    }

    mBinder->_1EC._5 = true;
    if (MR::isGreaterStep(this, ::hStampDeathTime)) {
        MR::startSound(this, "SE_EM_EXPLODE_S");
        MR::appearCoinPop(this, mPosition, 1);
        kill();
    }
}

void KoteBug::exeBindStarPointer() {
    MR::updateActorStateAndNextNerve(this, mStarPointer, GET_NERVE(KoteBug, HostTypeNrvWait));
}

void KoteBug::endBindStarPointer() {
    mStarPointer->kill();
}

bool KoteBug::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(GET_NERVE(KoteBug, HostTypeNrvBlow)) || isNerve(GET_NERVE(KoteBug, HostTypeNrvStampDeath))) {
        return false;
    }

    if (MR::isMsgPlayerHipDrop(msg)) {
        MR::startSound(this, "SE_EM_STOMPED_S");
        setNerve(GET_NERVE(KoteBug, HostTypeNrvStampDeath));
        return true;
    }

    if (MR::isMsgPlayerTrample(msg)) {
        if (isNerve(GET_NERVE(KoteBug, HostTypeNrvOverturnFall))) {
            return false;
        }

        if (isNerve(GET_NERVE(KoteBug, HostTypeNrvOverturn)) || isNerve(GET_NERVE(KoteBug, HostTypeNrvRecover))) {
            MR::startSound(this, "SE_EM_STOMPED_S");
            setNerve(GET_NERVE(KoteBug, HostTypeNrvStampDeath));
            return true;
        }

        MR::startSound(this, "SE_EM_KOTEBUG_TRAMPLE");
        mVelocity.add(mFront * -10.0f);
        setNerve(GET_NERVE(KoteBug, HostTypeNrvOverturnFall));
        return true;
    }

    if (MR::isMsgLockOnStarPieceShoot(msg)) {
        if (isNerve(GET_NERVE(KoteBug, HostTypeNrvOverturnFall)) || isNerve(GET_NERVE(KoteBug, HostTypeNrvOverturn))) {
            return false;
        }

        return true;
    }

    if (MR::isMsgStarPieceAttack(msg)) {
        if (isNerve(GET_NERVE(KoteBug, HostTypeNrvOverturnFall)) || isNerve(GET_NERVE(KoteBug, HostTypeNrvOverturn))) {
            MR::addVelocityJump(this, ::hOverturnFallJumpVelV);
            return true;
        }

        setOverturnFall(pSender, pReceiver);
        return true;
    }

    if (MR::isMsgInvincibleAttack(msg) || MR::isMsgFireBallAttack(msg)) {
        mSpinHitController->start(this, pSender->mPosition, pReceiver->mPosition);
        setNerve(GET_NERVE(KoteBug, HostTypeNrvBlow));
        return true;
    }

    if (MR::isMsgPlayerHitAll(msg)) {
        if (isNerve(GET_NERVE(KoteBug, HostTypeNrvOverturnFall)) || isNerve(GET_NERVE(KoteBug, HostTypeNrvOverturn))) {
            mSpinHitController->start(this, pSender->mPosition, pReceiver->mPosition);
            setNerve(GET_NERVE(KoteBug, HostTypeNrvBlow));
            return true;
        }

        setOverturnFall(pSender, pReceiver);
        return true;
    }

    return false;
}

bool KoteBug::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgInhaleBlackHole(msg)) {
        kill();
        return true;
    }

    if (MR::isMsgPlayerKick(msg)) {
        if (MR::isPlayerExistUp(this, getSensor("body")->mRadius, 0.25f)) {
            return false;
        }

        if (isNerve(GET_NERVE(KoteBug, HostTypeNrvOverturn)) || isNerve(GET_NERVE(KoteBug, HostTypeNrvPreRecover)) ||
            isNerve(GET_NERVE(KoteBug, HostTypeNrvRecover))) {
            mSpinHitController->start(this, pSender->mPosition, pReceiver->mPosition);
            setNerve(GET_NERVE(KoteBug, HostTypeNrvBlow));
            return true;
        }
    }

    return false;
}

void KoteBug::setOverturnFall(HitSensor* pSender, HitSensor* pReceiver) {
    MR::startSound(this, "SE_EM_KOTEBUG_TRAMPLE");

    TVec3f direction(pReceiver->mPosition);
    direction -= pSender->mPosition;
    MR::vecKillElement(direction, mGravity, &direction);
    MR::normalizeOrZero(&direction);
    mVelocity.add(direction * 10.0f);

    setNerve(GET_NERVE(KoteBug, HostTypeNrvOverturnFall));
}

bool KoteBug::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(GET_NERVE(KoteBug, HostTypeNrvBlow)) || isNerve(GET_NERVE(KoteBug, HostTypeNrvStampDeath))) {
        return false;
    }

    if (MR::isMsgExplosionAttack(msg)) {
        mSpinHitController->start(this, pSender->mPosition, pReceiver->mPosition);
        setNerve(GET_NERVE(KoteBug, HostTypeNrvBlow));
        return true;
    }

    if (MR::isMsgToEnemyAttackBlow(msg)) {
        mSpinHitController->start(this, pSender->mPosition, pReceiver->mPosition);
        setNerve(GET_NERVE(KoteBug, HostTypeNrvBlow));
        return true;
    }

    if (MR::isMsgToEnemyAttackTrample(msg)) {
        setNerve(GET_NERVE(KoteBug, HostTypeNrvStampDeath));
        return true;
    }

    return false;
}

void KoteBug::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(GET_NERVE(KoteBug, HostTypeNrvBlow)) || isNerve(GET_NERVE(KoteBug, HostTypeNrvStampDeath))) {
        return;
    }

    if (pSender != getSensor("body")) {
        return;
    }

    if (MR::isSensorPlayer(pReceiver)) {
        if (MR::isPlayerHipDropFalling() || MR::isPlayerHipDropLand()) {
            return;
        }

        if (isNerve(GET_NERVE(KoteBug, HostTypeNrvFlyStart)) || isNerve(GET_NERVE(KoteBug, HostTypeNrvFlyPursue))) {
            if (MR::isPlayerExistUp(this, getSensor("body")->mRadius, 0.25f)) {
                MR::sendMsgPush(pReceiver, pSender);
                return;
            }

            if (MR::sendMsgEnemyAttack(pReceiver, pSender) == true) {
                setNerve(GET_NERVE(KoteBug, HostTypeNrvHitToPlayer));
                return;
            }
        }

        MR::sendMsgPushAndKillVelocityToTarget(this, pReceiver, pSender);
    } else if (MR::isSensorEnemy(pReceiver)) {
        MR::sendMsgPush(pReceiver, pSender);
    }
}

bool KoteBug::receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(GET_NERVE(KoteBug, HostTypeNrvBlow)) || isNerve(GET_NERVE(KoteBug, HostTypeNrvStampDeath)) ||
        isNerve(GET_NERVE(KoteBug, HostTypeNrvBindStarPointer))) {
        return false;
    }

    if (MR::isSensorEnemy(pSender) || MR::isSensorMapObj(pSender)) {
        MR::addVelocityFromPushHorizon(this, ::hPushedAcc, pSender, pReceiver);
        return true;
    }

    return false;
}

void KoteBug::calcAndSetBaseMtx() {
    TPos3f mtx;
    MR::calcMtxFromGravityAndZAxis(&mtx, this, mGravity, mFront);
    TPos3f stampMtx(mStampController->_14);
    mtx.concat(stampMtx, mtx);
    mtx.setTrans(mPosition);
    MR::setBaseTRMtx(this, mtx);

    TVec3f scale = mScaleController->_C * mScale;
    MR::setBaseScale(this, scale);
    mJointController->registerCallBack();
}

bool KoteBug::calcJoint(TPos3f* pMtx, const JointControllerInfo& rInfo) {
    TPos3f rotation;
    rotation.makeQuat(mPosture);
    pMtx->concat(*pMtx, rotation);
    return true;
}
