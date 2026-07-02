#include "Game/MapObj/AirBubble.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/RailUtil.hpp"
#include "Game/Util/SoundUtil.hpp"

namespace NrvAirBubble {
    NEW_NERVE(AirBubbleNrvWait, AirBubble, Wait);
    NEW_NERVE(AirBubbleNrvMove, AirBubble, Move);
    NEW_NERVE(AirBubbleNrvBreak, AirBubble, Break);
    NEW_NERVE(AirBubbleNrvKillWait, AirBubble, KillWait);
};  // namespace NrvAirBubble

AirBubble::AirBubble(const char* pName) : LiveActor(pName) {
    _A4.x = 0.0f;
    _A4.y = 0.0f;
    _A4.z = 1.0f;
    _B0 = 0.0f;
    _C4 = 360;
}

void AirBubble::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::useStageSwitchWriteDead(this, rIter);
    initModelManagerWithAnm("AirBubble", nullptr, false);
    MR::connectToSceneItem(this);
    initHitSensor(1);
    MR::addHitSensorMapObj(this, "body", 8, 130.0f * mScale.x, TVec3f(0.0f, 0.0f, 0.0f));
    initEffectKeeper(0, nullptr, false);
    initSound(2, false);
    initNerve(&NrvAirBubble::AirBubbleNrvWait::sInstance);
    _C8 = 0;
    makeActorAppeared();
    MR::setClippingFar100m(this);
    MR::startBck(this, "Move", nullptr);
    _C9 = 0;
}

void AirBubble::initAfterPlacement() {
    if (_C8) {
        MR::moveCoordAndTransToNearestRailPos(this);
    }

    _8C = mPosition;
}

void AirBubble::appear() {
    LiveActor::appear();
}

void AirBubble::kill() {
    if (MR::isValidSwitchDead(this)) {
        MR::onSwitchDead(this);
    }

    LiveActor::kill();
}

void AirBubble::control() {
}

void AirBubble::appearMove(const TVec3f& a1, s32 a2) {
    _8C.set< f32 >(a1);
    mPosition.set< f32 >(a1);
    appear();
    MR::showModel(this);
    setNerve(&NrvAirBubble::AirBubbleNrvMove::sInstance);
    MR::invalidateClipping(this);
    _C4 = a2 > 0 ? a2 : 360;
}

void AirBubble::exeBreak() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_OJ_AIR_BUBBLE_BREAK");
        MR::emitEffect(this, "RecoveryBubbleBreak");
        MR::incPlayerOxygen(8);
        MR::tryRumblePadMiddle(this, 0);
        kill();
    }
}

void AirBubble::exeKillWait() {
    if (MR::isGreaterStep(this, 90)) {
        kill();
    }
}

bool AirBubble::receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver)) {
        if (isNerve(&NrvAirBubble::AirBubbleNrvKillWait::sInstance)) {
            MR::incPlayerOxygen(8);
            kill();
            return true;
        } else {
            setNerve(&NrvAirBubble::AirBubbleNrvBreak::sInstance);
        }

        return true;
    }

    return false;
}

bool AirBubble::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    bool res;

    if (MR::isMsgItemShow(msg)) {
        return MR::receiveItemShowMsg(msg, pSender, pReceiver);
    }

    if (MR::isMsgItemHide(msg)) {
        return MR::receiveItemHideMsg(msg, pSender, pReceiver);
    }

    if (MR::isMsgItemStartMove(msg)) {
        _C9 = 1;
        return true;
    } else if (MR::isMsgItemEndMove(msg)) {
        _C9 = 0;
        _8C = mPosition;
        return true;
    } else if (MR::isMsgSpinStormRange(msg) && canSpinGet()) {
        if ((pSender->mPosition - mPosition).length() < 250.0f) {
            setNerve(&NrvAirBubble::AirBubbleNrvBreak::sInstance);
            return true;
        }
    }

    return false;
}

bool AirBubble::canSpinGet() const {
    if (isNerve(&NrvAirBubble::AirBubbleNrvWait::sInstance) || isNerve(&NrvAirBubble::AirBubbleNrvMove::sInstance)) {
        return true;
    }

    return false;
}

AirBubble::~AirBubble() {
}
