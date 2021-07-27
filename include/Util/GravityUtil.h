#pragma once

#include "JSystem/JGeometry/TVec3.h"
#include <revolution.h>

class GravityInfo;
class NameObj;

namespace MR
{
    bool calcDropShadowVector(const NameObj *, const JGeometry::TVec3f &, JGeometry::TVec3f *, GravityInfo *, u32);
    bool calcGravityVectorOrZero(const NameObj *, const JGeometry::TVec3f &, JGeometry::TVec3f *, GravityInfo *, u32);
};