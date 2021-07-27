#pragma once

#include <revolution.h>
#include "JSystem/JGeometry/TVec2.h"
#include "JSystem/JGeometry/TVec3.h"

class StarPointerTarget
{
public:
    StarPointerTarget(f32, const JGeometry::TVec3f *, Mtx *, JGeometry::TVec3f);

    u8 _0[0x1C];
};