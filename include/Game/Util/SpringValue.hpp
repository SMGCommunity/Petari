#pragma once

#include <revolution.h>

class SpringValue {
public:
    SpringValue();
    SpringValue(f32, f32, f32, f32, f32);

    void setParam(f32, f32, f32, f32, f32);
    void update();

    void reset();

    f32 _0;
    f32 _4;
    f32 _8;
    f32 _C;
    f32 _10;
};
