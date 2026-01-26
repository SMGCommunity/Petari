#pragma once

#include <revolution.h>

const f32 HALF_PI = 1.5707964f;
const f32 PI = 3.1415927f;
const f32 TWO_PI = 6.2831855f;
const f64 TWO_PI_D = 6.283185482025146;

const f32 PI_180 = 0.017453292f;
const f32 _180_PI = 57.29578f;

const f32 FLOAT_MAX = 3.4028235e38;
const f32 FLOAT_ZERO = 0.0f;

const static Vec gZeroVec = {0.0f, 0.0f, 0.0f};

namespace std {
    f32 atan2(f32, f32);
};
