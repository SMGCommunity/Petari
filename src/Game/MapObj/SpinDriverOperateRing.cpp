#include "Game/MapObj/SpinDriverOperateRing.hpp"

SpinDriverOperateRing::SpinDriverOperateRing(const char *pName) : LiveActor(pName),
    _8C(0, 0, 0), _98(0, 0, 0), _A4(0, 0, 0), mAccelerate(0, 0, 0), mDirection(0, 0, 0)
    {
        _E0 = 0;
        _C8 = 0.0f;
        _CC = 0.0f;
        _D0 = 0.0f;
        mRadiusRate = 0.0f;
        _D8 = 0.0f;
        _DC = 0.0f;
}

void SpinDriverOperateRing::init(const JMapInfoIter &rIter) {
    makeActorAppeared();
    MR::invalidateClipping(this);
}

void SpinDriverOperateRing::control() {
    
}

void SpinDriverOperateRing::setRadiusRate(f32 rate) {
    mRadiusRate = MR::normalize(rate, 0.0f, 1.0f);
}

void SpinDriverOperateRing::reset() {
    resetVelocityAndTrans();
}

void SpinDriverOperateRing::update(const TVec3f &a1, const TVec3f &a2) {
    _8C.set<f32>(a1);
    _98.set<f32>(a2);

    if (mRadiusRate <= 0.0f) {
        resetVelocityAndTrans();
    }
    else {
        
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

#ifdef NON_MATCHING
void SpinDriverOperateRing::updateDirection(const TVec3f &rVec) {
    TVec3f stack_20;
    TVec3f direction(mDirection);
    direction.scale(0.89999998f);
    TVec3f stack_14(rVec);
    stack_14.scale(0.1f);
    TVec3f stack_20(stack_14);
    stack_20 += direction;
    mDirection = stack_20;
}
#endif

void SpinDriverOperateRing::resetVelocityAndTrans() {
    mDirection.zero();
    mAccelerate.zero();
    _A4.zero();
    _D8 = 0.0f;
    _DC = 0.0f;
}

void SpinDriverOperateRing::addAccelToOperatePlane(const TVec3f &rVec) {
    f32 dot = -rVec.dot(_A4);
    TVec3f scalar(rVec);
    scalar.scale(0.050000001f * dot);
    mAccelerate += scalar;
}

void SpinDriverOperateRing::addAccelToCenter() {
    f32 norm = MR::normalize(_DC, 0.1f, 1.0f);

    if (norm > 0.000099999997f) {
        f32 scalar = ((0.5f * norm) / _D8);
        TVec3f accel(_A4);
        accel.scale(scalar);
        mAccelerate += accel;
    }
}

void SpinDriverOperateRing::attenuateVelocity() {
    mAccelerate.x *= 0.94f;
    mAccelerate.y *= 0.94f;
    mAccelerate.z *= 0.94f;
}

SpinDriverOperateRing::~SpinDriverOperateRing() {

}

namespace NrvSpinDriverOperateRing {
    INIT_NERVE(SpinDriverOperateRingNrvWait);

    void SpinDriverOperateRingNrvWait::execute(Spine *) const {

    }
};
