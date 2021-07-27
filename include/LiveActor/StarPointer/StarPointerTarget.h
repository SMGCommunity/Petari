#pragma once

#include <revolution.h>
#include "JSystem/JGeometry/TVec2.h"
#include "JSystem/JGeometry/TVec3.h"

class StarPointerTarget
{
public:
    StarPointerTarget(f32, const JGeometry::TVec3<f32> *, Mtx *, JGeometry::TVec3<f32>);

    u8 _0[0x1C];
};