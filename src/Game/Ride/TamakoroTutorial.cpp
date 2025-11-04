#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Ride/Tamakoro.hpp"
#include "Game/Ride/TamakoroTutorial.hpp"
#include "Game/Screen/PlayerActionGuidance.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SequenceUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/TalkUtil.hpp"

namespace NrvTamakoroTutorial {
    NEW_NERVE(HostTypeNrvFirst, TamakoroTutorial, First);
    NEW_NERVE(HostTypeNrvDenyTalk, TamakoroTutorial, DenyTalk);
    NEW_NERVE(HostTypeNrvFirstForceTalk, TamakoroTutorial, FirstForceTalk);
    NEW_NERVE(HostTypeNrvWaitRideTalk, TamakoroTutorial, WaitRideTalk);
    NEW_NERVE(HostTypeNrvWaitRide, TamakoroTutorial, WaitRide);
    NEW_NERVE(HostTypeNrvWaitRaiseTalk, TamakoroTutorial, WaitRaiseTalk);
    NEW_NERVE(HostTypeNrvWaitRaiseStable, TamakoroTutorial, WaitRaiseStable);
    NEW_NERVE(HostTypeNrvRaiseOk, TamakoroTutorial, RaiseOk);
    NEW_NERVE(HostTypeNrvRaiseOkTalk, TamakoroTutorial, RaiseOkTalk);
    NEW_NERVE(HostTypeNrvFinalTalk, TamakoroTutorial, FinalTalk);
    NEW_NERVE(HostTypeNrvWait, TamakoroTutorial, Wait);
    NEW_NERVE(HostTypeNrvDirectRide, TamakoroTutorial, Wait);
    NEW_NERVE(HostTypeNrvTrampleFront, TamakoroTutorial, Trample);
    NEW_NERVE(HostTypeNrvTrampleBack, TamakoroTutorial, Trample);
    NEW_NERVE(HostTypeNrvRecoverFront, TamakoroTutorial, Recover);
    NEW_NERVE(HostTypeNrvRecoverBack, TamakoroTutorial, Recover);
};

namespace {
    static TVec3f hRaiseAcc = TVec3f(0.0f, -1.0f, 0.0f);
    static const f32 hRaiseCheckDegree = 30.0f;
    static const f32 hDistToStartTutorial = 400.0f;
};

TamakoroTutorial::TamakoroTutorial(const char* pName) :
    LiveActor(pName),
    mHost(nullptr),
    mTalkCtrl(nullptr),
    mTalkCtrlAutomatic(nullptr),
    mPadAccel(0.0f, 0.0f, 0.0f),
    _A4(false),
    _A5(false)
{
    
}

void TamakoroTutorial::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("SignBoard", nullptr, false);
    MR::connectToSceneNpc(this);
    initHitSensor(2);
    MR::addHitSensorEnemy(this, "body", 16, 100.0f, TVec3f(0.0f, 100.0f, 0.0f));
    MR::addHitSensorCallbackMapObjSimple(this, "push", 16, 200.0f);
    initEffectKeeper(1, nullptr, false);
    MR::addEffectHitNormal(this, nullptr);
    initSound(4, false);
    MR::createSceneObj(SceneObj_PlayerActionGuidance);
    MR::getPlayerActionGuidance()->createTamakoroLayout();
    MR::initShadowFromCSV(this, "Shadow");
    appear();

    s32 omitTutorial = -1;
    MR::getJMapInfoArg0NoInit(rIter, &omitTutorial);

    if (omitTutorial == -1) {
        mTalkCtrl = MR::createTalkCtrlDirect(
            this, rIter, "Common_TamakoroTutorial000", TVec3f(0.0f, 0.0f, 0.0f), nullptr);

        initNerve(&NrvTamakoroTutorial::HostTypeNrvFirst::sInstance);

        _A5 = false;
    }
    else {
        mTalkCtrl = MR::createTalkCtrl(this, rIter, "SignBoard", TVec3f(0.0f, 0.0f, 0.0f), nullptr);

        initNerve(&NrvTamakoroTutorial::HostTypeNrvWait::sInstance);

        _A5 = true;

        getSensor("push")->invalidate();
        mHost->startRide();
    }

    mTalkCtrlAutomatic = MR::createTalkCtrlDirectOnRootNodeAutomatic(
        this, rIter, "Common_TamakoroTutorial007", TVec3f(0.0f, 0.0f, 0.0f), nullptr);

    MR::setMessageBalloonFollowOffset(mTalkCtrl, TVec3f(0.0f, 180.0f, 0.0f));
    MR::setDistanceToTalk(mTalkCtrl, hDistToStartTutorial);
    MR::setMessageBalloonFollowOffset(mTalkCtrlAutomatic, TVec3f(0.0f, 180.0f, 0.0f));
    MR::setDistanceToTalk(mTalkCtrlAutomatic, hDistToStartTutorial);
    MR::calcGravity(this);
}

bool TamakoroTutorial::isNeedTutorial() {
    return isNerve(&NrvTamakoroTutorial::HostTypeNrvWaitRide::sInstance);
}

void TamakoroTutorial::requestStart() {
    MR::forwardNode(mTalkCtrl);
    setNerve(&NrvTamakoroTutorial::HostTypeNrvWaitRaiseTalk::sInstance);
}

void TamakoroTutorial::requestEnd() {
    setNerve(&NrvTamakoroTutorial::HostTypeNrvDirectRide::sInstance);
}

void TamakoroTutorial::control() {
    MR::getCorePadAcceleration(&mPadAccel, WPAD_CHAN0);
    MR::normalizeOrZero(&mPadAccel);
}

void TamakoroTutorial::exeFirst() {
    if (MR::isFirstStep(this)) {
        getSensor("push")->validate();
        MR::startAction(this, "RevivalFront");
        MR::setBckFrameAndStop(this, MR::getBckFrameMax(this));
    }

    if (!MR::isOnGameEventFlagTamakoroTutorialAtFirst()) {
        f32 distPlayerToTutorial = MR::calcDistanceToPlayer(mPosition);
        f32 distPlayerToTamakoro = MR::calcDistanceToPlayer(mHost->mPosition);

        if (MR::isOnGroundPlayer()
            && !MR::isExecScenarioStarter()
            && (distPlayerToTutorial < hDistToStartTutorial
                || distPlayerToTamakoro < hDistToStartTutorial))
        {
            setNerve(&NrvTamakoroTutorial::HostTypeNrvFirstForceTalk::sInstance);
        }
    }
    else {
        if (MR::isFirstStep(this)) {
            getSensor("push")->invalidate();
            mHost->startRide();
        }

        if (MR::tryTalkNearPlayerAtEnd(mTalkCtrlAutomatic)
            && MR::tryTalkSelectLeft(mTalkCtrlAutomatic))
        {
            MR::resetNode(mTalkCtrl);
            MR::forwardNode(mTalkCtrl);
            setNerve(&NrvTamakoroTutorial::HostTypeNrvWaitRide::sInstance);
        }
    }
}

void TamakoroTutorial::exeDenyTalk() {
    if (MR::tryTalkForceAtEnd(mTalkCtrlAutomatic)) {
        MR::resetNode(mTalkCtrlAutomatic);
        setNerve(&NrvTamakoroTutorial::HostTypeNrvFirst::sInstance);
    }
}

void TamakoroTutorial::exeFirstForceTalk() {
    if (MR::isFirstStep(this)) {
        MR::resetNode(mTalkCtrl);
    }

    if (MR::tryTalkForceAtEnd(mTalkCtrl)) {
        setNerve(&NrvTamakoroTutorial::HostTypeNrvWaitRideTalk::sInstance);
    }
}

void TamakoroTutorial::exeWaitRideTalk() {
    if (MR::isFirstStep(this)) {
        MR::forwardNode(mTalkCtrl);
    }

    if (MR::tryTalkForceAtEnd(mTalkCtrl)) {
        getSensor("push")->invalidate();
        mHost->startRide();
        setNerve(&NrvTamakoroTutorial::HostTypeNrvWaitRide::sInstance);
    }
}

void TamakoroTutorial::exeWaitRide() {
    if (MR::isFirstStep(this)) {
        MR::forwardNode(mTalkCtrl);
    }

    MR::tryTalkNearPlayer(mTalkCtrl);
}

void TamakoroTutorial::exeWaitRaiseTalk() {
    PlayerActionGuidance* pPlayerActionGuidance = MR::getPlayerActionGuidance();

    pPlayerActionGuidance->mGuidanceState = 0;
    pPlayerActionGuidance->_36 = true;

    if (MR::isFirstStep(this)) {
        
    }

    MR::tryTalkForceWithoutDemo(mTalkCtrl);

    if (MR::isGreaterStep(this, 90)) {
        if (MR::isNearAngleDegree(::hRaiseAcc, mPadAccel, hRaiseCheckDegree)) {
            MR::startSystemSE("SE_SY_SURF_TUTORIAL_OK", -1, -1);
            setNerve(&NrvTamakoroTutorial::HostTypeNrvWaitRaiseStable::sInstance);
        }
    }
}

void TamakoroTutorial::exeWaitRaiseStable() {
    PlayerActionGuidance* pPlayerActionGuidance = MR::getPlayerActionGuidance();

    pPlayerActionGuidance->mGuidanceState = 1;
    pPlayerActionGuidance->_36 = true;

    MR::tryTalkForceWithoutDemoAtEnd(mTalkCtrl);

    if (!MR::isNearAngleDegree(::hRaiseAcc, mPadAccel, hRaiseCheckDegree)) {
        setNerve(&NrvTamakoroTutorial::HostTypeNrvWaitRaiseTalk::sInstance);
        MR::startSystemSE("SE_SY_SURF_TUTORIAL_NG", -1, -1);
    }
    else {
        startTimerSound(getNerveStep(), 1);

        if (MR::isGreaterStep(this, 60)) {
            MR::startSystemSE("SE_SY_SURF_TUTORIAL_GONEXT", -1, -1);
            setNerve(&NrvTamakoroTutorial::HostTypeNrvRaiseOk::sInstance);
        }
    }
}

void TamakoroTutorial::exeRaiseOk() {
    PlayerActionGuidance* pPlayerActionGuidance = MR::getPlayerActionGuidance();

    pPlayerActionGuidance->mGuidanceState = 2;
    pPlayerActionGuidance->_36 = true;

    if (MR::isGreaterStep(this, 60)) {
        setNerve(&NrvTamakoroTutorial::HostTypeNrvRaiseOkTalk::sInstance);
    }
}

void TamakoroTutorial::exeRaiseOkTalk() {
    PlayerActionGuidance* pPlayerActionGuidance = MR::getPlayerActionGuidance();

    pPlayerActionGuidance->mGuidanceState = 2;
    pPlayerActionGuidance->_36 = true;

    if (MR::isFirstStep(this)) {
        MR::forwardNode(mTalkCtrl);
    }

    if (MR::tryTalkForceWithoutDemoAtEnd(mTalkCtrl)) {
        setNerve(&NrvTamakoroTutorial::HostTypeNrvFinalTalk::sInstance);
    }
}

void TamakoroTutorial::exeFinalTalk() {
    PlayerActionGuidance* pPlayerActionGuidance = MR::getPlayerActionGuidance();

    pPlayerActionGuidance->mGuidanceState = 3;
    pPlayerActionGuidance->_36 = true;

    if (MR::isFirstStep(this)) {
        MR::forwardNode(mTalkCtrl);
    }

    if (MR::tryTalkForceWithoutDemoAtEnd(mTalkCtrl)) {
        if (MR::isExistNextNode(mTalkCtrl)) {
            setNerve(&NrvTamakoroTutorial::HostTypeNrvFinalTalk::sInstance);
        }
        else {
            mHost->requestTutorialEnd();
            MR::onGameEventFlagTamakoroTutorialAtFirst();
            setNerve(&NrvTamakoroTutorial::HostTypeNrvWait::sInstance);
        }
    }
}

void TamakoroTutorial::exeWait() {
    if (isNerve(&NrvTamakoroTutorial::HostTypeNrvDirectRide::sInstance)
        && MR::isFirstStep(this))
    {
        mHost->requestTutorialEnd();
        MR::onGameEventFlagTamakoroTutorialAtFirst();
        setNerve(&NrvTamakoroTutorial::HostTypeNrvWait::sInstance);
    }
    else if (_A5 && MR::tryTalkNearPlayerAtEnd(mTalkCtrl)) {
        MR::tryForwardNode(mTalkCtrl);
    }
}

void TamakoroTutorial::exeTrample() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_OJ_SIGN_BOARD_TRAMP_BALL", -1, -1);

        if (isNerve(&NrvTamakoroTutorial::HostTypeNrvTrampleFront::sInstance)) {
            MR::startAction(this, "DownFront");
        }
        else {
            MR::startAction(this, "DownBack");
        }
    }

    if (MR::isActionEnd(this) && MR::isGreaterStep(this, 60) && !_A4) {
        if (isNerve(&NrvTamakoroTutorial::HostTypeNrvTrampleFront::sInstance)) {
            setNerve(&NrvTamakoroTutorial::HostTypeNrvRecoverFront::sInstance);
        }
        else {
            setNerve(&NrvTamakoroTutorial::HostTypeNrvRecoverBack::sInstance);
        }
    }
    else {
        _A4 = false;
    }
}

void TamakoroTutorial::exeRecover() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_OJ_SIGN_BOARD_RECOVER", -1, -1);

        if (isNerve(&NrvTamakoroTutorial::HostTypeNrvRecoverFront::sInstance)) {
            MR::startAction(this, "RevivalFront");
        }
        else {
            MR::startAction(this, "RevivalBack");
        }
    }

    if (MR::isActionEnd(this)) {
        setNerve(&NrvTamakoroTutorial::HostTypeNrvWait::sInstance);
    }
}

void TamakoroTutorial::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (!isNerve(&NrvTamakoroTutorial::HostTypeNrvTrampleFront::sInstance)
        && !isNerve(&NrvTamakoroTutorial::HostTypeNrvTrampleBack::sInstance))
    {
        MR::sendMsgPush(pReceiver, pSender);
    }

    if (MR::isSensorRide(pReceiver)) {
        _A4 = true;
    }
}

bool TamakoroTutorial::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgPlayerTrample(msg)) {
        if (!MR::isSensorRide(pSender)) {
            MR::startSound(this, "SE_OJ_SIGN_BOARD_TRAMP_PLR", -1, -1);

            return true;
        }

        if (isNerve(&NrvTamakoroTutorial::HostTypeNrvWait::sInstance)
            || isNerve(&NrvTamakoroTutorial::HostTypeNrvRecoverFront::sInstance)
            || isNerve(&NrvTamakoroTutorial::HostTypeNrvRecoverBack::sInstance))
        {
            TVec3f v1 = pSender->mPosition - pReceiver->mPosition;
            TVec3f v2;

            MR::calcFrontVec(&v2, this);

            if (0.0f < v2.dot(v1)) {
                setNerve(&NrvTamakoroTutorial::HostTypeNrvTrampleBack::sInstance);
            }
            else {
                setNerve(&NrvTamakoroTutorial::HostTypeNrvTrampleFront::sInstance);
            }

            MR::emitEffectHitBetweenSensors(this, pSender, pReceiver, 0.0f, nullptr);

            return true;
        }
    }

    if (MR::isMsgPlayerHipDrop(msg)) {
        MR::sendMsgAwayJump(pSender, pReceiver);

        return true;
    }

    return false;
}

// TamakoroTutorial::updateHitSensor

void TamakoroTutorial::startTimerSound(s32 step, s32 param2) {
    if (step == 20) {
        MR::startSystemSE("SE_SY_SURF_TUTORIAL_TIMER", param2, -1);
    }

    if (step == 40) {
        MR::startSystemSE("SE_SY_SURF_TUTORIAL_TIMER2", param2, -1);
    }
}
