#pragma once

#include "JSystem/JGeometry/TVec3.h"

#include <revolution.h>

class CameraPoseParam
{
public:
    CameraPoseParam();

    void copyFrom(const CameraPoseParam &);

    JGeometry::TVec3f mWatchUpVec; // _0
    JGeometry::TVec3f mWatchPos; // _C
    JGeometry::TVec3f mUpVector; // _18
    JGeometry::TVec3f mPosition; // _24
    f32 mFovy; // _30
    JGeometry::TVec3f mGlobalOffset; // _34
    JGeometry::TVec3f mLocalOffset; // _40
    f32 mFrontOffset; // _4C
    f32 mUpperOffset; // _50
    f32 mRoll; // _54
};
