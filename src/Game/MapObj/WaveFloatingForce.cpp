#include "Game/MapObj/WaveFloatingForce.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/MathUtil.hpp"

void WaveFloatingForce_FORCE_MATCH_SDATA2() {
    (void)0.0f;
    (void)(MR::pi()/2.0f);
    (void)2.0f;
}

WaveFloatingForce::WaveFloatingForce(const LiveActor* pActor, s32 stepCount, f32 amplitude, bool) : mActor(pActor), mStepCount(stepCount), mAmplitude(amplitude), mStep(), _10() {
    mStep = 2 * MR::pi() * MR::getRandom();
}

void WaveFloatingForce::update() {
    if (!_10 && MR::isOnPlayer(mActor)) {
        updateOnPlayer();
    } else {
        updateNormal();
    }
}

f32 WaveFloatingForce::getCurrentValue() const {
    return mAmplitude * MR::sin(mStep);
}

void WaveFloatingForce::updateNormal() {
    mStep += 2 * MR::pi() / mStepCount;
    mStep = MR::repeat(mStep, 0.0f, 2 * MR::pi());
}

void WaveFloatingForce::updateOnPlayer() {
    f32 stepSize = 2 * MR::pi() / mStepCount;
    if (MR::pi() / 2.0f < mStep && mStep < 3.0f / 2.0f * MR::pi()) {
        mStep += stepSize;
    } else {
        mStep -= stepSize;
    }

    mStep = MR::repeat(mStep, 0.0f, 2.0f * MR::pi());

    if (MR::isNearZero(MR::abs(mStep) - 3.0f / 2.0f * MR::pi(), 2.0f * stepSize)) {
        mStep = 3.0f / 2.0f * MR::pi();
    }
}
