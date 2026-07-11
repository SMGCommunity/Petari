#include "Game/MapObj/SpinDriverOperateRing.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util.hpp"

namespace NrvSpinDriverOperateRing {
    NEW_NERVE(SpinDriverOperateRingNrvWait, SpinDriverOperateRing, Wait);
};  // namespace NrvSpinDriverOperateRing

void FORCE_OPERATOR() {
    TVec3f vec;
    TVec3f vec2;
    vec.scale(1.0f);
    vec -= vec2;
}

SpinDriverOperateRing::SpinDriverOperateRing(const char* pName)
    : LiveActor(pName), _8C(0, 0, 0), _98(0, 0, 0), _A4(0, 0, 0), mAccelerate(0, 0, 0), mDirection(0, 0, 0) {
    _E0 = 0;
    _C8 = 0.0f;
    _CC = 0.0f;
    _D0 = 0.0f;
    mRadiusRate = 0.0f;
    _D8 = 0.0f;
    _DC = 0.0f;
}

void SpinDriverOperateRing::init(const JMapInfoIter& rIter) {
    makeActorAppeared();
    MR::invalidateClipping(this);
}

void SpinDriverOperateRing::control() {
}

void SpinDriverOperateRing::exeWait() {
}

void SpinDriverOperateRing::setRadiusRate(f32 rate) {
    mRadiusRate = MR::normalize(rate, 0.0f, 1.0f);
}

void SpinDriverOperateRing::reset() {
    resetVelocityAndTrans();
}

void SpinDriverOperateRing::update(const TVec3f& a1, const TVec3f& a2) {
    _8C.set< f32 >(a1);
    _98.set< f32 >(a2);

    if (mRadiusRate <= 0.0f) {
        resetVelocityAndTrans();
    } else {
        updateControlPoint(a2);
        addAccelToOperatePlane(a2);
        TVec3f stack_8;
        addAccelOperate(&stack_8, a2);
        addAccelToCenter();
        attenuateVelocity();
        updateDirection(stack_8);
        updatePosition();
    }
}

void SpinDriverOperateRing::updateDirection(const TVec3f& rVec) {
    mDirection = rVec * 0.1f + mDirection * 0.9f;
}

void SpinDriverOperateRing::resetVelocityAndTrans() {
    mDirection.zero();
    mAccelerate.zero();
    _A4.zero();
    _D8 = 0.0f;
    _DC = 0.0f;
}

void SpinDriverOperateRing::addAccelToOperatePlane(const TVec3f& rVec) {
    f32 dot = -rVec.dot(_A4);
    mAccelerate += rVec * (0.05f * dot);
}

void SpinDriverOperateRing::addAccelToCenter() {
    f32 norm = MR::normalize(_DC, 0.1f, 1.0f);

    if (norm > 0.0001f) {
        f32 scalar = ((0.5f * norm) / _D8);
        mAccelerate -= _A4 * scalar;
    }
}

void SpinDriverOperateRing::attenuateVelocity() {
    mAccelerate.x *= 0.94f;
    mAccelerate.y *= 0.94f;
    mAccelerate.z *= 0.94f;
}

SpinDriverOperateRing::~SpinDriverOperateRing() {
}
