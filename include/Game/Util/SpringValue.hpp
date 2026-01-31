#pragma once

#include <revolution.h>

class SpringValue {
public:
    SpringValue();
    SpringValue(f32, f32, f32, f32, f32);

    void setParam(f32, f32, f32, f32, f32);
    f32 update();

    void reset();

    f32 mRestValue;
    f32 mSpringValue;
    f32 mAccelFactor;
    f32 mDamping;
    f32 mVelocity;
};
