#include "Game/MapObj/PrizeRing.hpp"

namespace NrvPrizeRing {
    NEW_NERVE(PrizeRingStart, PrizeRing, Start);
    NEW_NERVE(PrizeRingLoop, PrizeRing, Loop);
    NEW_NERVE(PrizeRingTimeout, PrizeRing, Timeout);
    NEW_NERVE(PrizeRingReadyToPass, PrizeRing, ReadyToPass);
    NEW_NERVE(PrizeRingPass, PrizeRing, Pass);
    NEW_NERVE(PrizeRingReadyToKill, PrizeRing, ReadyToKill);
};  // namespace NrvPrizeRing

PrizeRing::PrizeRing() : ModelObj("PrizeRing", "PrizeRing", nullptr, -2, -2, -2, false) {
    _90 = 800;
}

PrizeRing::~PrizeRing() {}

void PrizeRing::init(const JMapInfoIter& rIter) {
    initNerve(&NrvPrizeRing::PrizeRingStart::sInstance);
    initHitSensor(1);
    TVec3f vec;
    vec.x = 0.0f;
    vec.y = 0.0f;
    vec.z = 0.0f;
    MR::addHitSensorMapObjSimple(this, "body", 4, 300.0f, vec);
    makeActorDead();
}

void PrizeRing::appear() {
    this->makeActorAppeared();
    MR::showModel(this);
    MR::invalidateClipping(this);
    setNerve(&NrvPrizeRing::PrizeRingStart::sInstance);
}

void PrizeRing::kill() {
    makeActorDead();
}

void PrizeRing::setLife(int num) {
    _90 = num;
}

void PrizeRing::setNumber(int num) {
    f32 pNum = num - 1.0f;
    MR::startBva(this, "Number");
    MR::setBvaFrameAndStop(this, pNum);
}

void PrizeRing::setNervePass() {
    setNerve(&NrvPrizeRing::PrizeRingPass::sInstance);
}

bool PrizeRing::isReadyToPass() const {
    if (MR::isDead(this)) {
        return false;
    } else {
        return isNerve(&NrvPrizeRing::PrizeRingReadyToPass::sInstance);
    }
}

bool PrizeRing::isReadyToKill() const {
    if (MR::isDead(this)) {
        return false;
    } else {
        return isNerve(&NrvPrizeRing::PrizeRingReadyToKill::sInstance);
    }
}

void PrizeRing::exeStart() {
    if (MR::isFirstStep(this)) {
        MR::startSound(this, "SE_OJ_PRIZE_RING_APPEAR", -1, -1);
        MR::startBck(this, "Start", nullptr);
        MR::startBrk(this, "Loop");
        MR::setBrkFrameAndStop(this, 0.0f);
    }

    if (MR::isBckStopped(this)) {
        setNerve(&NrvPrizeRing::PrizeRingLoop::sInstance);
    }
}

void PrizeRing::exeLoop() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Loop", nullptr);
        MR::startBrk(this, "Loop");
    }

    if (MR::isStep(this, _90)) {
        setNerve(&NrvPrizeRing::PrizeRingTimeout::sInstance);
    }
}

void PrizeRing::exeTimeout() {
    if (MR::isFirstStep(this)) {
        MR::startBrk(this, "Timeout");
    }

    if (PrizeRing::isOnTriggerTimeoutFlash()) {
        MR::startSound(this, "SE_OJ_PRIZE_RING_BLINK", -1, -1);
    }

    if (MR::isBrkStopped(this)) {
        MR::startSound(this, "SE_OJ_PRIZE_RING_DISAPPEAR", -1, -1);
        this->kill();
    }
}

void PrizeRing::exeReadyToPass() {}

void PrizeRing::exePass() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "End", nullptr);
        MR::startBrk(this, "End");
        MR::tryRumblePadMiddle(this, 0);
        PrizeRing::playSound();
    }

    if (MR::isBckStopped(this)) {
        setNerve(&NrvPrizeRing::PrizeRingReadyToKill::sInstance);
    }
}

void PrizeRing::exeReadyToKill() {
    if (MR::isFirstStep(this)) {
        MR::hideModel(this);
    }
}
/*
bool PrizeRing::isPassed() const {
    if (!PrizeRing::isReadyToPass()) {
        if (!isNerve(&NrvPrizeRing::PrizeRingPass::sInstance)) {
            return PrizeRing::isReadyToKill();
        }
    }
}
*/

void PrizeRing::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver)) {
        if (!PrizeRing::isPassed()) {
            setNerve(&NrvPrizeRing::PrizeRingReadyToPass::sInstance);
        }
    }
}
