#pragma once

#include <revolution.h>
#include "JGeometry/TVec3.h"

namespace MR
{
    f32 getInterpolatevalue(f32, f32, f32);
    f32 getLinerValue(f32, f32, f32, f32);

    bool isSameDirection(const JGeometry::TVec3<f32> &, const JGeometry::TVec3<f32> &, f32);

    bool isNearZero(f32, f32);
    bool isNearZero(const JGeometry::TVec3<f32> &, f32);
    f32 mod(f32, f32);
    void normalize(JGeometry::TVec3<f32> *);

    f32 getRandom();
    f32 getRandom(f32, f32);
    f32 getRandom(s32, s32);
    f32 getRandomDegree();

    f32 min(f32, f32);
    f32 max(f32, f32);

    void vecKillElement(const JGeometry::TVec3<f32> &, const JGeometry::TVec3<f32> &, JGeometry::TVec3<f32> *);
    void vecScaleAll(const JGeometry::TVec3<f32> *, const JGeometry::TVec3<f32> *, f32);
    void PSvecBlend(const JGeometry::TVec3<f32> *, const JGeometry::TVec3<f32> *, JGeometry::TVec3<f32> *, f32, f32);
};