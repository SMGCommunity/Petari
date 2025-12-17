#include "Game/NPC/RabbitStateWaitStart.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "Game/Util/TalkUtil.hpp"

namespace {
    static const f32 sGravityAccel = 0.1f;
    static const s32 sSmallTurnTime = 30;
    static const s32 sSmallTurnStopTime = 30;
    static const f32 sSmallTurnDegree = 6.0f;
    static const f32 sSmallTurnStartDegree = 45.0f;
    static const f32 sSpinDegree = 2.0f;
    static const f32 sInStartTalkRange = 300.0f;
};  // namespace

namespace NrvRabbitStateWaitStart {
    NEW_NERVE(RabbitStateWaitStartNrvWait, RabbitStateWaitStart, Wait);
    NEW_NERVE(RabbitStateWaitStartNrvSmallTurn, RabbitStateWaitStart, SmallTurn);
    NEW_NERVE(RabbitStateWaitStartNrvHitReaction, RabbitStateWaitStart, HitReaction);
    NEW_NERVE(RabbitStateWaitStartNrvTrample, RabbitStateWaitStart, Trample);
    NEW_NERVE(RabbitStateWaitStartNrvSpin, RabbitStateWaitStart, Spin);
    NEW_NERVE(RabbitStateWaitStartNrvPointing, RabbitStateWaitStart, Pointing);
    NEW_NERVE(RabbitStateWaitStartNrvTalk, RabbitStateWaitStart, Talk);
};  // namespace NrvRabbitStateWaitStart

RabbitStateWaitStart::RabbitStateWaitStart(LiveActor* pHost, TVec3f* pHostRotateFront, TalkMessageCtrl* pTalkMessageCtrl)
    : ActorStateBaseInterface("うさぎ会話待ち状態"), mHost(pHost), mTalkActionName("Wait"), mHostRotateFront(pHostRotateFront),
      mTalkMessageCtrl(pTalkMessageCtrl), _1C(false), _1D(true) {}

void RabbitStateWaitStart::setTalkActionName(const char* pName) {
    mTalkActionName = pName;
}

void RabbitStateWaitStart::init() {
    initNerve(&NrvRabbitStateWaitStart::RabbitStateWaitStartNrvWait::sInstance);
}

void RabbitStateWaitStart::appear() {
    mIsDead = false;

    setNerve(&NrvRabbitStateWaitStart::RabbitStateWaitStartNrvWait::sInstance);
}

void RabbitStateWaitStart::control() {
    tryPointing();
}

bool RabbitStateWaitStart::receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgStarPieceAttack(msg) || MR::isMsgPlayerHitAll(msg)) {
        return requestHitReaction();
    }

    if (MR::isMsgPlayerHipDrop(msg)) {
        MR::sendMsgAwayJump(pSender, pReceiver);

        return requestTrample();
    }

    if (MR::isMsgPlayerTrample(msg)) {
        return requestTrample();
    }

    return false;
}

bool RabbitStateWaitStart::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isMsgSpinStormRange(msg)) {
        return requestSpin();
    }

    return false;
}

bool RabbitStateWaitStart::requestTrample() {
    if (isEnableReaction()) {
        setNerve(&NrvRabbitStateWaitStart::RabbitStateWaitStartNrvTrample::sInstance);

        return true;
    }

    return false;
}

bool RabbitStateWaitStart::requestSpin() {
    if (isEnableReaction()) {
        setNerve(&NrvRabbitStateWaitStart::RabbitStateWaitStartNrvSpin::sInstance);

        return true;
    }

    return false;
}

bool RabbitStateWaitStart::requestHitReaction() {
    if (isEnableReaction()) {
        setNerve(&NrvRabbitStateWaitStart::RabbitStateWaitStartNrvHitReaction::sInstance);

        return true;
    }

    return false;
}

bool RabbitStateWaitStart::isEnableReaction() const {
    if (isNerve(&NrvRabbitStateWaitStart::RabbitStateWaitStartNrvWait::sInstance) ||
        isNerve(&NrvRabbitStateWaitStart::RabbitStateWaitStartNrvSmallTurn::sInstance) ||
        isNerve(&NrvRabbitStateWaitStart::RabbitStateWaitStartNrvTrample::sInstance) ||
        isNerve(&NrvRabbitStateWaitStart::RabbitStateWaitStartNrvTalk::sInstance) && MR::isShortTalk(mTalkMessageCtrl)) {
        return true;
    }

    return false;
}

bool RabbitStateWaitStart::trySmallTurn() {
    if (MR::isGreaterStep(this, sSmallTurnTime) && !MR::isFaceToPlayerHorizontalDegree(mHost, *mHostRotateFront, sSmallTurnStartDegree)) {
        setNerve(&NrvRabbitStateWaitStart::RabbitStateWaitStartNrvSmallTurn::sInstance);

        return true;
    }

    return false;
}

bool RabbitStateWaitStart::tryTalk() {
    if (MR::isNearPlayer(mHost, sInStartTalkRange) && MR::tryTalkNearPlayer(mTalkMessageCtrl)) {
        setNerve(&NrvRabbitStateWaitStart::RabbitStateWaitStartNrvTalk::sInstance);

        return true;
    }

    return false;
}

bool RabbitStateWaitStart::tryPointing() {
    if (!MR::isExistStarPointerTarget(mHost)) {
        return false;
    }

    if (!isEnableReaction()) {
        return false;
    }

    if (MR::isStarPointerPointing2POnPressButton(mHost, "弱", true, false)) {
        setNerve(&NrvRabbitStateWaitStart::RabbitStateWaitStartNrvPointing::sInstance);

        return true;
    }

    return false;
}

void RabbitStateWaitStart::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "Wait");
        MR::validateClipping(mHost);
        MR::zeroVelocity(mHost);
    }

    if (_1D) {
        if (MR::isExistBinder(mHost) && !MR::isBindedGround(mHost)) {
            MR::addVelocityToGravity(mHost, sGravityAccel);
        } else {
            MR::zeroVelocity(mHost);
        }
    }

    if (tryTalk()) {
        return;
    }

    if (trySmallTurn()) {
        return;
    }
}

void RabbitStateWaitStart::exeSmallTurn() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "TurnSmall");
    }

    MR::turnDirectionToPlayerDegree(mHost, mHostRotateFront, sSmallTurnDegree);
    MR::zeroVelocity(mHost);

    if (!tryTalk() && MR::isGreaterStep(this, sSmallTurnStopTime)) {
        setNerve(&NrvRabbitStateWaitStart::RabbitStateWaitStartNrvWait::sInstance);
    }
}

void RabbitStateWaitStart::exeHitReaction() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "Reaction");
        MR::limitedStarPieceHitSound();
        MR::startSound(mHost, "SE_SM_RABBIT_STAR_PIECE_HIT", -1, -1);
        MR::startSound(mHost, "SE_SV_RABBIT_STAR_PIECE_HIT", -1, -1);
    }

    MR::zeroVelocity(mHost);

    if (MR::isActionEnd(mHost)) {
        setNerve(&NrvRabbitStateWaitStart::RabbitStateWaitStartNrvWait::sInstance);
    }
}

void RabbitStateWaitStart::exeTrample() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "Press");
        MR::startSound(mHost, "SE_SM_NPC_TRAMPLED", -1, -1);
        MR::startSound(mHost, "SE_SV_RABBIT_TRAMPLED", -1, -1);
    }

    MR::zeroVelocity(mHost);

    if (MR::isActionEnd(mHost)) {
        setNerve(&NrvRabbitStateWaitStart::RabbitStateWaitStartNrvWait::sInstance);
    }
}

void RabbitStateWaitStart::exeSpin() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "Spin");
        MR::startSound(mHost, "SE_SV_RABBIT_SPIN", -1, -1);
        MR::startSound(mHost, "SE_SM_RABBIT_SPIN", -1, -1);
    }

    MR::turnDirectionToPlayerDegree(mHost, mHostRotateFront, sSpinDegree);
    MR::zeroVelocity(mHost);

    if (MR::isActionEnd(mHost)) {
        setNerve(&NrvRabbitStateWaitStart::RabbitStateWaitStartNrvWait::sInstance);
    }
}

void RabbitStateWaitStart::exePointing() {
    if (MR::isFirstStep(this)) {
        MR::startAction(mHost, "Pointing");
        MR::startDPDHitSound();
    }

    MR::startLevelSound(mHost, "SE_SM_LV_RABBIT_POINT", -1, -1, -1);
    MR::zeroVelocity(mHost);

    if ((MR::isShortTalk(mTalkMessageCtrl) || !tryTalk()) && MR::isActionEnd(mHost)) {
        setNerve(&NrvRabbitStateWaitStart::RabbitStateWaitStartNrvWait::sInstance);
    }
}

void RabbitStateWaitStart::exeTalk() {
    if (MR::isFirstStep(this)) {
        MR::invalidateClipping(mHost);
        MR::startAction(mHost, mTalkActionName);
        MR::zeroVelocity(mHost);
    }

    MR::turnDirectionToPlayerDegreeHorizon(mHost, mHostRotateFront, 30.0f);

    if (MR::tryTalkNearPlayerAtEnd(mTalkMessageCtrl)) {
        if (_1C) {
            kill();
        } else {
            setNerve(&NrvRabbitStateWaitStart::RabbitStateWaitStartNrvWait::sInstance);
        }
    }
}
