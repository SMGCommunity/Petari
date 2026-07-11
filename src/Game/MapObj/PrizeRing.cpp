#include "Game/MapObj/PrizeRing.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "revolution/types.h"
#include <cstddef>

namespace {
    static char* const sPlaySoundNames[] = {nullptr, "SE_SY_GET_PRIZE_RING_5", "SE_SY_GET_PRIZE_RING_4", "SE_SY_GET_PRIZE_RING_3",
                                            "SE_SY_GET_PRIZE_RING_2"};
    static const f32 sOnTriggerFlashFrames[] = {10.0f, 60.0f, 110.0f, 160.0f, 210.0f, 240.0f, 270.0f, 300.0f, -1.0f};
};  // namespace

namespace NrvPrizeRing {
    NEW_NERVE(PrizeRingStart, PrizeRing, Start);
    NEW_NERVE(PrizeRingLoop, PrizeRing, Loop);
    NEW_NERVE(PrizeRingTimeout, PrizeRing, Timeout);
    NEW_NERVE(PrizeRingReadyToPass, PrizeRing, ReadyToPass);
    NEW_NERVE(PrizeRingPass, PrizeRing, Pass);
    NEW_NERVE(PrizeRingReadyToKill, PrizeRing, ReadyToKill);
};  // namespace NrvPrizeRing

PrizeRing::PrizeRing() : ModelObj("PrizeRing", "PrizeRing", nullptr, -2, -2, -2, false) {
    mLifeTime = 800;
}

PrizeRing::~PrizeRing() {
}

void PrizeRing::init(const JMapInfoIter& rIter) {
    initNerve(&NrvPrizeRing::PrizeRingStart::sInstance);
    initHitSensor(1);
    TVec3f offset;
    offset.x = 0.0f;
    offset.y = 0.0f;
    offset.z = 0.0f;
    MR::addHitSensorMapObjSimple(this, "body", 4, 300.0f, offset);
    makeActorDead();
}

void PrizeRing::appear() {
    makeActorAppeared();
    MR::showModel(this);
    MR::invalidateClipping(this);
    setNerve(&NrvPrizeRing::PrizeRingStart::sInstance);
}

void PrizeRing::kill() {
    makeActorDead();
}

void PrizeRing::setLife(int pTime) {
    mLifeTime = pTime;
}

void PrizeRing::setNumber(int pNum) {
    f32 num = pNum - 1.0f;
    MR::startBva(this, "Number");
    MR::setBvaFrameAndStop(this, num);
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
        MR::startSound(this, "SE_OJ_PRIZE_RING_APPEAR");
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

    if (MR::isStep(this, mLifeTime)) {
        setNerve(&NrvPrizeRing::PrizeRingTimeout::sInstance);
    }
}

void PrizeRing::exeTimeout() {
    if (MR::isFirstStep(this)) {
        MR::startBrk(this, "Timeout");
    }

    if (PrizeRing::isOnTriggerTimeoutFlash()) {
        MR::startSound(this, "SE_OJ_PRIZE_RING_BLINK");
    }

    if (MR::isBrkStopped(this)) {
        MR::startSound(this, "SE_OJ_PRIZE_RING_DISAPPEAR");
        kill();
    }
}

void PrizeRing::exeReadyToPass() {
}

void PrizeRing::exePass() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "End", nullptr);
        MR::startBrk(this, "End");
        MR::tryRumblePadMiddle(this, 0);
        playSound();
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

bool PrizeRing::isPassed() const {
    bool pass = isReadyToPass();

    if (!pass) {
        pass = isNerve(&NrvPrizeRing::PrizeRingPass::sInstance);
    }

    if (!pass) {
        return isReadyToKill();
    }

    return pass;
}

void PrizeRing::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (MR::isSensorPlayer(pReceiver)) {
        if (!isPassed()) {
            setNerve(&NrvPrizeRing::PrizeRingReadyToPass::sInstance);
        }
    }
}

s32 PrizeRing::getNumber() const {
    return static_cast< s32 >(MR::getBvaCtrl(this)->mFrame) + 1;
}

void PrizeRing::playSound() const {
    const char* sound = nullptr;
    if (getNumber() < static_cast< u32 >(ARRAY_SIZE(::sPlaySoundNames))) {
        sound = ::sPlaySoundNames[getNumber()];
    }

    if (sound == nullptr) {
        sound = "SE_SY_GET_PRIZE_RING_1";
    }

    MR::startSystemSE(sound);
    if (getNumber() == 1) {
        MR::startSystemSE("SE_SY_TOTAL_COMPLETE");
    }
}

bool PrizeRing::isOnTriggerTimeoutFlash() const {
    if (!isNerve(&NrvPrizeRing::PrizeRingTimeout::sInstance)) {
        return false;
    }

    J3DFrameCtrl* brkCtrl = MR::getBrkCtrl(this);
    for (s32 i = 0; i < 9; i++) {
        f32 frame;
        f32 rate;
        f32 flashFrames;

        flashFrames = ::sOnTriggerFlashFrames[i];
        frame = brkCtrl->mFrame;
        if (frame <= flashFrames) {
            rate = brkCtrl->mRate;
            if (flashFrames < frame + rate) {
                return true;
            }
        }
    }
    return false;
}
