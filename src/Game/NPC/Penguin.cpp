#include "Game/NPC/Penguin.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/MapObj/JetTurtle.hpp"
#include "Game/Scene/SceneFunction.hpp"

namespace {
    static const f32 sDistanceCall = 350.0f;
    // static const _32 sCallFrameStart =
    // static const _32 sCallFrameEnd =
    static const f32 sSpeedWalk = 1.5f;
    static const f32 sSpeedDash = 6.0f;
    static const f32 sBlendDash = 0.08f;
    static const f32 sSpeedSwim = 5.0f;
    static const f32 sSpeedSwimTurtle = 10.0f;
    static const s32 sStepToDiveMin = 120;
    static const s32 sStepToDiveMax = 300;
    // static const _32 sDistanceDiveMessage =
    // static const _32 sDistanceDiveTalk =
}  // namespace

namespace NrvPenguin {
    NEW_NERVE(PenguinNrvWait, Penguin, Wait);
    NEW_NERVE(PenguinNrvReaction, Penguin, Reaction);
    NEW_NERVE(PenguinNrvDive, Penguin, Dive);
    NEW_NERVE(PenguinNrvFlow, Penguin, Flow);
}  // namespace NrvPenguin

RemovableTurtle::RemovableTurtle(LiveActor* pHost, bool isGolden) {
    if (isGolden) {
        mShellModel = new TurtlePartsModel(pHost, "ペンギン用ゴールデンコウラ", "KouraShine", nullptr,
                                           MR::DrawBufferType_NoSilhouettedMapObjStrongLight, false);
        mJetTurtle = new GoldenTurtle("ゴールデン甲羅");
    } else {
        mShellModel = new PartsModel(pHost, "ペンギン用コウラ", "Koura", nullptr, MR::DrawBufferType_NoSilhouettedMapObjStrongLight, false);
        mJetTurtle = new JetTurtle("ジェット亀さん");
    }

    mShellModel->_99 = true;
    mShellModel->initFixedPosition(TVec3f(-5.85f, -68.0f, 30.0f), TVec3f(0.0f, 0.0f, 0.0f), nullptr);
    mShellModel->initWithoutIter();
    mShellModel->makeActorDead();

    mJetTurtle->initWithoutIter();
    mJetTurtle->_E0 = false;
    mJetTurtle->makeActorDead();
}

bool RemovableTurtle::isPullRange() const {
    return MR::isNearPlayer(mShellModel, 450.0f);
}

bool RemovableTurtle::tryRemove() {
    if (MR::isDead(mJetTurtle) && !MR::isPlayerCarryAny()) {
        mShellModel->makeActorDead();
        mJetTurtle->mPosition.set(mShellModel->mPosition);
        mJetTurtle->appearAndTryTaken();
        return true;
    }

    return false;
}

bool RemovableTurtle::tryAttach() {
    if (MR::isDead(mJetTurtle)) {
        mShellModel->appear();
        return true;
    }
    return false;
}

Penguin::Penguin(const char* pName) : NPCActor(pName), mBehavior(Behavior_Default), mStepToDive(0) {
}

void Penguin::init(const JMapInfoIter& rIter) {
    MR::initDefaultPosAndQuat(this, rIter);
    MR::getJMapInfoArg0NoInit(rIter, &mBehavior);
    initModelManagerWithAnm("Penguin", nullptr, false);
    MR::connectToSceneNpc(this);
    MR::initLightCtrl(this);

    initHitSensor(1);
    MR::addHitSensorMtxNpc(this, "push", 8, 80.0f, MR::getJointMtx(this, "CenterJoint"), TVec3f(0.0f, 0.0f, 0.0f));

    initEffectKeeper(0, nullptr, false);
    initSound(4, false);

    MR::initShadowVolumeSphere(this, 50.0f);
    MR::onCalcShadowOneTime(this, nullptr);

    initTalkCtrl(rIter, "Penguin", TVec3f(0.0f, 150.0f, 0.0f), nullptr);

    NPCActorCaps caps("");
    caps.mLodCtrl = true;
    caps.mNerve = false;
    caps.mPointer = true;
    caps._70 = "CenterJoint";
    initialize(rIter, caps);

    initNerve(&NrvPenguin::PenguinNrvWait::sInstance);

    if (MR::isConnectedWithRail(rIter)) {
        initRailRider(rIter);
        MR::moveCoordToNearestPos(this, mPosition);
        mPosition.set(MR::getRailPos(this));
    }

    if (mBehavior == Behavior_SwimTurtle) {
        mTurtle = new RemovableTurtle(this, false);
        mTurtle->tryAttach();
    }

    s32 color = 0;
    MR::getJMapInfoArg7NoInit(rIter, &color);
    MR::startBrk(this, "ColorChange");
    MR::setBrkFrameAndStop(this, color);

    setDefaults();
    _128 = false;
    _12C = sDistanceCall;

    switch (mBehavior) {
    case Behavior_Default:
    case Behavior_Stand:
        mParam.setMoveAction("Wait", "Turn");
        mParam.setTalkAction("Talk", "TalkTurn");
        setTalkAction("Walk");
        _10C = sSpeedWalk;
        _118 = 2.0f;  // talkBckRate?
        _124 = true;  // follow rail on ground
        _128 = true;
        break;
    case Behavior_Sit:
        mParam.setMoveTalkNoTurnAction("SitDown", "SitDownTalk");
        break;
    case Behavior_Swim:
        mParam.setMoveAction("SwimWait", "SwimWaitTalk");
        setTalkAction("Swim");
        _10C = sSpeedSwim;
        break;
    case Behavior_SwimSurface:
        mParam.setMoveAction("SwimWaitSurface", "SwimSurfaceTalk");
        setTalkAction("SwimSurface");
        _10C = sSpeedSwim;
        break;
    case Behavior_Dive:
        mParam.setSingleAction("SwimWaitSurface");
        break;
    case Behavior_SwimTurtle:
        mParam.setSingleAction("SwimTurtleTalk");
        setTalkAction("SwimTurtle");
        _10C = sSpeedSwimTurtle;
        break;
    case Behavior_Dash:
        mParam.setMoveAction("Wait", "Turn");
        mParam.setTalkAction("Talk", "TalkTurn");
        setTalkAction("DashA");
        _10C = sSpeedDash;
        _114 = sBlendDash;
        _124 = true;  // follow rail on ground
        _128 = true;
        break;
    default:
        break;
    }
    setNerve(&NrvPenguin::PenguinNrvWait::sInstance);
    makeActorAppeared();
}

void Penguin::initAfterPlacement() {
    NPCActor::initAfterPlacement();

    switch (mBehavior) {
    case Behavior_SwimSurface:
    case Behavior_Dive: {
        MR::calcGravity(this);
        MR::turnQuatYDirRad(&_A0, _A0, mGravity.negateInline(), PI);

        TVec3f pos;
        TVec3f grav = mGravity;
        Triangle triangle;
        if (MR::getFirstPolyOnLineToWaterSurface(&pos, &triangle, mPosition - grav * 100.0f, grav * 500.0f)) {
            mPosition.set(pos);
        }

        setInitPose();
    }
    default:
        break;
    }
}

void Penguin::exeReaction() {
    if (_D8) {
        MR::startSound(this, "SE_SM_PENGUIN_TRAMPLED", -1, -1);
        MR::startSound(this, "SE_SV_PENGUIN_S_TRAMPLED", -1, -1);
    }

    if (isPointingSe()) {
        MR::startSound(this, "SE_SV_PENGUIN_S_POINTED_2P", -1, -1);
        MR::startDPDHitSound();
    }

    if (_D9) {
        MR::startSound(this, "SE_SV_PENGUIN_S_SPIN_HIT", -1, -1);
    }

    if (_DB) {
        MR::startSound(this, "SE_SV_PENGUIN_S_STAR_PIECE", -1, -1);
    }

    if (MR::tryStartReactionAndPopNerve(this)) {
        return;
    }
}

void Penguin::exeWait() {
    if (MR::isFirstStep(this)) {
        mStepToDive = MR::getRandom(sStepToDiveMin, sStepToDiveMax);
        if (MR::isExistRail(this)) {
            MR::onCalcShadow(this, nullptr);
        }
    }

    if (MR::tryStartReactionAndPushNerve(this, &NrvPenguin::PenguinNrvReaction::sInstance)) {
        return;
    }

    switch (mBehavior) {
    case Behavior_Default:
    case Behavior_Stand:
    case Behavior_Dash:
        MR::tryChangeTalkActionRandom(this, "Talk", "TalkA", "TalkB");
    default:
        break;
    }

    MR::tryTalkNearPlayerAndStartMoveTalkAction(this);

    if (mBehavior == Behavior_SwimTurtle) {
        MR::startLevelSound(this, "SE_SM_LV_PENGUIN_SWIM_JET", -1, -1, -1);
    }

    if (mBehavior == Behavior_Dive && MR::isStep(this, mStepToDive)) {
        setNerve(&NrvPenguin::PenguinNrvDive::sInstance);
    }
}

void Penguin::exeDive() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "SwimDive", static_cast< const char* >(nullptr));
        MR::startSound(this, "SE_SV_PENGUIN_S_DIVE", -1, -1);
    }

    if (MR::isBckStopped(this)) {
        setNerve(&NrvPenguin::PenguinNrvWait::sInstance);
    }
}

void Penguin::exeFlow() {
    if (MR::isFirstStep(this)) {
        _11C = "Swim";
        _120 = "Swim";
        _10C = sSpeedSwim;
    }

    MR::tryTalkNearPlayerAndStartMoveTalkAction(this);

    if (mTurtle->tryAttach()) {
        mParam.setSingleAction("SwimTurtleTalk");
        _11C = "SwimTurtle";
        _120 = "SwimTurtle";
        _10C = sSpeedSwimTurtle;
        setNerve(&NrvPenguin::PenguinNrvWait::sInstance);
    }
}

void Penguin::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver)) {
        tryReleaseTurtle();
    }

    NPCActor::attackSensor(pSender, pReceiver);
}

bool Penguin::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgSpinStormRange(msg) && tryReleaseTurtle()) {
        return true;
    }

    return false;
}

bool Penguin::tryReleaseTurtle() {
    if (mBehavior == Behavior_SwimTurtle && isNerve(&NrvPenguin::PenguinNrvWait::sInstance) && mTurtle->isPullRange() && mTurtle->tryRemove()) {
        setNerve(&NrvPenguin::PenguinNrvFlow::sInstance);
        return true;
    }

    return false;
}
