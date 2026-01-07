#include "Game/Boss/BossBegoman.hpp"
#include "Game/Boss/BossBegomanHead.hpp"
#include "Game/Enemy/BegomanBaby.hpp"
#include "Game/MapObj/ElectricRailHolder.hpp"
#include "Game/Util.hpp"

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
}  // namespace NrvBossBegoman

namespace {
    const MR::ActorMoveParam hWaitParam = {0.0f, 3.0f, 0.95f, 1.0f};
    const MR::ActorMoveParam hSignAttackParam = {0.0f, 3.0f, 0.8f, 3.0f};
    const MR::ActorMoveParam hPursueParam = {1.3f, 3.0f, 0.97f, 3.0f};
    const MR::ActorMoveParam hTurnParam = {0.4f, 3.0f, 0.95f, 0.0f};
    const MR::ActorMoveParam hHitReactionParam = {0.0f, 3.0f, 0.95f, 0.0f};
    const MR::ActorMoveParam hOnWeakParam = {-0.5f, 3.0f, 0.95f, 3.0f};
    const MR::ActorMoveParam hOnWeakNoMoveParam = {0.0f, 3.0f, 0.95f, 3.0f};
}  // namespace

BossBegoman::BossBegoman(const char* pName)
    : BegomanBase(pName), mBabyFollowers(nullptr), mSpikeFollowers(nullptr), mBabyFollowerNum(0), mSpikeFollowerNum(0), mFollowerKind(FollowerKind_BothFollower),
      mPath(nullptr), mHead(nullptr), mHealth(3), _150(0.2f), mOpeningDemoInfo(nullptr) {
    mHeadMtx.identity();
}

void BossBegoman::init(const JMapInfoIter& rIter) {
    initCore(rIter, "BossBegoman", true);

    MR::calcGravity(this);

    mInitPos.sub(mGravity.scaleInline(10.0f));
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
    mBabyFollowers = new BegomanBaby*[3];

    for (int i = 0; i < mBabyFollowerNum; i++) {
        BegomanBaby* newBaby = new BegomanBaby("子分ベビー");
        mBabyFollowers[i] = newBaby;
        newBaby->_134 = this;
        newBaby->mPosition.set(this->mPosition);
        newBaby->initWithoutIter();
        newBaby->makeActorDead();
    }

    mSpikeFollowerNum = 2;
    mSpikeFollowers = new BegomanBase*[2];

    for (int i = 0; i < mSpikeFollowerNum; i++) {
        // should be begomanSpike
        BegomanBase* newSpike = new BegomanBaby("子分トゲ");
        mSpikeFollowers[i] = newSpike;
        newSpike->mPosition.set(this->mPosition);
        newSpike->initWithoutIter();
        newSpike->makeActorDead();
    }

    if (MR::tryRegisterDemoCast(this, rIter)) {
        MR::registerDemoActionNerve(this, &NrvBossBegoman::HostTypeNrvFirstContactDemo::sInstance, nullptr);
        for (int i = 0; i < mBabyFollowerNum; i++) {
            MR::tryRegisterDemoCast(mBabyFollowers[i], rIter);
        }
        //i is stored in the wrong register
        for (int i = 0; i < mSpikeFollowerNum; i++) {
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
    mVelocity.add(mGravity.scaleInline(-getRailPushJumpBoss()));
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

    MR::startLevelSound(this, "SE_BM_LV_BBEGO_PRE_DEMO_FLY", -1, -1, -1);
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
        MR::startBossBGM(0);
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
    updateRotateY(0.15f, 0.005f);
    exeWaitCore(::hWaitParam, &NrvBossBegoman::HostTypeNrvSignAttack::sInstance, &NrvBossBegoman::HostTypeNrvKeepDistance::sInstance,
                &NrvBossBegoman::HostTypeNrvNoCalcWait::sInstance);
}

void BossBegoman::exeSignAttack() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Attack");
        MR::startSound(this, "SE_BM_BBEGO_PRE_PURSUE", -1, -1);
    }

    updateRotateY(0.5f, 0.005f);
    exeSignAttackCore(::hSignAttackParam, &NrvBossBegoman::HostTypeNrvPursue::sInstance);
}

void BossBegoman::exePursue() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_BM_BBEGO_PURSUE_START", -1, -1);
    }

    updateRotateY(0.5f, 0.005f);
    exePursueCore(::hPursueParam, &NrvBossBegoman::HostTypeNrvBrake::sInstance, &NrvBossBegoman::HostTypeNrvTurn::sInstance, *getSoundBoss(), _150);
}

void BossBegoman::exeTurn() {
    if (MR::isFirstStep(this)) {
        mHead->tryTurn();
    }

    if (MR::isBckPlaying(this, "Turn")) {
        MR::emitEffectHit(this, mPosition.addOperatorInLine(mFaceVec.scaleInline(180.0f)), "EdgeSpark");
    }

    updateRotateY(0.4f, 0.005f);
    MR::startLevelSound(this, "SE_BM_LV_BBEGO_TURN", -1, -1, -1);
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

    updateRotateY(0.4f, 0.005f);

    if (MR::calcDistanceToPlayer(mPosition) < 600.0f) {
        MR::moveAndTurnToPlayer(this, &mFaceVec, ::hOnWeakParam._0, ::hOnWeakParam._4, ::hOnWeakParam._8, ::hOnWeakParam._C);
        addVelocityEscapeToSide(0.35f);
    } else {
        MR::moveAndTurnToPlayer(this, &mFaceVec, ::hOnWeakNoMoveParam._0, ::hOnWeakNoMoveParam._4, ::hOnWeakNoMoveParam._8, ::hOnWeakNoMoveParam._C);
    }

    if (isNerve(&NrvBossBegoman::HostTypeNrvOnWeakTurn::sInstance)) {
        if (MR::isGreaterStep(this, 30)) {
            setNerve(&NrvBossBegoman::HostTypeNrvOnWeak::sInstance);
        }
    } else {
        if (MR::isGreaterStep(this, 420)) {
            setNerve(&NrvBossBegoman::HostTypeNrvAware::sInstance);
        } else if (MR::isStep(this, 240)) {
            MR::startBrk(mHead, "SignWait");
            MR::setBrkRate(mHead, 0.5f);
        } else if (MR::isStep(this, 360)) {
            MR::startBrk(mHead, "SignWait");
            MR::setBrkRate(mHead, 2.0f);
        }

        if (MR::isGreaterEqualStep(this, 360)) {
            MR::startLevelSound(this, "SE_BM_LV_BBEGO_ALARM_FAST", -1, -1, -1);
        } else if (MR::isGreaterEqualStep(this, 240)) {
            MR::startLevelSound(this, "SE_BM_LV_BBEGO_ALARM", -1, -1, -1);
        }
    }
}

void BossBegoman::endOnWeak() {
    MR::startBrk(mHead, "OffWait");
}

void BossBegoman::exeBrake() {
    updateRotateY(0.2f, 0.005f);
    MR::startLevelSound(this, "SE_EM_LV_BEGOMAN_SPARK", -1, -1, -1);
    exeBrakeCore(&NrvBossBegoman::HostTypeNrvTurn::sInstance);
}

void BossBegoman::exeStepBack() {
    //result of call is unused
    if (MR::isFirstStep(this)) {
    }

    updateRotateY(0.2f, 0.005f);

    if (isNerve(&NrvBossBegoman::HostTypeNrvStepBackOnWeak::sInstance)) {
        exeStepBackCore(::hWaitParam, &NrvBossBegoman::HostTypeNrvOnWeak::sInstance);
    } else {
        exeStepBackCore(::hWaitParam, &NrvBossBegoman::HostTypeNrvWait::sInstance);
    }
}

void BossBegoman::exeReturn() {
    updateRotateY(0.2f, 0.005f);
    exeReturnCore(&NrvBossBegoman::HostTypeNrvWait::sInstance);
}

void BossBegoman::exeProvoke() {
    //result of call is unused
    if (MR::isFirstStep(this)) {
    }

    updateRotateY(0.3f, 0.005f);
    exeProvokeCore(::hWaitParam, &NrvBossBegoman::HostTypeNrvSignAttack::sInstance);
}

void BossBegoman::exeTrampleReaction() {
    if (MR::isFirstStep(this)) {
        mHead->trySwitchPushTrample();

        if (!mHead->isSwitchOn()) {
            MR::startAction(this, "TrampleReaction");
            MR::startSound(this, "SE_BM_BBEGO_NEEDLE_ON", -1, -1);
        } else {
            MR::startAction(this, "HopEnd");
            MR::startSound(this, "SE_BM_BBEGO_STOMPED", -1, -1);
            MR::startSound(this, "SE_BM_BBEGO_NEEDLE_OFF", -1, -1);
        }
    }
    updateRotateY(0.25f, 0.005f);
    MR::moveAndTurnToPlayer(this, &mFaceVec, 0.0f, 3.0f, 0.95f, 3.0f);

    if (mHead->isSwitchOn()) {
        if (MR::isGreaterStep(this, 80)) {
            setNerve(&NrvBossBegoman::HostTypeNrvOnWeak::sInstance);
        }
    } else if (MR::isGreaterStep(this, 80)) {
        setNerve(&NrvBossBegoman::HostTypeNrvSignAttack::sInstance);
    }
}

void BossBegoman::exeAware() {
    if (MR::isFirstStep(this)) {
        mVelocity.zero();
    }

    updateRotateY(0.1f, 0.005f);
    MR::moveAndTurnToPlayer(this, &mFaceVec, 0.0f, 3.0f, 0.95f, 0.0f);
    edgeRecoverCore();

    if (MR::isGreaterStep(this, 60)) {
        tryLaunchFollower();
        setNerve(&NrvBossBegoman::HostTypeNrvSignAttack::sInstance);
    }
}

void BossBegoman::exeHitReaction() {
    updateRotateY(0.25f, 0.005f);
    exeHitReactionCore(::hHitReactionParam, &NrvBossBegoman::HostTypeNrvProvoke::sInstance);
}

void BossBegoman::exeBlow() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_EM_BEGOMAN_ROT_STOP", -1, -1);
        MR::startAction(this, "Damage");
        MR::stopScene(2);
    }

    MR::startLevelSound(this, "SE_EM_LV_BEGOMAN_SPARK", -1, -1, -1);

    MR::moveAndTurnToDirection(this, &mFaceVec, mTargetVec, ::hHitReactionParam._0, ::hHitReactionParam._4, ::hHitReactionParam._8,
                               ::hHitReactionParam._C);
    reboundWallAndGround(&mFaceVec, false);

    if (MR::isGreaterStep(this, 20) && MR::isOnGround(this)) {
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
        MR::startSound(this, "SE_EM_BEGOMAN_ELEC_DAMAGE", -1, -1);
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
            MR::appearStarPieceToDirection(this, mPosition.subOperatorInLine(mGravity.scaleInline(200.0f)), vec, 8, 20.0f, 40.0f, false);
            MR::startSound(this, "SE_OJ_STAR_PIECE_BURST", -1, -1);
        } else if (mHealth == 1) {
            MR::appearStarPieceToDirection(this, mPosition.subOperatorInLine(mGravity.scaleInline(200.0f)), vec, 16, 20.0f, 40.0f, false);
            MR::startSound(this, "SE_OJ_STAR_PIECE_BURST", -1, -1);
        }
    }

    if (MR::isGreaterStep(this, 60)) {
        MR::stopScene(5);
        MR::shakeCameraWeak();
        
        if (mHealth == 0) {
            MR::startSound(this, "SE_BM_BBEGO_DEAD", -1, -1);
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
        MR::startSound(this, "SE_BM_BBEGO_NEEDLE_ON", -1, -1);
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

            TVec3f vec3(vec1);
            PSVECCrossProduct(&vec2, &vec3, &vec3);

            TVec3f vec4(vec3);
            PSVECCrossProduct(&vec2, &vec4, &vec4);

            if (0.0f < vec1.dot(vec4)) {
                vec4 = -vec4;
            }

            vec4.setLength(25.0f);
            mVelocity.set(vec4);
        }
    }

    reboundWallAndGround(&mFaceVec, false);
    MR::moveAndTurnToPlayer(this, &mFaceVec, 0.0f, 3.0f, 0.8f, 0.0f);

    if (MR::isGreaterStep(this, 60)) {
        mFaceVec.set(mTargetVec);
        setNerve(&NrvBossBegoman::HostTypeNrvJumpToInitPos::sInstance);
    }
}

void BossBegoman::exeJumpToInitPos() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Shake");
        MR::startSound(this, "SE_BM_BBEGO_BIG_JUMP", -1, -1);
        // vec1 goes completely unused
        TVec3f vec1(mInitPos);
        vec1.sub(mPosition);
        vec1.scale(0.5f);

        TVec3f vec2(mGravity);
        vec2 = -vec2;

        mPath->initFromUpVector(mPosition, mInitPos.subOperatorInLine(mGravity.scaleInline(10.0f)), vec2, 700.0f);
        mVelocity.zero();
    }

    updateRotateY(0.1f, 0.005f);
    mPath->calcPosition(&mPosition, MR::calcNerveRate(this, 60));

    if (MR::isGreaterStep(this, 60)) {
        setNerve(&NrvBossBegoman::HostTypeNrvSignAttack::sInstance);
        tryLaunchFollower();
        MR::startSound(this, "SE_BM_BBEGO_NEEDLE_ON", -1, -1);
        mHead->tryForceRecover();
        MR::tryRumblePadAndCameraDistanceMiddle(this, 800.0f, 1200.0f, 2000.0f);
    }
}

void BossBegoman::exeKeepDistance() {
    updateRotateY(0.2f, 0.005f);
    exeKeepDistanceCore(&NrvBossBegoman::HostTypeNrvWait::sInstance, &NrvBossBegoman::HostTypeNrvSignAttack::sInstance,
                        &NrvBossBegoman::HostTypeNrvBrake::sInstance, 800.0f, 600.0f);
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
            launchBegomanBabyFromGuarder(this, mBabyFollowers, mBabyFollowerNum, 100.0f, 10.0f, 15.0f, &vec);
        } else if (mFollowerKind == FollowerKind_SpikeFollower) {
            launchBegoman(this, mSpikeFollowers, mSpikeFollowerNum, 100.0f, 10.0f, 15.0f, &vec);
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
        return isDeadAllFollowerCore((BegomanBase**)mBabyFollowers, mBabyFollowerNum);
    } else {
        return isDeadAllFollowerCore((BegomanBase**)mSpikeFollowers, mSpikeFollowerNum);
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

    if (mHead->isEdgeOut() && MR::isPlayerExistSide(this, 80.0f, 0.25f)) {
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
            mVelocity.add(dirReceiverToSender.scaleInline(2.0f));

            if (rebounded) {
                MR::startSound(this, "SE_EM_BEGOMAN_COLLI", -1, -1);
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
        MR::addVelocityLimit(this, vec2.scaleInline(6.0f).scaleInline(pSender->mRadius / getSensor("body")->mRadius));
        
        if (rebounded) {
            MR::startSound(this, "SE_EM_BEGOMAN_COLLI_BEGOMAN", -1, -1);
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

    if (MR::isMsgPlayerSpinAttack(msg) && !mHead->isSwitchOn() && MR::isPlayerExistSide(this, 80.0f, 0.25f)) {
        MR::sendMsgEnemyAttackFire(pSender, pReceiver);
        setNerve(&NrvBossBegoman::HostTypeNrvProvoke::sInstance);
        return false;
    }

    if (isNerve(&NrvBossBegoman::HostTypeNrvTrampleReaction::sInstance) && !MR::isGreaterStep(this, 30)) {
        return false;
    }

    if (!mHead->isEdgeOut() && MR::isMsgPlayerHitAll(msg)) {
        calcBlowReaction(pSender->mPosition, pReceiver->mPosition, 40.0f, 40.0f);
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
            TVec3f vec3(vec2);
            PSVECCrossProduct(&mGravity, &vec3, &vec3);

            vec3.setLength(25.0f);
            mVelocity = vec3;
        } else {
            TVec3f vec4;
            MR::getPlayerFrontVec(&vec4);
            PSVECCrossProduct(&mGravity, &vec4, &vec4);

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
    mHeadMtx.getYDirInline(vec);

    if (!MR::isSameDirection(vec, mTargetVec, 0.01f)) {
        MR::makeMtxUpFront(&mHeadMtx, vec, mTargetVec);
    }
}
void BossBegoman::startRotationLevelSound() {
    if (isNerve(&NrvBossBegoman::HostTypeNrvWait::sInstance)) {
        MR::startLevelSound(this, "SE_BM_LV_BBEGO_ROT_SLOW", -1, -1, -1);
    } else if (isNerve(&NrvBossBegoman::HostTypeNrvPursue::sInstance)) {
        MR::startLevelSound(this, "SE_BM_LV_BBEGO_PURSUE", -1, -1, -1);
    } else if (isNerve(&NrvBossBegoman::HostTypeNrvTrampleReaction::sInstance) || isNerve(&NrvBossBegoman::HostTypeNrvOnWeak::sInstance)) {
        MR::startLevelSound(this, "SE_BM_LV_BBEGO_ROT_WEAK", -1, -1, -1);
    } else {
        MR::startLevelSound(this, "SE_BM_LV_BBEGO_ROT_MIDDLE", -1, -1, -1);
    }
    
    if (!mHead->isSwitchOn()) {
        f32 f1 = mVelocity.length();
        f1 *= f1;
        MR::startLevelSound(this, "SE_BM_LV_BBEGO_ROT_NEEDLE", MR::getLinerValueFromMinMax(f1, 0.0f, 121.0f, 70.0f, 100.0f), -1, -1);
    }
}
