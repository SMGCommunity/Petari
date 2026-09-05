#include "Game/Boss/BossBegoman.hpp"
#include "Game/Boss/BossBegomanHead.hpp"
#include "Game/Enemy/BegomanBaby.hpp"
#include "Game/Enemy/BegomanSpike.hpp"
#include "Game/LiveActor/ActorCameraInfo.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/ElectricRailHolder.hpp"
#include "Game/Util/ActorCameraUtil.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/ParabolicPath.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace {
    const MR::ActorMoveParam hWaitParam = {0.0f, 3.0f, 0.95f, 1.0f};
    const MR::ActorMoveParam hSignAttackParam = {0.0f, 3.0f, 0.8f, 3.0f};
    const MR::ActorMoveParam hPursueParam = {1.3f, 3.0f, 0.97f, 3.0f};
    const MR::ActorMoveParam hTurnParam = {0.4f, 3.0f, 0.95f, 0.0f};
    const MR::ActorMoveParam hHitReactionParam = {0.0f, 3.0f, 0.95f, 0.0f};
    const MR::ActorMoveParam hOnWeakParam = {-0.5f, 3.0f, 0.95f, 3.0f};
    const MR::ActorMoveParam hOnWeakNoMoveParam = {0.0f, 3.0f, 0.95f, 3.0f};
    static const f32 sLaunchRadius = 100.0f;
    static const f32 sBabyLaunchVelH = 10.0f;
    static const f32 sBabyLaunchVelV = 15.0f;
    static const f32 hSpikeLaunchVelH = 10.0f;
    static const f32 hSpikeLaunchVelV = 15.0f;
    static const f32 hSideCheckOffsetY = 80.0f;
    static const f32 hSideCheckDot = 0.25f;
    // static const _32 sCommonDampVel = _;
    // static const _32 sCommonAirDamp = _;
    // static const _32 sCommonGravity = _;
    // static const _32 sReboundEnemyAddVel = _;
    // static const _32 sReboundPlayerAddVel = _;
    // static const _32 sPreventSwingbyDamp = _;
    static const s32 sDefaultLife = 3;
    // static const _32 sCommonAddRotate = _;
    // static const _32 sAddAttackRate = _;
    // static const _32 sInitAttackRate = _;
    // static const _32 hShadowSize = _;
    // static const _32 hShadowScale = _;
    // static const _32 hStarPieceEmitNum1st = _;
    // static const _32 hStarPieceEmitNum2nd = _;
    // static const _32 hEdgeRecoverTime = _;
    static const f32 sWaitRotate = 0.15f;
    static const f32 sSignAttackRotate = 0.5f;
    // static const _32 sRatio = _;
    static const f32 sPursueRotate = 0.5f;
    static const f32 sTurnRotate = 0.4f;
    // static const _32 hFrontSparkLength = _;
    static const s32 sOnWeakTime = 420;
    static const s32 hOnWeakSignSubTime = 240;
    static const s32 hOnWeakSignFastSubTime = 360;
    static const f32 hSignSlowRate = 0.5f;
    static const f32 hSignFastRate = 2.0f;
    static const s32 hOnWeakTurnTime = 30;
    // static const _32 sOnWeakUpdateFrontDegree = _;
    // static const _32 hOnWeakEscapeRange = _;
    // static const _32 hEscapeRate = _;
    // static const _32 hEscapeSideRate = _;
    // static const _32 hOnWeakVelocity = _;
    static const f32 hOnWeakSideVel = 0.35f;
    static const f32 sBrakeRotate = 0.2f;
    static const f32 sProvokeRotate = 0.3f;
    // static const _32 sShakeTime = _;
    // static const _32 sShakeDampVel = _;
    // static const _32 sShakeRotate = _;
    static const s32 sTrampleReactionOnSwitchTime = 80;
    // static const _32 sTrampleReactionDampVel = _;
    static const f32 sTrampleReactionRotate = 0.25f;
    // static const _32 sTrampBlowVel = _;
    static const s32 sAwareTime = 60;
    static const f32 sAwareRotate = 0.1f;
    static const f32 sHitReactionRotate = 0.25f;
    static const s32 sStopSceneTime = 2;
    static const s32 sBlowFrame = 20;
    static const f32 hBlowVel = 40.0f;
    static const f32 hBlowVerticalVel = 40.0f;
    static const s32 sDeathStopSceneTime = 5;
    static const s32 sElectricDeathTime = 60;
    static const s32 sElectricReturnTime = 60;
    // static const _32 sElectricReturnDampVelGround = _;
    // static const _32 sElectricReturnVel = _;
    static const s32 sJumpToInitPosTime = 60;
    // static const _32 sJumpToInitPosMaxHeight = _;
    static const f32 sJumpToInitPosRotate = 0.1f;
    static const f32 hKeepDistFar = 800.0f;
    static const f32 hKeepDistNear = 600.0f;
    // static const _32 sRotNeedleSeMaxVolumeSpeed = _;
};  // namespace

namespace NrvBossBegoman {
    NEW_NERVE_ONEND(HostTypeNrvPreDemoWait, BossBegoman, PreDemoWait, PreDemoWait);
    NEW_NERVE(HostTypeNrvFirstContactDemo, BossBegoman, FirstContactDemo);
    NEW_NERVE(HostTypeNrvReady, BossBegoman, Ready);
    NEW_NERVE_ONEND(HostTypeNrvNoCalcWait, BossBegoman, NoCalcWait, NoCalcWait);
    NEW_NERVE(HostTypeNrvWait, BossBegoman, Wait);
    NEW_NERVE(HostTypeNrvSignAttack, BossBegoman, SignAttack);
    NEW_NERVE(HostTypeNrvPursue, BossBegoman, Pursue);
    NEW_NERVE_ONEND(HostTypeNrvTurn, BossBegoman, Turn, Turn);
    NEW_NERVE_ONEND(HostTypeNrvOnWeak, BossBegoman, OnWeak, OnWeak);
    NEW_NERVE(HostTypeNrvOnWeakTurn, BossBegoman, OnWeak);
    NEW_NERVE(HostTypeNrvBrake, BossBegoman, Brake);
    NEW_NERVE(HostTypeNrvStepBack, BossBegoman, StepBack);
    NEW_NERVE(HostTypeNrvStepBackOnWeak, BossBegoman, StepBack);
    NEW_NERVE(HostTypeNrvReturn, BossBegoman, Return);
    NEW_NERVE(HostTypeNrvProvoke, BossBegoman, Provoke);
    NEW_NERVE(HostTypeNrvTrampleReaction, BossBegoman, TrampleReaction);
    NEW_NERVE(HostTypeNrvAware, BossBegoman, Aware);
    NEW_NERVE(HostTypeNrvHitReaction, BossBegoman, HitReaction);
    NEW_NERVE(HostTypeNrvBlow, BossBegoman, Blow);
    NEW_NERVE(HostTypeNrvElectricDeath, BossBegoman, ElectricDeath);
    NEW_NERVE(HostTypeNrvElectricReturn, BossBegoman, ElectricReturn);
    NEW_NERVE(HostTypeNrvJumpToInitPos, BossBegoman, JumpToInitPos);
    NEW_NERVE(HostTypeNrvKeepDistance, BossBegoman, KeepDistance);
};  // namespace NrvBossBegoman

BossBegoman::BossBegoman(const char* pName)
    : BegomanBase(pName), mBabyFollowers(), mSpikeFollowers(), mBabyFollowerNum(), mSpikeFollowerNum(), mFollowerKind(FollowerKind_BothFollower),
      mPath(), mHead(), mHealth(::sDefaultLife), _150(0.2f), mOpeningDemoInfo() {
    mHeadMtx.identity();
}

void BossBegoman::init(const JMapInfoIter& rIter) {
    initCore(rIter, "BossBegoman", true);

    MR::calcGravity(this);

    mInitPos.sub(mGravity * 10.0f);
    mHead = new BossBegomanHead(this, mHeadMtx);
    mHead->initWithoutIter();

    MR::startBrk(mHead, "OffWait");
    initBinder(160.0f, 160.0f, false);
    initNerve(&NrvBossBegoman::HostTypeNrvPreDemoWait::sInstance);

    f32 scaleY = mScale.y;
    initSensor(1, 190.0f, 250.0f, "Center");
    MR::addHitSensorAtJoint(this, "trample", "Center", ATYPE_BEGOMAN, 8, 180.0f * scaleY, TVec3f(0.0f, 0.0f, 0.0f));

    initEffect(0);
    MR::setEffectHostSRT(this, "EdgeSpark", nullptr, nullptr, nullptr);
    initSound(6, false);

    initShadow(200.0f, "Center");
    MR::setShadowDropLength(this, nullptr, 4000.0f);

    MR::declarePowerStar(this);
    MR::declareStarPiece(this, 24);

    mPath = new ParabolicPath();

    s32 arg7 = -1;
    MR::getJMapInfoArg7NoInit(rIter, &arg7);

    if (arg7 == -1) {
        mFollowerKind = FollowerKind_BabyFollower;
    } else {
        mFollowerKind = FollowerKind_SpikeFollower;
    }

    mBabyFollowerNum = 3;
    mBabyFollowers = new BegomanBaby*[mBabyFollowerNum];

    for (int i = 0; i < mBabyFollowerNum; i++) {
        BegomanBaby* newBaby = new BegomanBaby("子分ベビー");
        mBabyFollowers[i] = newBaby;
        newBaby->mHost = this;
        newBaby->mPosition.set(this->mPosition);
        newBaby->initWithoutIter();
        newBaby->makeActorDead();
    }

    mSpikeFollowerNum = 2;
    mSpikeFollowers = new BegomanSpike*[mSpikeFollowerNum];

    for (int i = 0; i < mSpikeFollowerNum; i++) {
        BegomanSpike* spike = new BegomanSpike("子分トゲ");
        mSpikeFollowers[i] = spike;
        spike->mPosition.set(this->mPosition);
        spike->initWithoutIter();
        spike->makeActorDead();
    }

    if (MR::tryRegisterDemoCast(this, rIter)) {
        s32 i;

        MR::registerDemoActionNerve(this, &NrvBossBegoman::HostTypeNrvFirstContactDemo::sInstance, nullptr);

        for (i = 0; i < mBabyFollowerNum; i++) {
            MR::tryRegisterDemoCast(mBabyFollowers[i], rIter);
        }

        for (i = 0; i < mSpikeFollowerNum; i++) {
            MR::tryRegisterDemoCast(mSpikeFollowers[i], rIter);
        }

        MR::tryRegisterDemoCast(mHead, rIter);
    }

    mOpeningDemoInfo = new ActorCameraInfo(rIter);
    MR::initAnimCamera(this, mOpeningDemoInfo, "OpeningDemo");

    makeActorAppeared();
}

void BossBegoman::appear() {
    BegomanBase::appear();
    MR::emitEffect(this, "Death");
    setNerve(&NrvBossBegoman::HostTypeNrvWait::sInstance);
}

void BossBegoman::kill() {
    BegomanBase::kill();
    killAllFollower(FollowerKind_BothFollower);
    MR::startAfterBossBGM();
    MR::requestAppearPowerStar(this, mPosition);
}

void BossBegoman::control() {
    TVec3f headEdgeScale;
    MR::copyJointScale(mHead, "Edge", &headEdgeScale);
    MR::setShadowVolumeSphereRadius(this, nullptr, 200.0f * (1.0f - 0.35f * (1 - headEdgeScale.x)));

    if (isNerve(&NrvBossBegoman::HostTypeNrvOnWeak::sInstance) || isNerve(&NrvBossBegoman::HostTypeNrvOnWeakTurn::sInstance)) {
        HitSensor* bodySensor = getSensor("body");
        bodySensor->mRadius = 160.0f;
    } else {
        HitSensor* bodySensor = getSensor("body");
        bodySensor->mRadius = 190.0f;
    }

    if (!isNerve(&NrvBossBegoman::HostTypeNrvBlow::sInstance) && !isNerve(&NrvBossBegoman::HostTypeNrvElectricDeath::sInstance) &&
        !isNerve(&NrvBossBegoman::HostTypeNrvElectricReturn::sInstance) && !isNerve(&NrvBossBegoman::HostTypeNrvJumpToInitPos::sInstance) &&
        !isNerve(&NrvBossBegoman::HostTypeNrvOnWeakTurn::sInstance)) {
        checkTouchElectricRail(true);
    } else {
        checkTouchElectricRail(false);
    }

    if (isNerve(&NrvBossBegoman::HostTypeNrvPursue::sInstance) || isNerve(&NrvBossBegoman::HostTypeNrvTurn::sInstance)) {
        f32 max = 1.0f;
        f32 new150;
        if (_150 + 0.01f >= max) {
            new150 = max;
        } else {
            new150 = _150 + 0.01f;
        }
        _150 = new150;
    } else {
        _150 = 0.2f;
    }

    if (MR::isStep(this, 1)) {
        if (isNerve(&NrvBossBegoman::HostTypeNrvWait::sInstance) || isNerve(&NrvBossBegoman::HostTypeNrvNoCalcWait::sInstance)) {
            MR::validateClipping(this);
        } else {
            MR::invalidateClipping(this);
        }
    }

    BegomanBase::control();

    if (!isNerve(&NrvBossBegoman::HostTypeNrvPreDemoWait::sInstance) && !isNerve(&NrvBossBegoman::HostTypeNrvFirstContactDemo::sInstance)) {
        startRotationLevelSound();
    }
}

void BossBegoman::setStepBackNerve() {
    if (mHead->isEdgeOut()) {
        setNerve(&NrvBossBegoman::HostTypeNrvStepBack::sInstance);
    } else {
        setNerve(&NrvBossBegoman::HostTypeNrvStepBackOnWeak::sInstance);
    }
}

bool BossBegoman::onTouchElectric(const TVec3f& a1, const TVec3f& a2) {
    if (isNerve(&NrvBossBegoman::HostTypeNrvElectricDeath::sInstance) || isNerve(&NrvBossBegoman::HostTypeNrvElectricReturn::sInstance) ||
        isNerve(&NrvBossBegoman::HostTypeNrvJumpToInitPos::sInstance)) {
        return false;
    }

    if (isNerve(&NrvBossBegoman::HostTypeNrvBlow::sInstance) || isNerve(&NrvBossBegoman::HostTypeNrvOnWeakTurn::sInstance)) {
        setNerve(&NrvBossBegoman::HostTypeNrvElectricDeath::sInstance);
        return false;
    }

    if (isNerve(&NrvBossBegoman::HostTypeNrvStepBack::sInstance) || isNerve(&NrvBossBegoman::HostTypeNrvStepBackOnWeak::sInstance)) {
        if (!MR::isOnGround(this)) {
            return false;
        }
    }

    if (BegomanBase::onTouchElectric(a1, a2)) {
        setStepBackNerve();
        return true;
    }

    return false;
}

bool BossBegoman::setNerveReturn() {
    setNerve(&NrvBossBegoman::HostTypeNrvReturn::sInstance);

    return true;
}

const Nerve* BossBegoman::getNerveWait() {
    return &NrvBossBegoman::HostTypeNrvWait::sInstance;
}

void BossBegoman::addVelocityOnPushedFromElectricRail(const TVec3f& a1, const TVec3f& a2) {
    TVec3f railPushVelH(mInitPos);
    railPushVelH.sub(getSensor("body")->mPosition);
    MR::vecKillElement(railPushVelH, mGravity, &railPushVelH);
    railPushVelH.setLength(getRailPushVelHBoss());

    mVelocity.add(railPushVelH);
    mVelocity.add(mGravity * -getRailPushJumpBoss());
}

bool BossBegoman::requestAttack() {
    if (isDeadAllFollower()) {
        return BegomanBase::requestAttack();
    }

    return false;
}

void BossBegoman::exePreDemoWait() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "PreDemoWait");
    }

    MR::startLevelSound(this, "SE_BM_LV_BBEGO_PRE_DEMO_FLY");
    exeNoCalcWaitCore(0.005f, nullptr);
}

void BossBegoman::endPreDemoWait() {
    finishNoCalcWait();
}

void BossBegoman::exeFirstContactDemo() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "OpeningDemo");
        MR::startAnimCameraTargetSelf(this, mOpeningDemoInfo, "OpeningDemo", 0, 1.0f);

        MR::showModel(this);
        MR::showModel(mHead);

        mHead->setOpeningDemo();
        mVelocity.zero();

        MR::stopStageBGM(60);
        MR::overlayWithPreviousScreen(2);
    }

    if (MR::isActionEnd(this)) {
        setNerve(&NrvBossBegoman::HostTypeNrvReady::sInstance);
    }
}
void BossBegoman::exeReady() {
    if (MR::isFirstStep(this)) {
        MR::startBossBGM(MR::BossBgmID_Begoman);
        mVelocity.zero();
    }

    edgeRecoverCore();

    if (MR::isActionEnd(this) && MR::isAnimCameraEnd(this, mOpeningDemoInfo, "OpeningDemo")) {
        MR::endAnimCamera(this, mOpeningDemoInfo, "OpeningDemo", -1, true);
        tryLaunchFollower();
        setNerve(&NrvBossBegoman::HostTypeNrvWait::sInstance);
    }
}

void BossBegoman::exeNoCalcWait() {
    if (MR::isFirstStep(this)) {
        tryLaunchFollower();
    }

    exeNoCalcWaitCore(0.005f, &NrvBossBegoman::HostTypeNrvWait::sInstance);
}

void BossBegoman::endNoCalcWait() {
    finishNoCalcWait();
}

void BossBegoman::exeWait() {
    updateRotateY(::sWaitRotate);
    exeWaitCore(::hWaitParam, &NrvBossBegoman::HostTypeNrvSignAttack::sInstance, &NrvBossBegoman::HostTypeNrvKeepDistance::sInstance,
                &NrvBossBegoman::HostTypeNrvNoCalcWait::sInstance);
}

void BossBegoman::exeSignAttack() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Attack");
        MR::startSound(this, "SE_BM_BBEGO_PRE_PURSUE");
    }

    updateRotateY(::sSignAttackRotate);
    exeSignAttackCore(::hSignAttackParam, &NrvBossBegoman::HostTypeNrvPursue::sInstance);
}

void BossBegoman::exePursue() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_BM_BBEGO_PURSUE_START");
    }

    updateRotateY(::sPursueRotate);
    exePursueCore(::hPursueParam, &NrvBossBegoman::HostTypeNrvBrake::sInstance, &NrvBossBegoman::HostTypeNrvTurn::sInstance, *getSoundBoss(), _150);
}

void BossBegoman::exeTurn() {
    if (MR::isFirstStep(this)) {
        mHead->tryTurn();
    }

    if (MR::isBckPlaying(this, "Turn")) {
        MR::emitEffectHit(this, mPosition + mFaceVec * 180.0f, "EdgeSpark");
    }

    updateRotateY(::sTurnRotate);
    MR::startLevelSound(this, "SE_BM_LV_BBEGO_TURN");
    exeTurnCore(::hTurnParam, &NrvBossBegoman::HostTypeNrvBrake::sInstance, &NrvBossBegoman::HostTypeNrvPursue::sInstance, false);
}

void BossBegoman::endTurn() {
    mHead->tryTurnEnd();
}

void BossBegoman::exeOnWeak() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Shake");
        MR::startBrk(mHead, "OnWait");
    }

    updateRotateY(0.4f);

    if (MR::calcDistanceToPlayer(mPosition) < 600.0f) {
        MR::moveAndTurnToPlayer(this, &mFaceVec, ::hOnWeakParam._0, ::hOnWeakParam._4, ::hOnWeakParam._8, ::hOnWeakParam._C);
        addVelocityEscapeToSide(::hOnWeakSideVel);
    } else {
        MR::moveAndTurnToPlayer(this, &mFaceVec, ::hOnWeakNoMoveParam._0, ::hOnWeakNoMoveParam._4, ::hOnWeakNoMoveParam._8, ::hOnWeakNoMoveParam._C);
    }

    if (isNerve(&NrvBossBegoman::HostTypeNrvOnWeakTurn::sInstance)) {
        if (MR::isGreaterStep(this, ::hOnWeakTurnTime)) {
            setNerve(&NrvBossBegoman::HostTypeNrvOnWeak::sInstance);
        }
    } else {
        if (MR::isGreaterStep(this, ::sOnWeakTime)) {
            setNerve(&NrvBossBegoman::HostTypeNrvAware::sInstance);
        } else if (MR::isStep(this, ::hOnWeakSignSubTime)) {
            MR::startBrk(mHead, "SignWait");
            MR::setBrkRate(mHead, ::hSignSlowRate);
        } else if (MR::isStep(this, ::hOnWeakSignFastSubTime)) {
            MR::startBrk(mHead, "SignWait");
            MR::setBrkRate(mHead, ::hSignFastRate);
        }

        if (MR::isGreaterEqualStep(this, ::hOnWeakSignFastSubTime)) {
            MR::startLevelSound(this, "SE_BM_LV_BBEGO_ALARM_FAST");
        } else if (MR::isGreaterEqualStep(this, ::hOnWeakSignSubTime)) {
            MR::startLevelSound(this, "SE_BM_LV_BBEGO_ALARM");
        }
    }
}

void BossBegoman::endOnWeak() {
    MR::startBrk(mHead, "OffWait");
}

void BossBegoman::exeBrake() {
    updateRotateY(::sBrakeRotate);
    MR::startLevelSound(this, "SE_EM_LV_BEGOMAN_SPARK");
    exeBrakeCore(&NrvBossBegoman::HostTypeNrvTurn::sInstance);
}

void BossBegoman::exeStepBack() {
    // result of call is unused
    if (MR::isFirstStep(this)) {
    }

    updateRotateY(0.2f);

    if (isNerve(&NrvBossBegoman::HostTypeNrvStepBackOnWeak::sInstance)) {
        exeStepBackCore(::hWaitParam, &NrvBossBegoman::HostTypeNrvOnWeak::sInstance);
    } else {
        exeStepBackCore(::hWaitParam, &NrvBossBegoman::HostTypeNrvWait::sInstance);
    }
}

void BossBegoman::exeReturn() {
    updateRotateY(0.2f);
    exeReturnCore(&NrvBossBegoman::HostTypeNrvWait::sInstance);
}

void BossBegoman::exeProvoke() {
    if (MR::isFirstStep(this)) {
    }

    updateRotateY(::sProvokeRotate);
    exeProvokeCore(::hWaitParam, &NrvBossBegoman::HostTypeNrvSignAttack::sInstance);
}

void BossBegoman::exeTrampleReaction() {
    if (MR::isFirstStep(this)) {
        mHead->trySwitchPushTrample();

        if (!mHead->isSwitchOn()) {
            MR::startAction(this, "TrampleReaction");
            MR::startSound(this, "SE_BM_BBEGO_NEEDLE_ON");
        } else {
            MR::startAction(this, "HopEnd");
            MR::startSound(this, "SE_BM_BBEGO_STOMPED");
            MR::startSound(this, "SE_BM_BBEGO_NEEDLE_OFF");
        }
    }

    updateRotateY(::sTrampleReactionRotate);
    MR::moveAndTurnToPlayer(this, &mFaceVec, 0.0f, 3.0f, 0.95f, 3.0f);

    if (mHead->isSwitchOn()) {
        if (MR::isGreaterStep(this, ::sTrampleReactionOnSwitchTime)) {
            setNerve(&NrvBossBegoman::HostTypeNrvOnWeak::sInstance);
        }
    } else if (MR::isGreaterStep(this, ::sTrampleReactionOnSwitchTime)) {
        setNerve(&NrvBossBegoman::HostTypeNrvSignAttack::sInstance);
    }
}

void BossBegoman::exeAware() {
    if (MR::isFirstStep(this)) {
        mVelocity.zero();
    }

    updateRotateY(::sAwareRotate);
    MR::moveAndTurnToPlayer(this, &mFaceVec, 0.0f, 3.0f, 0.95f, 0.0f);
    edgeRecoverCore();

    if (MR::isGreaterStep(this, ::sAwareTime)) {
        tryLaunchFollower();
        setNerve(&NrvBossBegoman::HostTypeNrvSignAttack::sInstance);
    }
}

void BossBegoman::exeHitReaction() {
    updateRotateY(::sHitReactionRotate);
    exeHitReactionCore(::hHitReactionParam, &NrvBossBegoman::HostTypeNrvProvoke::sInstance);
}

void BossBegoman::exeBlow() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_EM_BEGOMAN_ROT_STOP");
        MR::startAction(this, "Damage");
        MR::stopScene(::sStopSceneTime);
    }

    MR::startLevelSound(this, "SE_EM_LV_BEGOMAN_SPARK");

    MR::moveAndTurnToDirection(this, &mFaceVec, mTargetVec, ::hHitReactionParam._0, ::hHitReactionParam._4, ::hHitReactionParam._8,
                               ::hHitReactionParam._C);
    reboundWallAndGround(&mFaceVec, false);

    if (MR::isGreaterStep(this, ::sBlowFrame) && MR::isOnGround(this)) {
        MR::startAction(this, "Turn");

        if (mHead->isSwitchOn()) {
            setNerve(&NrvBossBegoman::HostTypeNrvOnWeakTurn::sInstance);
        } else {
            setNerve(&NrvBossBegoman::HostTypeNrvTurn::sInstance);
        }
    }
}

void BossBegoman::exeElectricDeath() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "electricshock");
        MR::startBrk(mHead, "Damage");
        mVelocity.zero();

        getSensor("body")->invalidate();
        MR::startSound(this, "SE_EM_BEGOMAN_ELEC_DAMAGE");
        MR::invalidateClipping(this);
        mHealth--;
    }

    if (MR::isStep(this, 30)) {
        TVec3f vec(mInitPos);
        vec.sub(mPosition);
        MR::vecKillElement(vec, mGravity, &vec);
        MR::normalizeOrZero(&vec);
        vec.sub(mGravity);
        MR::normalizeOrZero(&vec);

        if (mHealth == 2) {
            MR::appearStarPieceToDirection(this, mPosition - mGravity * 200.0f, vec, 8, 20.0f, 40.0f, false);
            MR::startSound(this, "SE_OJ_STAR_PIECE_BURST");
        } else if (mHealth == 1) {
            MR::appearStarPieceToDirection(this, mPosition - mGravity * 200.0f, vec, 16, 20.0f, 40.0f, false);
            MR::startSound(this, "SE_OJ_STAR_PIECE_BURST");
        }
    }

    if (MR::isGreaterStep(this, ::sElectricDeathTime)) {
        MR::stopScene(::sDeathStopSceneTime);
        MR::shakeCameraWeak();

        if (mHealth == 0) {
            MR::startSound(this, "SE_BM_BBEGO_DEAD");
            kill();
            MR::emitEffect(this, "Death");
        } else {
            if (mHealth == 1) {
                killAllFollower(FollowerKind_SpikeFollower);
                mFollowerKind = FollowerKind_SpikeFollower;
            }

            setNerve(&NrvBossBegoman::HostTypeNrvJumpToInitPos::sInstance);
        }

        getSensor("body")->validate();
        MR::validateClipping(this);
    }
}

void BossBegoman::edgeRecoverCore() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Recover");
    }

    if (MR::isStep(this, 55)) {
        MR::startSound(this, "SE_BM_BBEGO_NEEDLE_ON");
        mHead->tryForceRecover();
        MR::tryRumblePadAndCameraDistanceMiddle(this, 800.0f, 1200.0f, 2000.0f);
    }
}

void BossBegoman::exeElectricReturn() {
    TVec3f vec1;
    TVec3f vec2;

    if (MR::isFirstStep(this)) {
        if (ElectricRailFunction::isTouchRail(getSensor("check"), &vec1, &vec2)) {
            vec1.sub(mPosition);

            TVec3f vec3 = vec1;
            vec3.cross(vec2, vec3);

            TVec3f vec4 = vec3;
            vec4.cross(vec2, vec4);

            if (0.0f < vec1.dot(vec4)) {
                vec4 = -vec4;
            }

            vec4.setLength(25.0f);
            mVelocity.set(vec4);
        }
    }

    reboundWallAndGround(&mFaceVec, false);
    MR::moveAndTurnToPlayer(this, &mFaceVec, 0.0f, 3.0f, 0.8f, 0.0f);

    if (MR::isGreaterStep(this, ::sElectricReturnTime)) {
        mFaceVec.set(mTargetVec);
        setNerve(&NrvBossBegoman::HostTypeNrvJumpToInitPos::sInstance);
    }
}

void BossBegoman::exeJumpToInitPos() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Shake");
        MR::startSound(this, "SE_BM_BBEGO_BIG_JUMP");
        // vec1 goes completely unused
        TVec3f vec1(mInitPos);
        vec1 -= mPosition;
        vec1 *= 0.5f;

        TVec3f vec2(mGravity);
        vec2 = -vec2;

        mPath->initFromUpVector(mPosition, mInitPos - mGravity * 10.0f, vec2, 700.0f);
        mVelocity.zero();
    }

    updateRotateY(::sJumpToInitPosRotate);
    mPath->calcPosition(&mPosition, MR::calcNerveRate(this, ::sJumpToInitPosTime));

    if (MR::isGreaterStep(this, ::sJumpToInitPosTime)) {
        setNerve(&NrvBossBegoman::HostTypeNrvSignAttack::sInstance);
        tryLaunchFollower();
        MR::startSound(this, "SE_BM_BBEGO_NEEDLE_ON");
        mHead->tryForceRecover();
        MR::tryRumblePadAndCameraDistanceMiddle(this, 800.0f, 1200.0f, 2000.0f);
    }
}

void BossBegoman::exeKeepDistance() {
    updateRotateY(0.2f);
    exeKeepDistanceCore(&NrvBossBegoman::HostTypeNrvWait::sInstance, &NrvBossBegoman::HostTypeNrvSignAttack::sInstance,
                        &NrvBossBegoman::HostTypeNrvBrake::sInstance, ::hKeepDistFar, ::hKeepDistNear);
}

void BossBegoman::tryLaunchFollower() {
    if (isDeadAllFollower()) {
        TVec3f vec(*MR::getPlayerCenterPos());
        vec.sub(mPosition);
        MR::vecKillElement(vec, mGravity, &vec);
        MR::normalizeOrZero(&vec);

        if (MR::isNearZero(vec)) {
            vec.set(mFaceVec);
        }

        if (mFollowerKind == FollowerKind_BabyFollower) {
            launchBegomanBabyFromGuarder(this, mBabyFollowers, mBabyFollowerNum, ::sLaunchRadius, ::sBabyLaunchVelH, ::sBabyLaunchVelV, &vec);
        } else if (mFollowerKind == FollowerKind_SpikeFollower) {
            launchBegoman(this, reinterpret_cast< BegomanBase** >(mSpikeFollowers), mSpikeFollowerNum, ::sLaunchRadius, ::hSpikeLaunchVelH,
                          ::hSpikeLaunchVelV, &vec);
        }
    }
}

void BossBegoman::killAllFollower(FollowerKind kind) {
    if (kind == FollowerKind_BabyFollower || kind == FollowerKind_BothFollower) {
        for (int i = 0; i < mBabyFollowerNum; i++) {
            if (!MR::isDead(mBabyFollowers[i])) {
                MR::emitEffect(mBabyFollowers[i], "Death");
                mBabyFollowers[i]->kill();
            }
        }
    }

    if (kind == FollowerKind_SpikeFollower || kind == FollowerKind_BothFollower) {
        for (int i = 0; i < mSpikeFollowerNum; i++) {
            if (!MR::isDead(mSpikeFollowers[i])) {
                MR::emitEffect(mSpikeFollowers[i], "Death");
                mSpikeFollowers[i]->kill();
            }
        }
    }
}

bool BossBegoman::isDeadAllFollower() {
    if (mFollowerKind == FollowerKind_BabyFollower) {
        return isDeadAllFollowerCore(reinterpret_cast< BegomanBase** >(mBabyFollowers), mBabyFollowerNum);
    } else {
        return isDeadAllFollowerCore(reinterpret_cast< BegomanBase** >(mSpikeFollowers), mSpikeFollowerNum);
    }
}

bool BossBegoman::isDeadAllFollowerCore(BegomanBase** begomanList, s32 numBegoman) {
    for (int i = 0; i < numBegoman; i++) {
        if (!MR::isDead(begomanList[i])) {
            return false;
        }
    }

    return true;
}

void BossBegoman::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (pSender != getSensor("body")) {
        return;
    }

    if (MR::isSensorEnemy(pReceiver)) {
        MR::sendMsgEnemyAttack(pReceiver, pSender);
        return;
    }

    if (!MR::isSensorPlayer(pReceiver)) {
        return;
    }

    if (isNerve(&NrvBossBegoman::HostTypeNrvBlow::sInstance) || isNerve(&NrvBossBegoman::HostTypeNrvElectricDeath::sInstance) ||
        isNerve(&NrvBossBegoman::HostTypeNrvElectricReturn::sInstance) || isNerve(&NrvBossBegoman::HostTypeNrvAware::sInstance)) {
        MR::sendMsgPush(pReceiver, pSender);
        return;
    }

    if (isNerve(&NrvBossBegoman::HostTypeNrvJumpToInitPos::sInstance)) {
        if (!MR::sendMsgEnemyAttackFlipRot(pReceiver, pSender)) {
            MR::sendMsgPush(pReceiver, pSender);
        }
        return;
    }

    if (MR::isPlayerSwingAction() || !MR::isOnGroundPlayer()) {
        return;
    }

    bool attackPlayerEdge;

    if (mHead->isEdgeOut() && MR::isPlayerExistSide(this, ::hSideCheckOffsetY, ::hSideCheckDot)) {
        attackPlayerEdge = MR::sendMsgEnemyAttackFire(pReceiver, pSender);
    } else {
        MR::sendMsgEnemyAttackFlipRot(pReceiver, pSender);
        return;
    }

    if (attackPlayerEdge) {
        TVec3f dirReceiverToSender(pSender->mPosition);
        dirReceiverToSender.sub(pReceiver->mPosition);
        MR::normalizeOrZero(&dirReceiverToSender);

        if (!MR::isNearZero(dirReceiverToSender)) {
            bool rebounded = reboundPlaneWithEffect(dirReceiverToSender, 0.0f, 0.0f, "Spark");
            mVelocity.add(dirReceiverToSender * 2.0f);

            if (rebounded) {
                MR::startSound(this, "SE_EM_BEGOMAN_COLLI");
            }
        }

        setNerve(&NrvBossBegoman::HostTypeNrvHitReaction::sInstance);
        return;
    }

    MR::sendMsgPushAndKillVelocityToTarget(this, pReceiver, pSender);
}

bool BossBegoman::receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver) {
    // result of call not used
    getSensor("body");
    return false;
}

bool BossBegoman::receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (pReceiver != getSensor("body")) {
        return false;
    }

    if (MR::isMsgEnemyAttackElectric(msg)) {
        TVec3f vec1(pSender->mHost->mVelocity);
        MR::vecKillElement(vec1, mGravity, &vec1);
        MR::normalizeOrZero(&vec1);

        return onTouchElectric(pSender->mPosition, vec1);
    }

    if (isNerve(&NrvBossBegoman::HostTypeNrvBlow::sInstance) || isNerve(&NrvBossBegoman::HostTypeNrvElectricDeath::sInstance) ||
        isNerve(&NrvBossBegoman::HostTypeNrvElectricReturn::sInstance) || isNerve(&NrvBossBegoman::HostTypeNrvJumpToInitPos::sInstance)) {
        return false;
    }

    if (MR::isSensorEnemy(pSender)) {
        TVec3f vec2(pReceiver->mPosition);
        vec2.sub(pSender->mPosition);
        MR::normalizeOrZero(&vec2);

        if (MR::isNearZero(vec2)) {
            return false;
        }

        bool rebounded = reboundPlaneWithEffect(vec2, 0.0f, 0.0f, "Spark");
        // float regswap
        MR::addVelocityLimit(this, vec2 * 6.0f * (pSender->mRadius / getSensor("body")->mRadius));

        if (rebounded) {
            MR::startSound(this, "SE_EM_BEGOMAN_COLLI_BEGOMAN");
        }

        return true;
    }

    return false;
}

bool BossBegoman::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgStarPieceReflect(msg)) {
        return true;
    }

    if (MR::isMsgPlayerTrample(msg) && pReceiver == getSensor("trample")) {
        return receiveMsgTrample(pSender, pReceiver);
    }

    if (isNerve(&NrvBossBegoman::HostTypeNrvBlow::sInstance) || isNerve(&NrvBossBegoman::HostTypeNrvElectricDeath::sInstance) ||
        isNerve(&NrvBossBegoman::HostTypeNrvElectricReturn::sInstance) || isNerve(&NrvBossBegoman::HostTypeNrvJumpToInitPos::sInstance)) {
        return false;
    }

    if (MR::isMsgPlayerHipDrop(msg) && pReceiver == getSensor("trample")) {
        MR::sendMsgAwayJump(pSender, pReceiver);
        setNerve(&NrvBossBegoman::HostTypeNrvTrampleReaction::sInstance);
        return true;
    }

    if (pReceiver != getSensor("body")) {
        return false;
    }

    if (MR::isMsgPlayerSpinAttack(msg) && !mHead->isSwitchOn() && MR::isPlayerExistSide(this, ::hSideCheckOffsetY, ::hSideCheckDot)) {
        MR::sendMsgEnemyAttackFire(pSender, pReceiver);
        setNerve(&NrvBossBegoman::HostTypeNrvProvoke::sInstance);
        return false;
    }

    if (isNerve(&NrvBossBegoman::HostTypeNrvTrampleReaction::sInstance) && !MR::isGreaterStep(this, 30)) {
        return false;
    }

    if (!mHead->isEdgeOut() && MR::isMsgPlayerHitAll(msg)) {
        calcBlowReaction(pSender->mPosition, pReceiver->mPosition, ::hBlowVel, ::hBlowVerticalVel);
        setNerve(&NrvBossBegoman::HostTypeNrvBlow::sInstance);
        return true;
    }

    return false;
}

bool BossBegoman::receiveMsgTrample(HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvBossBegoman::HostTypeNrvBlow::sInstance) || isNerve(&NrvBossBegoman::HostTypeNrvElectricDeath::sInstance) ||
        isNerve(&NrvBossBegoman::HostTypeNrvElectricReturn::sInstance) || isNerve(&NrvBossBegoman::HostTypeNrvJumpToInitPos::sInstance)) {
        return false;
    }

    if (MR::isPlayerDamaging() || MR::isPlayerJumpRising()) {
        return false;
    }

    if (isNerve(&NrvBossBegoman::HostTypeNrvTrampleReaction::sInstance) && MR::isLessStep(this, 5)) {
        return false;
    }

    if (pReceiver == getSensor("body")) {
        return false;
    }

    if (pReceiver == getSensor("trample")) {
        setNerve(&NrvBossBegoman::HostTypeNrvTrampleReaction::sInstance);

        TVec3f vec1(pReceiver->mPosition);
        vec1.sub(pSender->mPosition);

        MR::vecKillElement(vec1, mGravity, &vec1);
        MR::normalize(&vec1);
        mFaceVec.set(vec1);

        TVec3f vec2(*MR::getPlayerVelocity());
        MR::vecKillElement(vec2, mGravity, &vec2);

        if (!MR::isNearZero(vec2)) {
            TVec3f vec3 = vec2;
            vec3.cross(mGravity, vec3);
            vec3.setLength(25.0f);
            mVelocity = vec3;
        } else {
            TVec3f vec4;
            MR::getPlayerFrontVec(&vec4);
            vec4.cross(mGravity, vec4);
            vec4.setLength(25.0f);
            mVelocity = vec4;
        }
    }

    return true;
}

bool BossBegoman::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (isNerve(&NrvBossBegoman::HostTypeNrvBlow::sInstance) || isNerve(&NrvBossBegoman::HostTypeNrvElectricDeath::sInstance) ||
        isNerve(&NrvBossBegoman::HostTypeNrvElectricReturn::sInstance) || isNerve(&NrvBossBegoman::HostTypeNrvJumpToInitPos::sInstance)) {
        return false;
    }

    return MR::isMsgHitmarkEmit(msg);
}

void BossBegoman::calcAnim() {
    LiveActor::calcAnim();
    mHeadMtx.setInline(MR::getJointMtx(this, "Center"));

    TVec3f vec;
    mHeadMtx.getYDir(vec);

    if (!MR::isSameDirection(vec, mTargetVec)) {
        MR::makeMtxUpFront(&mHeadMtx, vec, mTargetVec);
    }
}
void BossBegoman::startRotationLevelSound() {
    if (isNerve(&NrvBossBegoman::HostTypeNrvWait::sInstance)) {
        MR::startLevelSound(this, "SE_BM_LV_BBEGO_ROT_SLOW");
    } else if (isNerve(&NrvBossBegoman::HostTypeNrvPursue::sInstance)) {
        MR::startLevelSound(this, "SE_BM_LV_BBEGO_PURSUE");
    } else if (isNerve(&NrvBossBegoman::HostTypeNrvTrampleReaction::sInstance) || isNerve(&NrvBossBegoman::HostTypeNrvOnWeak::sInstance)) {
        MR::startLevelSound(this, "SE_BM_LV_BBEGO_ROT_WEAK");
    } else {
        MR::startLevelSound(this, "SE_BM_LV_BBEGO_ROT_MIDDLE");
    }

    if (!mHead->isSwitchOn()) {
        f32 f1 = mVelocity.length();
        f1 *= f1;
        MR::startLevelSound(this, "SE_BM_LV_BBEGO_ROT_NEEDLE", MR::getLinerValueFromMinMax(f1, 0.0f, 121.0f, 70.0f, 100.0f));
    }
}
