#pragma once

#include "JSystem/JGeometry/TVec3.h"
#include <revolution.h>

class Binder
{
public:
    Binder(Mtx *, const JGeometry::TVec3f *, const JGeometry::TVec3f *, f32, f32, u64);

    static void bind(JGeometry::TVec3f &, Binder *, const JGeometry::TVec3f &);
    void clear();

    u8 _0[0x1F0];
};