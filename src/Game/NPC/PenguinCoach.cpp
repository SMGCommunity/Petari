#include "Game/NPC/PenguinCoach.hpp"
#include "Game/LiveActor/LiveActorGroup.hpp"
#include "Game/Map/RaceManager.hpp"
#include "Game/NPC/NPCActor.hpp"
#include "Game/NPC/Rosetta.hpp"
#include "Game/NPC/TurnJointCtrl.hpp"
#include "Game/Ride/SurfRayTutorial.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/NPCUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/TalkUtil.hpp"

namespace {
    static const f32 sTurnSpeed = 180.0f;
}

namespace NrvPenguinCoach {
    NEW_NERVE(PenguinCoachNrvTakeOutStar, PenguinCoach, TakeOutStar);
    NEW_NERVE(PenguinCoachNrvRace, PenguinCoach, Race);
    NEW_NERVE(PenguinCoachNrvReaction, PenguinCoach, Reaction);
    NEW_NERVE(PenguinCoachNrvPrep, PenguinCoach, Prep);
    NEW_NERVE(PenguinCoachNrvWait, PenguinCoach, Wait);
    NEW_NERVE(PenguinCoachNrvFade, PenguinCoach, Fade);
    NEW_NERVE(PenguinCoachNrvTutorial, PenguinCoach, Tutorial);
    NEW_NERVE(PenguinCoachNrvPraise, PenguinCoach, Praise);
}  // namespace NrvPenguinCoach

PenguinCoach::PenguinCoach(const char* pName) : NPCActor(pName), mIsRaceComplete(false), mTakeOutStar(nullptr), mBehavior(-1), mCameraInfo(nullptr) {
}

void PenguinCoach::init(const JMapInfoIter& rIter) {
    NPCActorCaps caps("PenguinCoach");
    caps.setDefault();
    caps.mWaitNerve = &NrvPenguinCoach::PenguinCoachNrvWait::sInstance;
    caps.mBinder = false;
    caps.mShadowSize = 130.0f;
    caps.mSensor = false;
    caps.mMakeActor = false;
    caps.mMessageOffset.setPS2(TVec3f(0.0f, 430.0f, 0.0f));
    caps.mPointerSize = 180.0f;
    caps.mStarPointerOffs.set(0.0f, 200.0f, 80.0f);

    MR::getJMapInfoArg0NoInit(rIter, &mBehavior);

    initialize(rIter, caps);

    if (mBehavior == Behavior_Star) {
        MR::useStageSwitchWriteA(this, rIter);
        MR::declarePowerStar(this);
        mTakeOutStar = new TakeOutStar(this, "TakeOutStar", "TakeOutStarPenguinCoach", &NrvPenguinCoach::PenguinCoachNrvTakeOutStar::sInstance);
    } else if (mBehavior == Behavior_Tutorial || mBehavior == Behavior_Race) {
        MR::declarePowerStar(this);
        mTakeOutStar = new TakeOutStar(this, "TakeOutStar", "TakeOutStarPenguinCoach", &NrvPenguinCoach::PenguinCoachNrvTakeOutStar::sInstance);
        RaceManagerFunction::entryAudience(this);
    }

    initHitSensor(2);
    MR::addHitSensorPush(this, "body", 8, 160.0f, TVec3f(0.0f, 150.0f, 0.0f));
    MR::addHitSensorPush(this, "head", 8, 150.0f, TVec3f(0.0f, 300.0f, 40.0f));

    if (mBehavior == Behavior_Tutorial || mBehavior == Behavior_Race) {
        if (mBehavior == Behavior_Tutorial) {
            MR::initMultiActorCamera(this, rIter, &mCameraInfo, "チュートリアル");  // tutorial
        }
        MR::initMultiActorCamera(this, rIter, &mCameraInfo, "会話");  // conversation

        MR::joinToGroupArray(this, rIter, nullptr, 0x20);
        MR::invalidateClipping(this);

        if (mBehavior == Behavior_Tutorial) {
            MR::offReadNodeAutomatic(mMsgCtrl);
        }
        setNerve(&NrvPenguinCoach::PenguinCoachNrvPrep::sInstance);
    }

    mTutorial = new SurfRayTutorial(this, MR::createTalkCtrlDirect(this, rIter, "Common_SurfingTutorial000", caps.mMessageOffset, nullptr), rIter);
    mTurnJointCtrl = new TurnJointCtrl(this);
    mTurnJointCtrl->init(90.0f, 10.0f, 40.0f);
    mTurnJointCtrl->addFace("Head", 1.0f, TurnJointCtrl::X, TurnJointCtrl::Y, TurnJointCtrl::Z);
    mTurnJointCtrl->_60 = 0.1f;
    mTurnJointCtrl->_5C = 800.0f;
    makeActorAppeared();

    mParam.setMoveAction("Wait", "Turn");
    mParam.setTalkAction("Talk", "TalkTurn");
    mParam._8 = 1.0f;
    _134 = "Trampled";
    _13C = "Reaction";
    _130 = "Reaction";
    _138 = "Pointing";

    if (mMsgCtrl != nullptr) {
        MR::registerBranchFunc(getMsgCtrl(), TalkMessageFunc(this, &PenguinCoach::branchFunc));
        MR::registerEventFunc(getMsgCtrl(), TalkMessageFunc(this, &PenguinCoach::eventFunc));
        MR::setDistanceToTalk(mMsgCtrl, 350.0f);
    }
}

void PenguinCoach::initAfterPlacement() {
    if (mBehavior == Behavior_Tutorial) {
        LiveActorGroup* group = MR::getGroupFromArray(this);
        mActor = group->getActor(0) == this ? group->getActor(1) : group->getActor(0);
    }
}

void PenguinCoach::calcAndSetBaseMtx() {
    mTurnJointCtrl->setCallBackFunction();
    NPCActor::calcAndSetBaseMtx();
}

bool PenguinCoach::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgPlayerSpinAttack(msg)) {
        _E3 = true;
        return true;
    }

    return NPCActor::receiveMsgPlayerAttack(msg, pSender, pReceiver);
}

void PenguinCoach::control() {
    TVec3f starePos;
    MR::calcPlayerFaceStarePos(&starePos, MR::getJointMtx(this, "Head"), getBaseMtx());
    mTurnJointCtrl->setStarePos(starePos);
    mTurnJointCtrl->update();
    NPCActor::control();
}

bool PenguinCoach::branchFunc(u32 state) {
    if (state == 0) {
        return MR::isPlayerCarryAny();
    } else if (state == 1) {
        return MR::isPlayerCarryAny() && MR::isPlayerTakingActor("ゴールデン甲羅");
    } else if (state == 2) {
        return MR::isOnGameEventFlagSurfingTutorialAtFirst();
    } else if (state == 3) {
        return mIsRaceComplete;
    } else if (state == 4) {
        return MR::getRaceCurrentTime() < 90 * 60;
    } else if (state == 5) {
        return MR::getRaceCurrentTime() < 180 * 60;
    }

    return false;
}

void PenguinCoach::setTutorialPos() {
    MR::setNPCActorPos(this, "コーチチュートリアル位置");
}

void PenguinCoach::setPlayerNoSelectionPos() {
    MR::setPlayerPos("いいえ選択移動位置");
}

void PenguinCoach::setFinishPos() {
    MR::setNPCActorPos(this, "コーチレース終了後位置");
}

bool PenguinCoach::eventFunc(u32 state) {
    if (state == 0) {
        MR::onSwitchA(this);
        return true;
    } else if (state == 1) {
        if (mTakeOutStar->isFirstStep()) {
            mTurnJointCtrl->endCtrl(60);
        }
        mTakeOutStar->takeOut();
        if (mTakeOutStar->isLastStep()) {
            mTurnJointCtrl->startCtrl(60);
            return true;
        }
        return false;
    } else if (state == 2) {
        MR::forceKillPlayerByGroundRace();
        return true;
    }

    return true;
}
void PenguinCoach::resetAudience() {
    mIsRaceComplete = true;
    MR::onReadNodeAutomatic(mMsgCtrl);
    setNerve(&NrvPenguinCoach::PenguinCoachNrvPraise::sInstance);
    MR::requestMovementOn(this);
    if (mBehavior == Behavior_Tutorial) {
        setFinishPos();
    }

    turnToPlayer(sTurnSpeed);
}

void PenguinCoach::prepAudience() {
}

void PenguinCoach::exeTakeOutStar() {
}

void PenguinCoach::exeRace() {
}

void PenguinCoach::exePrep() {
    if (mBehavior == Behavior_Race) {
        if (MR::isTalkStart(mMsgCtrl)) {
            MR::startMultiActorCameraTargetSelf(this, mCameraInfo, "会話", -1);
        }

        if (MR::isTalkEnd(mMsgCtrl)) {
        }
    }

    if (!MR::tryStartReactionAndPushNerve(this, &NrvPenguinCoach::PenguinCoachNrvReaction::sInstance) &&
        MR::tryTalkNearPlayerAtEndAndStartTalkAction(this)) {
        if (mBehavior == Behavior_Tutorial || (mBehavior == Behavior_Race && MR::tryTalkSelectLeft(mMsgCtrl))) {
            MR::startTalkingSequenceWithoutCinemaFrame(this);
            MR::closeWipeFade(-1);
            setNerve(&NrvPenguinCoach::PenguinCoachNrvFade::sInstance);
        } else if (mBehavior == Behavior_Race && MR::tryTalkSelectRight(mMsgCtrl)) {
            MR::endMultiActorCamera(this, mCameraInfo, "会話", false, -1);
        }
    }
}

void PenguinCoach::exeWait() {
    if (!MR::tryStartReactionAndPushNerve(this, &NrvPenguinCoach::PenguinCoachNrvReaction::sInstance)) {
        MR::tryTalkNearPlayerAndStartTalkAction(this);
    }
}

void PenguinCoach::exeFade() {
    if (MR::isWipeActive()) {
        return;
    }

    MR::openWipeFade(-1);
    MR::endTalkingSequence(this);

    if (mBehavior == Behavior_Tutorial) {
        if (MR::isOnGameEventFlagSurfingTutorialAtFirst()) {
            if (MR::tryTalkSelectLeft(mMsgCtrl)) {
                MR::startMultiActorCameraTargetSelf(this, mCameraInfo, "チュートリアル", 0);
                setTutorialPos();
                setNerve(&NrvPenguinCoach::PenguinCoachNrvTutorial::sInstance);
            } else {
                setFinishPos();
                mTutorial->omitTutorial();
                setNerve(&NrvPenguinCoach::PenguinCoachNrvRace::sInstance);
            }
        } else {
            if (MR::tryTalkSelectLeft(mMsgCtrl)) {
                MR::startMultiActorCameraTargetSelf(this, mCameraInfo, "チュートリアル", 0);
                setTutorialPos();
                setNerve(&NrvPenguinCoach::PenguinCoachNrvTutorial::sInstance);
            } else {
                MR::clearTalkState(mMsgCtrl);
                setPlayerNoSelectionPos();
                setNerve(&NrvPenguinCoach::PenguinCoachNrvPrep::sInstance);
            }
        }
    } else {
        MR::setPlayerPos("スタート位置（サーフィン）");
        MR::endMultiActorCamera(this, mCameraInfo, "会話", false, 0);
        setNerve(&NrvPenguinCoach::PenguinCoachNrvRace::sInstance);
    }
}

void PenguinCoach::exeReaction() {
    if (MR::isFirstStep(this)) {
        mTurnJointCtrl->endCtrl(4);
    }

    if (_D8) {
        MR::startSound(this, "SE_SM_PENGUIN_TRAMPLED", -1, -1);
        MR::startSound(this, "SE_SV_PENGUIN_L_TRAMPLED", -1, -1);
    }

    if (isPointingSe()) {
        MR::startSound(this, "SE_SV_PENGUIN_L_POINTED_2P", -1, -1);
        MR::startDPDHitSound();
    }

    if (_D9) {
        MR::startSound(this, "SE_SV_PENGUIN_L_SPIN_HIT", -1, -1);
        MR::startSound(this, "SE_SM_PENGUIN_L_SPIN_HIT", -1, -1);
    }

    if (_DB) {
        MR::startSound(this, "SE_SV_PENGUIN_L_STAR_PIECE", -1, -1);
        MR::startSound(this, "SE_SM_PENGUIN_L_SPIN_HIT", -1, -1);
    }

    if (MR::tryStartReactionAndPopNerve(this)) {
        mTurnJointCtrl->startCtrl(60);
    }
}

void PenguinCoach::exeTutorial() {
    if (mTutorial->update()) {
        setNerve(&NrvPenguinCoach::PenguinCoachNrvRace::sInstance);
        MR::endMultiActorCamera(this, mCameraInfo, "チュートリアル", true, 120);
        MR::onGameEventFlagSurfingTutorialAtFirst();
    }
}

void PenguinCoach::exePraise() {
    if (MR::isFirstStep(this)) {
        MR::startMultiActorCameraTargetSelf(this, mCameraInfo, "会話", 0);
    }

    if (MR::isWipeOpen() && MR::tryTalkForceAtEndAndStartTalkAction(this)) {
        MR::endMultiActorCamera(this, mCameraInfo, "会話", true, -1);
        setNerve(&NrvPenguinCoach::PenguinCoachNrvWait::sInstance);
    }
}
