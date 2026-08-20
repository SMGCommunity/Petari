#include "Game/Camera/CameraShakePatternImpl.hpp"
#include "Game/AreaObj/MercatorTransformCube.hpp"
#include "Game/Util/MathUtil.hpp"

void CameraShakePatternImpl_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)(MR::pi() * 0.5f);
}

namespace {
    static const s32 sSinglyMaxFrame = 25;
}

CameraShakePatternSingly::CameraShakePatternSingly(f32 intensity) : CameraShakePattern(), mIntensity(intensity), mDirection(0.0f, 1.0f) {
}

bool CameraShakePatternSingly::isEnd() const {
    return mFrame >= ::sSinglyMaxFrame;
}

void CameraShakePatternSingly::getOffset(TVec2f* pOffset) const {
    pOffset->set(mOffset);
}

void CameraShakePatternSingly::start() {
    mOffset.zero();
}

void CameraShakePatternSingly::update() {
    s32 framesLeft = ::sSinglyMaxFrame - mFrame;

    if (framesLeft < 0) {
        framesLeft = 0;
    }

    f32 amount = mIntensity * MR::sin(MR::pi() * 4.0f * framesLeft / ::sSinglyMaxFrame);
    amount *= MR::sin(MR::pi() * 0.5f * framesLeft / ::sSinglyMaxFrame);

    mOffset.set(mDirection * amount);
}

void CameraShakePatternSingly::setDirection(const TVec2f& rDir) {
    mDirection.set(rDir);
    MR::normalize(&mDirection);
}

CameraShakePatternVerticalSin::CameraShakePatternVerticalSin(f32 intensity, f32 speed) : mIntensity(intensity), mSpeed(speed), mOffset(0.0f, 0.0f) {
}

bool CameraShakePatternVerticalSin::isEnd() const {
    return mFrame >= mSpeed;
}

void CameraShakePatternVerticalSin::getOffset(TVec2f* pOffset) const {
    pOffset->set(mOffset);
}

void CameraShakePatternVerticalSin::start() {
    mOffset.set(0.0f, 0.0f);
}

void CameraShakePatternVerticalSin::update() {
    if (mSpeed < 0.01) {
        mOffset.zero();
        return;
    }

    f32 amount = mIntensity * -MR::sin(TWO_PI * mFrame / mSpeed);

    mOffset.set(0.0f, amount);
}
