#pragma once

#include "JSystem/JGeometry.hpp"
#include <revolution.h>

class RumbleCalculator {
public:
    RumbleCalculator(f32, f32, f32, u32);

    virtual void calcValues(TVec3f*, const TVec3f&) = 0;

    void start(u32);
    void calc();
    void reset();

    u32 _4;
    u32 _8;
    TVec3f _C;
    f32 _18;
    f32 _1C;
    f32 _20;
};

class RumbleCalculatorCosMultLinear : public RumbleCalculator {
public:
    RumbleCalculatorCosMultLinear(f32, f32, f32, u32);

    virtual void calcValues(TVec3f*, const TVec3f&);
};