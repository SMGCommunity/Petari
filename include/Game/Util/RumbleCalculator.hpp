#pragma once

#include <JSystem/JGeometry.hpp>
#include <revolution.h>

class RumbleCalculator {
public:
    RumbleCalculator(f32, f32, f32, u32);

    virtual void calcValues(TVec3f*, const TVec3f&) = 0;

    void start(u32);
    void calc();
    void reset();

    bool isRumbling() const {
        return _4 < _8;
    }

    /* 0x04 */ u32 _4;
    /* 0x08 */ u32 _8;
    /* 0x0C */ TVec3f _C;
    /* 0x18 */ f32 _18;
    /* 0x1C */ f32 _1C;
    /* 0x20 */ f32 _20;
};

class RumbleCalculatorCosMultLinear : public RumbleCalculator {
public:
    RumbleCalculatorCosMultLinear(f32, f32, f32, u32);

    virtual void calcValues(TVec3f*, const TVec3f&);
};