#include "Game/Util/SpringValue.hpp"

SpringValue::SpringValue() {
    mRestValue = 0.0f;
    mSpringValue = 0.0f;
    mAccelFactor = 0.3f;
    mDamping = 0.8f;
    mVelocity = 0.0f;
}

SpringValue::SpringValue(f32 param1, f32 param2, f32 param3, f32 param4, f32 param5) {
    mRestValue = param1;
    mSpringValue = param2;
    mAccelFactor = param3;
    mDamping = param4;
    mVelocity = param5;
}

void SpringValue::setParam(f32 param1, f32 param2, f32 param3, f32 param4, f32 param5) {
    mRestValue = param1;
    mSpringValue = param2;
    mAccelFactor = param3;
    mDamping = param4;
    mVelocity = param5;
}

f32 SpringValue::update() {
    mSpringValue += mVelocity;
    mVelocity += mAccelFactor * (mRestValue - mSpringValue);
    mVelocity *= mDamping;
    return mSpringValue;
}
