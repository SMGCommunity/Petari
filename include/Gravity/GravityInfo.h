#pragma once

#include "JSystem/JGeometry/TVec3.h"

class GravityInfo
{
public:
    GravityInfo();

    void init();

    JGeometry::TVec3<f32> _0;
    s32 _C;
    u32 _10;
};