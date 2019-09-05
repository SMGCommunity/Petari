#ifndef MATHUTIL_H
#define MATHUTIL_H

#include "types.h"
#include "JGeometry/TVec3.h"

namespace MR
{
    f32 getInterpolatevalue(f32, f32, f32);
    f32 getLinerValue(f32, f32, f32, f32);
    bool isNearZero(f32, f32);
    f32 mod(f32, f32);
    void normalize(JGeometry::TVec3<f32> *);
};

#endif // MATHUTIL_H