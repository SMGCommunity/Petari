#pragma once

#include <cmath>
#include <revolution.h>

const f32 HALF_PI = 1.5707964f;
const f32 PI = 3.1415927f;
const f32 TWO_PI = 6.2831855f;
const f64 TWO_PI_D = 6.283185482025146;

const f32 PI_180 = 0.017453292f;
const f32 _180_PI = 57.29578f;
const f32 DEGREE_TO_S16 = 182.04445f;
const f32 PI_4 = 0.78539819f;
const f32 _4_PI = 1.2566371f;
const f32 PI_6 = 0.52359879f;

const f32 FLOAT_MAX = 3.4028235e38;
const f32 FLOAT_ZERO = 0.0f;

extern const Vec gZeroVec;

namespace std {
    inline f32 atan2(f32 x, f32 y) {
        return ::atan2(x, y);
    }
};  // namespace std
