#include "Game/NPC/DemoRabbit.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Util.hpp"

namespace {
    static const f32 sUpVecBlendRate = 0.1f;
    static const f32 sFrontVecBlendRate = 0.2f;
    static const f32 sGroundGravityAccel = 0.1f;
    static const f32 sAirGravityAccel = 1.0f;
    static const f32 sGroundFric = 0.9f;
    static const f32 sAirFric = 0.99f;
    static const s32 sIsAirTime = 5;
    static const f32 sInRunawayRange = 800.0f;
    static const f32 sOutRunawayRange = 1100.0f;
    static const f32 sTalkRunawayRange = 1300.0f;
    static const f32 sRunawayAccel = 1.55f;
    static const f32 sRunawayEndAccel = 1.2f;
    static const s32 sRunawayDeccelTime = 900;
    static const f32 sRunawayTurnStartLimit = 10.0f;
    static const f32 sRunawayTurnLimit = 1.5f;
    static const s32 sRunawayTurnDeccelTime = 30;
    // static const f32 sAwayTargetVelocityRate;
    static const f32 sWallJumpPowerV = 30.0f;
    static const f32 sAppearJumpPowerV = 20.0f;
    static const f32 sNearGoalDistance = 130.0f;
    static const f32 sReachGoalDistance = 100.0f;
};  // namespace

namespace NrvDemoRabbit {
    NEW_NERVE(DemoRabbitNrvAppear, DemoRabbit, Appear);
    NEW_NERVE(DemoRabbitNrvDemo, DemoRabbit, Demo);
    NEW_NERVE(DemoRabbitNrvTalk0, DemoRabbit, Talk);
    NEW_NERVE(DemoRabbitNrvTalk1, DemoRabbit, Talk);
    NEW_NERVE(DemoRabbitNrvWait, DemoRabbit, Wait);
    NEW_NERVE(DemoRabbitNrvGuide, DemoRabbit, Guide);
    NEW_NERVE(DemoRabbitNrvGoal, DemoRabbit, Goal);
    NEW_NERVE(DemoRabbitNrvRunaway, DemoRabbit, Runaway);
    NEW_NERVE(DemoRabbitNrvChange, DemoRabbit, Change);
    NEW_NERVE(DemoRabbitNrvStartBGM, DemoRabbit, StartBGM);
};  // namespace NrvDemoRabbit

void DemoRabbit_FORCE_MATCH_SDATA2() {
    (void) 1.0f;
    (void) 0.0f;
    (void) MR::epsilon();
    (void) 0.5f;
    (void) 3.0f;
    (void) 2.0f;
}

DemoRabbit::DemoRabbit(const char* pName) : NPCActor(pName) {
}

void DemoRabbit::makeArchiveList(NameObjArchiveListCollector* pCollector, const JMapInfoIter& rIter) {
    if (MR::getDemoCastID(rIter) == 0) {
        pCollector->addArchive("TrickRabbitBaby");
    } else {
        pCollector->addArchive("TrickRabbit");
    }
}

void DemoRabbit::init(const JMapInfoIter& rIter) {
    NPCActorCaps caps("DemoRabbit");
    caps.setDefault();
    caps.mObjectName = "TrickRabbit";
    caps.mRailRider = true;
    caps.mMessage = true;
    caps.mMakeActor = false;
    caps.mWaitNerve = GET_NERVE(DemoRabbit, DemoRabbitNrvAppear);

    if (MR::tryRegisterDemoCast(this, rIter)) {
        if (MR::getDemoCastID(rIter) == 0) {
            caps.mWaitNerve = GET_NERVE(DemoRabbit, DemoRabbitNrvAppear);
            caps.mObjectName = "TrickRabbitBaby";
            MR::invalidateClipping(this);
            MR::registerDemoActionNerve(this, GET_NERVE(DemoRabbit, DemoRabbitNrvTalk0), "チコとの出会い[ウサギ会話]");
            MR::registerDemoActionNerve(this, GET_NERVE(DemoRabbit, DemoRabbitNrvGuide), "チコとの出会い[ウサギ逃走]");
            MR::registerDemoActionFunctor(this, MR::Functor(this, &DemoRabbit::fadeOut), "ウサギ追いかけ[フェードアウト]");
            MR::registerDemoActionFunctor(this, MR::Functor(this, &DemoRabbit::fadeIn), "ウサギ追いかけ[フェードイン]");
            MR::registerDemoActionNerve(this, GET_NERVE(DemoRabbit, DemoRabbitNrvTalk1), "ウサギ追いかけ[会話]");
            MR::registerDemoActionNerve(this, GET_NERVE(DemoRabbit, DemoRabbitNrvRunaway), "ウサギ追いかけ[逃走]");
        } else {
            MR::registerDemoActionNerve(this, GET_NERVE(DemoRabbit, DemoRabbitNrvRunaway), "ウサギ追いかけ[逃走]");
            caps.mWaitNerve = GET_NERVE(DemoRabbit, DemoRabbitNrvDemo);
        }
    }

    initialize(rIter, caps);

    if (mMsgCtrl != nullptr) {
        MR::setDistanceToTalk(mMsgCtrl, ::sTalkRunawayRange);
        MR::offRootNodeAutomatic(mMsgCtrl);
    }

    MR::onCalcShadow(this, nullptr);
    MR::onCalcGravity(this);

    _A0.getZDir(mFrontVec);

    if (isNerve(GET_NERVE(DemoRabbit, DemoRabbitNrvAppear))) {
        makeActorDead();
    } else {
        makeActorAppeared();
    }
}

void DemoRabbit::initAfterPlacement() {
    if (isNerve(GET_NERVE(DemoRabbit, DemoRabbitNrvAppear)) && MR::isOnGameEventFlagEndTicoGuideDemo()) {
        MR::forwardNode(mMsgCtrl);
        makeActorAppeared();
        setNerve(GET_NERVE(DemoRabbit, DemoRabbitNrvStartBGM));
    }
}

void DemoRabbit::control() {
    MR::blendQuatUpFront(&_A0, -mGravity, mFrontVec, ::sUpVecBlendRate, ::sFrontVecBlendRate);

    if (MR::isBindedGround(this)) {
        mAirTimer = 0;
    } else {
        mAirTimer++;
    }
}

void DemoRabbit::fadeOut() {
    MR::timeKeepDemoFadeOut();
}

void DemoRabbit::fadeIn() {
    TVec3f railEndPos;
    MR::calcRailEndPos(&railEndPos, this);

    TVec3f gravityVector;
    MR::calcGravityVector(this, railEndPos, &gravityVector, nullptr, 0);

    TVec3f poly;

    MR::getFirstPolyOnLineToMap(&railEndPos, nullptr, railEndPos - gravityVector * 100.0f, gravityVector * 1000.0f);
    mPosition.set(railEndPos);

    MR::timeKeepDemoFadeIn();
}

void DemoRabbit::updateStopVelocity() {
    MR::attenuateVelocity(this, mAirTimer < ::sIsAirTime ? ::sGroundFric : ::sAirFric);
    MR::addVelocityToGravityOrGround(this, ::sGroundGravityAccel);
    MR::reboundVelocityFromCollision(this);

    TVec3f normal;
    MR::getFirstPolyNormalOnLineToMap(&normal, mPosition, mGravity * 10.0f, nullptr, nullptr);
    MR::vecKillElement(mVelocity, normal, &normal);
    mVelocity.sub(normal);
}

void DemoRabbit::updateNormalVelocity() {
    MR::attenuateVelocity(this, ::sGroundFric);

    if (MR::isBindedWall(this)) {
        MR::addVelocityToGravityOrGround(this, ::sGroundGravityAccel);
    } else if (MR::isBindedGround(this)) {
        MR::addVelocityToGravityOrGround(this, ::sGroundGravityAccel);
    } else {
        MR::addVelocityToGravityOrGround(this, ::sAirGravityAccel);
    }

    MR::reboundVelocityFromCollision(this, -0.05f);
}

void DemoRabbit::updateRun(const TVec3f& targetDir, bool unused) {
    f32 turnAngle = MR::calcNerveValue(this, ::sRunawayTurnDeccelTime, ::sRunawayTurnStartLimit, ::sRunawayTurnLimit);

    MR::turnDirectionDegree(this, &mFrontVec, targetDir - mPosition, turnAngle);

    f32 accel = MR::calcNerveValue(this, ::sRunawayDeccelTime, ::sRunawayAccel, ::sRunawayEndAccel);

    while (MR::isExistMapCollision(mPosition, mFrontVec * 200.0f)) {
        f32 oldLength = mFrontVec.length();
        mFrontVec = (mFrontVec + -mGravity) / 2.0f;
        mFrontVec.setLength(oldLength);
    }

    MR::addVelocityMoveToDirection(this, mFrontVec, mAirTimer < ::sIsAirTime ? accel : accel / 2.0f);
}

void DemoRabbit::updateJump() {
    if (MR::isOnGround(this)) {
        mVelocity -= *MR::getGroundNormal(this) * mVelocity.dot(*MR::getGroundNormal(this));
    }

    if (MR::isBindedWall(this) && MR::calcHitPowerToWall(this) >= 0.0f) {
        MR::addVelocityJump(this, ::sWallJumpPowerV);
    }
}

bool DemoRabbit::tryGuide() {
    if (MR::isNearPlayer(this, ::sInRunawayRange)) {
        MR::invalidateClipping(this);
        setNerve(GET_NERVE(DemoRabbit, DemoRabbitNrvGuide));
        return true;
    }
    return false;
}

bool DemoRabbit::tryWait() {
    bool isFar = !MR::isNearPlayer(this, ::sOutRunawayRange);
    if (isFar) {
        MR::validateClipping(this);
        setNerve(GET_NERVE(DemoRabbit, DemoRabbitNrvWait));
        return true;
    }

    return false;
}

bool DemoRabbit::tryGoal() {
    if (MR::isOnGround(this) && MR::isRailReachedNearGoal(this, ::sReachGoalDistance)) {
        MR::validateClipping(this);
        setNerve(GET_NERVE(DemoRabbit, DemoRabbitNrvGoal));
        return true;
    }

    return false;
}

void DemoRabbit::exeAppear() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Appear");
        MR::addVelocityJump(this, ::sAppearJumpPowerV);
        MR::calcVecToPlayerH(&mFrontVec, this, &mGravity);
        MR::startSound(this, "SE_SM_DEMORABBIT_APPEAR");
        MR::startSound(this, "SE_SM_DEMORABBIT_SMOKE");
    }

    MR::addVelocityToGravity(this, ::sAirGravityAccel);
    MR::attenuateVelocity(this, ::sAirFric);

    if (MR::isGreaterStep(this, 5) && MR::isBindedGround(this)) {
        mVelocity.zero();
        setNerve(GET_NERVE(DemoRabbit, DemoRabbitNrvDemo));
    }
}

void DemoRabbit::exeDemo() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Wait");
    }

    MR::turnDirectionToPlayerDegree(this, &mFrontVec, 10.0f);
    updateStopVelocity();
}

void DemoRabbit::exeTalk() {
    if (MR::isFirstStep(this)) {
        if (isNerve(GET_NERVE(DemoRabbit, DemoRabbitNrvTalk1))) {
            MR::forwardNode(mMsgCtrl);
        }

        MR::tryTalkTimeKeepDemoMarioPuppetable(mMsgCtrl);

        if (isNerve(GET_NERVE(DemoRabbit, DemoRabbitNrvTalk0))) {
            MR::forwardNode(mMsgCtrl);
        }
    }

    MR::turnDirectionToPlayerDegree(this, &mFrontVec, 10.0f);
    updateStopVelocity();
}

void DemoRabbit::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Wait");
    }

    MR::tryTalkNearPlayer(mMsgCtrl);
    MR::turnDirectionToPlayerDegree(this, &mFrontVec, 10.0f);
    updateStopVelocity();

    if (tryGuide()) {
        (void)0.0f;
    }
}

void DemoRabbit::exeGoal() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Wait");
    }

    MR::turnDirectionToPlayerDegree(this, &mFrontVec, 10.0f);
    updateStopVelocity();

    if (MR::isDemoActive()) {
        return;
    }

    if (MR::isNearPlayer(mMsgCtrl, 500.0f)) {
        MR::startTimeKeepDemoMarioPuppetable(this, "チコガイドデモ", "ウサギ追いかけ[フェードアウト]");
    } else {
        MR::tryTalkNearPlayer(mMsgCtrl);
    }
}

void DemoRabbit::exeGuide() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Run");
        MR::startSound(this, "SE_SM_RABBIT_JUMP");
    }

    if (MR::checkPassBckFrame(this, 3.0f)) {
        MR::startSound(this, "SE_SM_RABBIT_JUMP");
    }

    TVec3f railPosFrontCoord;
    MR::calcRailPosFrontCoord(&railPosFrontCoord, this, 200.0f);

    if (MR::isRailReachedNearGoal(this, ::sNearGoalDistance)) {
        updateRun(railPosFrontCoord - mGravity * 2.0f, true);
    } else {
        updateRun(railPosFrontCoord - mGravity * 2.0f, false);
    }

    updateNormalVelocity();
    updateJump();
    MR::moveCoordToNearestPos(this, mPosition);

    if (tryGoal() || tryWait()) {
        (void)0.0f;
    }
}

void DemoRabbit::exeRunaway() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Run");
        MR::startSound(this, "SE_SM_RABBIT_JUMP");
    }

    MR::invalidateClipping(this);
    if (MR::checkPassBckFrame(this, 3.0f)) {
        MR::startSound(this, "SE_SM_RABBIT_JUMP");
    }

    updateRun(mPosition + mPosition - *MR::getPlayerPos(), false);
    updateNormalVelocity();
    updateJump();

    if (MR::isGreaterEqualStep(this, 120)) {
        setNerve(GET_NERVE(DemoRabbit, DemoRabbitNrvChange));
    }
}

void DemoRabbit::exeChange() {
    if (MR::isFirstStep(this)) {
        MR::startAction(this, "Change");
        MR::startSound(this, "SE_SM_RABBIT_HIDE");
    }

    updateNormalVelocity();

    if (MR::isBckStopped(this)) {
        MR::startSound(this, "SE_SM_METAMORPHOSE_SMOKE");
        return kill();
    }
}

void DemoRabbit::exeStartBGM() {
    if (MR::isFirstStep(this) && !MR::isPlayingStageBgm()) {
        MR::startStageBGM("MBGM_GALAXY_24", false);
    }

    setNerve(GET_NERVE(DemoRabbit, DemoRabbitNrvGuide));
}
