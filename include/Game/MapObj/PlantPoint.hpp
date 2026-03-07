#pragma once

#include <JSystem/JGeometry/TVec.hpp>

class PlantPoint {
public:
    PlantPoint(const TVec3f&, const TVec3f&, f32);

    virtual void setAxisAndPos(const TVec3f&, const TVec3f&, const TVec3f&, const TVec3f&);

    /* 0x04 */ TVec3f mPosition;
    /* 0x10 */ TVec3f mSide;
    /* 0x1C */ TVec3f mUp;
    /* 0x28 */ TVec3f mFront;
    /* 0x34 */ f32 mThickness;
};
