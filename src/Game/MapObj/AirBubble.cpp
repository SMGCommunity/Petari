#include "Game/MapObj/AirBubble.hpp"

AirBubble::AirBubble(const char *pName) : LiveActor(pName) {
    _A4.x = 0.0f;
    _A4.y = 0.0f;
    _A4.z = 1.0f;
    _B0 = 0.0f;
    _C4 = 360;
}

void AirBubble::init(const JMapInfoIter &rIter) {
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

void AirBubble::appearMove(const TVec3f &a1, s32 a2) {
    _8C.set(a1);
    mPosition.set(a1);
    appear();
    MR::showModel(this);
    setNerve(&NrvAirBubble::AirBubbleNrvMove::sInstance);
    MR::invalidateClipping(this);
    _C4 = a2 > 0 ? a2 : 360;
}

void AirBubble::exeBreak() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_OJ_AIR_BUBBLE_BREAK", -1, -1);
        MR::emitEffect(this, "RecoveryBubbleBreak");
        MR::incPlayerOxygen(8);
        MR::tryRumblePadMiddle(this, 0);
        kill();
    }
}

bool AirBubble::receiveMsgPush(HitSensor *a2, HitSensor *a3) {
    if (MR::isSensorPlayer(a2)) {
        if (isNerve(&NrvAirBubble::AirBubbleNrvKillWait::sInstance)) {
            MR::incPlayerOxygen(8);
            kill();
            return true;
        }
        else {
            setNerve(&NrvAirBubble::AirBubbleNrvBreak::sInstance);
        }

        return true;
    }

    return false;
}

bool AirBubble::receiveOtherMsg(u32 msg, HitSensor *a2, HitSensor *a3) {
    bool res;

    if (MR::isMsgItemShow(msg)) {
        return MR::receiveItemShowMsg(msg, a2, a3);
    }

    if (MR::isMsgItemHide(msg)) {
        return MR::receiveItemHideMsg(msg, a2, a3);
    }

    if (MR::isMsgItemStartMove(msg)) {
        _C9 = 1;
        return true;
    }
    else if (MR::isMsgItemEndMove(msg)) {
        _C9 = 0;
        _8C = mPosition;
        return true;
    }
    else if (MR::isMsgSpinStormRange(msg) && canSpinGet()) {
        TVec3f stack_8(a2->mPosition);
        stack_8 -= mPosition;

        if (PSVECMag(stack_8.toCVec()) < 250.0f) {
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

namespace NrvAirBubble {
    INIT_NERVE(AirBubbleNrvWait);
    INIT_NERVE(AirBubbleNrvMove);
    INIT_NERVE(AirBubbleNrvBreak);
    INIT_NERVE(AirBubbleNrvKillWait);

    void AirBubbleNrvKillWait::execute(Spine *pSpine) const {
        AirBubble* bubble = reinterpret_cast<AirBubble*>(pSpine->mExecutor);
        
        if (MR::isGreaterStep(bubble, 90)) {
            bubble->kill();
        }
    }

    void AirBubbleNrvBreak::execute(Spine *pSpine) const {
        AirBubble* bubble = reinterpret_cast<AirBubble*>(pSpine->mExecutor);
        bubble->exeBreak();
    }

    void AirBubbleNrvMove::execute(Spine *pSpine) const {
        AirBubble* bubble = reinterpret_cast<AirBubble*>(pSpine->mExecutor);
        bubble->exeMove();
    }

    void AirBubbleNrvWait::execute(Spine *pSpine) const {
        AirBubble* bubble = reinterpret_cast<AirBubble*>(pSpine->mExecutor);
        bubble->exeWait();
    }
};