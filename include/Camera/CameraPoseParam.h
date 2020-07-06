#pragma once

#include "JGeometry/TVec3.h"

#include <revolution.h>

class CameraPoseParam
{
public:
    CameraPoseParam();

    void copyFrom(const CameraPoseParam &);

    JGeometry::TVec3<f32> _0;
    JGeometry::TVec3<f32> _C;
    JGeometry::TVec3<f32> _18;
    JGeometry::TVec3<f32> _24;
    JGeometry::TVec3<f32> _34;
    JGeometry::TVec3<f32> _40;
    f32 _4C;
    f32 _50;
    f32 _54;
};