#include "Game/NPC/CareTaker.hpp"
#include "Game/Effect/EffectSystem.hpp"
#include "Game/Effect/EffectSystemUtil.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/NPC/NPCActorItem.hpp"
#include "Game/NPC/TalkMessageFunc.hpp"
#include "Game/NameObj/NameObjArchiveListCollector.hpp"
#include "Game/Screen/BombTimerLayout.hpp"
#include "Game/Util.hpp"

namespace {
    static const s32 sTidyTimeLimit = 1800;
    static const s32 sBgmStartStep = 166;
    // static const f32 sTalkRotateDegree = _;
    // static const f32 sTalkCameraScale = _;
    // static const s32 sDirtBtkFrame = _;
    static const s32 sPreRaceWait = 100;
    static const s32 sPstRaceWait = 50;
    // static const s32 sNormalBtkFrame = _;
};  // namespace

namespace NrvCaretaker {
    NEW_NERVE(CaretakerNrvTakeOutStar, Caretaker, TakeOutStar);
    NEW_NERVE(CaretakerNrvReaction, Caretaker, Reaction);
    NEW_NERVE(CaretakerNrvTalk, Caretaker, Talk);
    NEW_NERVE(CaretakerNrvWait, Caretaker, Wait);
    NEW_NERVE(CaretakerNrvPreTalk, Caretaker, PreTalk);
    NEW_NERVE(CaretakerNrvPreWipeOut, Caretaker, PreWipeOut);
    NEW_NERVE(CaretakerNrvPreWipeIn, Caretaker, PreWipeIn);
    NEW_NERVE(CaretakerNrvPreWait, Caretaker, PreWait);
    NEW_NERVE(CaretakerNrvCountDown, Caretaker, CountDown);
    NEW_NERVE(CaretakerNrvTidy, Caretaker, Tidy);
    NEW_NERVE(CaretakerNrvRunaway, Caretaker, Runaway);
    NEW_NERVE(CaretakerNrvPstWipeOut, Caretaker, PstWipeOut);
    NEW_NERVE(CaretakerNrvPstWipeIn, Caretaker, PstWipeIn);
    NEW_NERVE(CaretakerNrvPstWait, Caretaker, PstWait);
    NEW_NERVE(CaretakerNrvPstTalk, Caretaker, PstTalk);
    NEW_NERVE(CaretakerNrvAppearedStar, Caretaker, AppearedStar);
};  // namespace NrvCaretaker

Caretaker::Caretaker(const char* pName) : NPCActor(pName), mBombTimerLayout(), mObjArg0(-1), mTidyTimeLimit(::sTidyTimeLimit) {
}

void Caretaker::makeArchiveList(NameObjArchiveListCollector* pCollector, const JMapInfoIter& rIter) {
    NPCActorItem item("Caretaker");

    pCollector->addArchive("CaretakerMiddle");
    pCollector->addArchive("CaretakerLow");

    s32 arg0 = -1;
    MR::getJMapInfoArg0NoInit(rIter, &arg0);

    if (arg0 != 0) {
        return;
    }

    pCollector->addArchive("BombTimer");

    if (MR::getNPCItemData(&item, 0)) {
        NPCActor::addArchive(pCollector, item);
    }
}

void Caretaker::init(const JMapInfoIter& rIter) {
    NPCActorCaps caps = NPCActorCaps("Caretaker");
    NPCActorItem item = NPCActorItem("Caretaker");
    caps.setDefault();
    caps.mSoundSize = 8;
    caps.mRailRider = true;
    caps._5D = 1;
    caps.mWaitNerve = &NrvCaretaker::CaretakerNrvTalk::sInstance;
    caps.mSensorSize = 100.0f;

    s32 arg3 = 0;
    MR::getJMapInfoArg3NoInit(rIter, &arg3);
    mBodyColorFrame = arg3;

    s32 arg4 = 0;
    MR::getJMapInfoArg4NoInit(rIter, &arg4);
    mObjArg4 = arg4;

    MR::startBrk(this, "BodyColor");
    MR::setBrkFrameAndStop(this, mBodyColorFrame);
    MR::startBtk(this, "Dirt");
    MR::setBtkFrameAndStop(this, 0.0f);

    _134 = "BTrampled";
    _130 = "BSpinHit";
    _13C = "BSpinHit";
    _138 = "BTalkHelp";
    mParam._14 = "BWaitStand";
    mParam._18 = "BWaitStand";
    _11C = "BWaitRun";
    _120 = "BRunTalk";
    mParam._0 = 0;
    mParam._1 = 0;
    mParam._8 = 3.0f;
    _10C = 2.0f;
    _110 = 0.1f;
    _114 = 0.05f;
    _124 = 1;

    setAnim(mObjArg4);

    s32 arg0 = -1;
    MR::getJMapInfoArg0NoInit(rIter, &arg0);
    mObjArg0 = arg0;

    MR::startBckNoInterpole(this, "Wait");
    MR::calcAnimDirect(this);

    if (mMsgCtrl != nullptr) {
        MR::registerBranchFunc(mMsgCtrl, TalkMessageFunc(this, &Caretaker::branchFuncComet));
        MR::registerEventFunc(mMsgCtrl, TalkMessageFunc(this, &Caretaker::eventFuncComet));
    }

    if (mObjArg0 == -1 || mObjArg0 == 1) {
        return;
    }

    _130 = "SpinHit";
    _13C = "SpinHit";
    _134 = "Trampled";
    _138 = "TalkAngry";
    mParam._14 = "Wait";
    mParam._18 = "Wait";
    mParam._1C = "TalkNormal";
    mParam._20 = "TalkNormal";
    _11C = "WaitRun";
    _120 = "WaitRun";

    MR::getNPCItemData(&item, 0);
    equipment(item, false);

    mObjArg0 = 0;

    if (MR::isExistRail(this)) {
        MR::moveCoordAndFollowTrans(this);
        _C0 = mPosition;
    }

    setNerve(&NrvCaretaker::CaretakerNrvWait::sInstance);
    MR::needStageSwitchReadA(this, rIter);
    MR::needStageSwitchWriteB(this, rIter);
    MR::declarePowerStar(this);

    mTakeOutStar = new TakeOutStar(this, "TakeOutStarCaretaker", "TakeOutStarCaretaker", &NrvCaretaker::CaretakerNrvTakeOutStar::sInstance);

    if (MR::isPlayerLuigi()) {
        MR::getJMapInfoArg2NoInit(rIter, &mTidyTimeLimit);
    } else {
        MR::getJMapInfoArg1NoInit(rIter, &mTidyTimeLimit);
    }

    mBombTimerLayout = new BombTimerLayout(true);
    mBombTimerLayout->initWithoutIter();
    mBombTimerLayout->setTimeLimit(mTidyTimeLimit * 60);
    mBombTimerLayout->kill();

    _168 = MR::joinToGroupArray(this, rIter, "ゴミ管理", 32);

    if (mMsgCtrl != nullptr) {
        MR::registerBranchFunc(mMsgCtrl, TalkMessageFunc(this, &Caretaker::branchFuncStar));
        MR::registerAnimeFunc(mMsgCtrl, TalkMessageFunc(this, &Caretaker::animeFunc));
        MR::setMessageArg(mMsgCtrl, mTidyTimeLimit);
        MR::setDistanceToTalk(mMsgCtrl, 350.0f);
    }

    mCameraInfo = MR::createActorCameraInfo(rIter);

    MR::initActorCamera(this, rIter, &mCameraInfo);
}

void Caretaker::initAfterPlacement() {
    if (MR::isExistRail(this)) {
        MR::followRailPoseOnGround(this, 1.0f);
    }
}

void Caretaker::kill() {
    NPCActor::kill();

    if (mBombTimerLayout != nullptr) {
        mBombTimerLayout->kill();
    }
}

void Caretaker::control() {
    MR::startLevelSound(this, "SE_SM_LV_CARETAKER_WAIT");
    NPCActor::control();
}

bool Caretaker::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgPlayerSpinAttack(msg)) {
        _E3 = 1;

        return true;
    }

    return NPCActor::receiveMsgPlayerAttack(msg, pSender, pReceiver);
}

bool Caretaker::branchFuncStar(u32 msg) {
    switch (msg) {
    case 0:
        return MR::isOnSwitchA(this);
    default:
        return false;
    }
}

bool Caretaker::branchFuncComet(u32 msg) {
    s32 purpleCoinNum = MR::getPurpleCoinNum();

    switch (msg) {
    case 0:
        return purpleCoinNum == 0;
    case 1:
        return purpleCoinNum >= 1 && purpleCoinNum < 50;
    case 2:
        return purpleCoinNum >= 50 && purpleCoinNum < 80;
    case 3:
        return purpleCoinNum >= 80 && purpleCoinNum < 90;
    case 4:
        return purpleCoinNum >= 90 && purpleCoinNum < 95;
    case 5:
        return purpleCoinNum >= 95 && purpleCoinNum < 100;
    case 6:
        return purpleCoinNum == 100;
    default:
        return false;
    }
}

bool Caretaker::eventFuncComet(u32 msg) {
    if (msg == 0) {
        return mTakeOutStar->takeOut();
    } else if (msg == 1) {
        MR::forceKillPlayerByGroundRace();

        return true;
    } else if (msg == 2) {
        MR::setMessageArg(mMsgCtrl, MR::getPurpleCoinNum());

        return true;
    }
}

bool Caretaker::animeFunc(u32 msg) {
    switch (msg) {
    case 0:
        MR::startAction(this, "TalkNormal");
        break;
    case 1:
        MR::startAction(this, "TalkAngry");
        break;
    case 2:
        MR::startAction(this, "TalkCry");
        break;
    }

    return true;
}

void Caretaker::setAnim(s32 anim) {
    switch (anim) {
    case 0:
        mParam.setTalkAction("BTalkNormal", "BTalkNormal");
        break;
    case 1:
        mParam.setTalkAction("BTalkCry", "BTalkCry");
        break;
    case 2:
        mParam.setTalkAction("BTalkSpin", "BTalkSpin");
        break;
    case 3:
        mParam.setTalkAction("BTalkSurprise", "BTalkSurprise");
        break;
    case 4:
        mParam.setTalkAction("BTalkSpring", "BTalkSpring");
        break;
    case 5:
        mParam.setSingleAction("BTalkHelp");
        break;
    case 6:
        MR::invalidateLodCtrl(this);
        MR::setBtkFrameAndStop(this, 2.0f);
        mParam.setMoveTalkNoTurnAction("BTalkSurvive", "BTalkSurvive");
        break;
    }
}

void Caretaker::updateCounterSE() {
    if (MR::isPlayerDead()) {
        return;
    }

    if (MR::isPlayerConfrontDeath()) {
        return;
    }

    u32 restTime = mBombTimerLayout->getRestTime();

    if (restTime == 0) {
        MR::startSystemSE("SE_SY_E3_TIMER_TIME_UP");
    } else if (restTime <= 120) {
        if (restTime % 60 == 0) {
            MR::startSystemSE("SE_SY_E3_TIMER_COUNT_4");
        }
    } else if (restTime <= 360) {
        if (restTime % 60 == 0) {
            MR::startSystemSE("SE_SY_E3_TIMER_COUNT_2");
        }
    } else if (restTime <= mBombTimerLayout->mDangerTransFrame) {
        if (restTime % 60 == 0) {
            MR::startSystemSE("SE_SY_E3_TIMER_COUNT_1");
        }
    }
}

void Caretaker::exeTakeOutStar() {
}

void Caretaker::exeReaction() {
    if (_D8) {
        MR::startSound(this, "SE_SM_NPC_TRAMPLED_METAL");
        MR::startSound(this, "SE_SV_CARE_TAKER_TRAMPLE");
    }

    if (_D9) {
        MR::startSound(this, "SE_SM_CARETAKER_SPIN_HIT");
        MR::startSound(this, "SE_SV_CARE_TAKER_SPIN_HIT");
    }

    if (isPointingSe()) {
        MR::startSound(this, "SE_SV_CARE_TAKER_POINTED2P");
        MR::startDPDHitSound();
    }

    if (_DB) {
        MR::startSound(this, "SE_SM_CARETAKER_SPIN_HIT");
        MR::startSound(this, "SE_SV_CARE_TAKER_STAR_PIECE");
    }

    if (MR::tryStartReactionAndPopNerve(this)) {
        return;
    }
}

void Caretaker::exeTalk() {
    MR::tryTalkNearPlayerAndStartMoveTalkAction(this);
    MR::tryStartReactionAndPushNerve(this, &NrvCaretaker::CaretakerNrvReaction::sInstance);
}

void Caretaker::exeWait() {
    if (MR::tryTalkNearPlayerAndStartMoveTalkAction(this)) {
        MR::startNPCTalkCamera(mMsgCtrl, getBaseMtx(), 1.3f, -1);
        MR::setRailCoordSpeed(this, 0.0f);
        setNerve(&NrvCaretaker::CaretakerNrvPreTalk::sInstance);
    } else if (MR::tryStartReactionAndPushNerve(this, &NrvCaretaker::CaretakerNrvReaction::sInstance)) {
        return;
    }
}

void Caretaker::exePreTalk() {
    turnToPlayer(mParam._8);

    if (!MR::tryTalkNearPlayerAtEnd(mMsgCtrl)) {
        return;
    }

    if (MR::tryTalkSelectLeft(mMsgCtrl)) {
        MR::tryStartDemoMarioPuppetableWithoutCinemaFrame(this, "ゴミ掃除タイムアタック");
        MR::startAction(this, "Wait");
        MR::startBckPlayer("Watch", static_cast< const char* >(nullptr));
        setNerve(&NrvCaretaker::CaretakerNrvPreWipeOut::sInstance);
    } else {
        MR::endNPCTalkCamera(false, -1);
        setNerve(&NrvCaretaker::CaretakerNrvWait::sInstance);
    }
}

void Caretaker::exePreWipeOut() {
    if (MR::isFirstStep(this)) {
        MR::stopStageBGM(90);
        MR::invalidateClipping(this);
        MR::closeWipeFade();
    }

    if (MR::isWipeActive()) {
        return;
    }

    MR::setRailCoordSpeed(this, 0.0f);
    MR::setRailCoord(this, 0.0f);
    MR::setDefaultPose(this);
    MR::setPlayerPosOnGroundAndWait("バトルシップ・タイムアタック前位置");
    MR::endNPCTalkCamera(false, 0);
    MR::callInvalidateClippingAllGroupMember(this);
    mBombTimerLayout->appear();
    mBombTimerLayout->suspend();
    mBombTimerLayout->setTimeLimit(mTidyTimeLimit * 60);
    setNerve(&NrvCaretaker::CaretakerNrvPreWipeIn::sInstance);
}

void Caretaker::exePreWipeIn() {
    if (MR::isFirstStep(this)) {
        MR::openWipeFade();
        MR::startSubBGM("BGM_MINIGAME_START", false);
    }

    if (MR::isWipeActive()) {
        return;
    }

    setNerve(&NrvCaretaker::CaretakerNrvPreWait::sInstance);
}

void Caretaker::exePreWait() {
    if (MR::isFirstStep(this)) {
        MR::endDemo(this, "ゴミ掃除タイムアタック");
        MR::offPlayerControl();
        MR::onSwitchB(this);
    }

    if (MR::isGreaterStep(this, ::sPreRaceWait)) {
        setNerve(&NrvCaretaker::CaretakerNrvCountDown::sInstance);
    }
}

void Caretaker::exeCountDown() {
    if (getNerveStep() % 60 == 0) {
        MR::startSystemSE("SE_SY_RACE_COUNT_DOWN");
    }

    if (MR::isStep(this, ::sBgmStartStep)) {
        MR::startStageBGM("MBGM_RACE_01", false);
    }

    if (MR::isGreaterEqualStep(this, 180)) {
        MR::onPlayerControl(true);
        setNerve(&NrvCaretaker::CaretakerNrvTidy::sInstance);
    }
}

void Caretaker::exeTidy() {
    if (MR::isFirstStep(this)) {
        mBombTimerLayout->resume();
        MR::startSystemSE("SE_SY_RACE_START");
    }

    if (MR::isPlayerInBind()) {
        mBombTimerLayout->kill();
        MR::stopStageBGM(60);
        setNerve(&NrvCaretaker::CaretakerNrvRunaway::sInstance);
    } else {
        updateCounterSE();

        if (MR::canStartDemo() && mBombTimerLayout->isReadyToTimeUp() || MR::isOnSwitchA(this)) {
            MR::tryStartDemoMarioPuppetableWithoutCinemaFrame(this, "ゴミ掃除タイムアタック");
            setNerve(&NrvCaretaker::CaretakerNrvPstWipeOut::sInstance);
        }
    }
}

void Caretaker::exeRunaway() {
    if (MR::isPlayerInBind()) {
        return;
    }

    MR::offSwitchB(this);
    MR::validateClipping(this);
    MR::callAppearAllGroupMember(this);
    MR::callValidateClippingAllGroupMember(this);
    MR::resetNode(mMsgCtrl);
    setNerve(&NrvCaretaker::CaretakerNrvWait::sInstance);
}

void Caretaker::exePstWipeOut() {
    if (MR::isFirstStep(this)) {
        if (MR::isOnSwitchA(this)) {
            MR::startSystemSE("SE_SY_TOTAL_COMPLETE");
            MR::stopStageBGM(10);
        } else {
            MR::stopStageBGM(60);
        }

        MR::closeWipeFade();
    }

    if (MR::isWipeActive()) {
        return;
    }

    MR::offSwitchB(this);

    if (MR::isOnSwitchA(this)) {
        MR::callKillAllGroupMember(this);
    } else {
        MR::callAppearAllGroupMember(this);
    }

    MR::callValidateClippingAllGroupMember(this);
    MR::Effect::forceDeleteAllEmitters(MR::getEffectSystem());
    MR::tryPlayerKillTakingActor();
    MR::setPlayerPosOnGroundAndWait("バトルシップ・タイムアタック後位置");
    MR::startBckPlayer("Watch", static_cast< const char* >(nullptr));
    MR::startNPCTalkCamera(mMsgCtrl, getBaseMtx(), 1.3f, 1);
    setNerve(&NrvCaretaker::CaretakerNrvPstWipeIn::sInstance);
}

void Caretaker::exePstWipeIn() {
    if (MR::isFirstStep(this)) {
        mBombTimerLayout->kill();
        MR::openWipeFade();
        MR::startLastStageBGM();
    }

    if (MR::isWipeActive()) {
        return;
    }

    MR::startAction(this, "TalkNormal");
    MR::validateClipping(this);
    setNerve(&NrvCaretaker::CaretakerNrvPstWait::sInstance);
}

void Caretaker::exePstWait() {
    if (MR::isGreaterEqualStep(this, ::sPstRaceWait)) {
        setNerve(&NrvCaretaker::CaretakerNrvPstTalk::sInstance);
    }
}

void Caretaker::exePstTalk() {
    if (!MR::tryTalkForceWithoutDemoMarioPuppetableAtEnd(mMsgCtrl)) {
        return;
    }

    MR::endDemo(this, "ゴミ掃除タイムアタック");
    MR::endNPCTalkCamera(false, -1);

    if (MR::isOnSwitchA(this)) {
        setNerve(&NrvCaretaker::CaretakerNrvAppearedStar::sInstance);
    } else {
        setNerve(&NrvCaretaker::CaretakerNrvWait::sInstance);
    }
}

void Caretaker::exeAppearedStar() {
    MR::tryTalkNearPlayerAndStartTalkAction(this);
    MR::tryStartReactionAndPushNerve(this, &NrvCaretaker::CaretakerNrvReaction::sInstance);
}
