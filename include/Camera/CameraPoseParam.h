#pragma once

#include "JGeometry/TVec3.h"

#include <revolution.h>

class CameraPoseParam
{
public:
    CameraPoseParam();

    void copyFrom(const CameraPoseParam &);

    JGeometry::TVec3<f32> mWatchUpVec; // _0
    JGeometry::TVec3<f32> mWatchPos; // _C
    JGeometry::TVec3<f32> mUpVector; // _18
    JGeometry::TVec3<f32> mPosition; // _24
    f32 mFovy; // _30
    JGeometry::TVec3<f32> mGlobalOffset; // _34
    JGeometry::TVec3<f32> mLocalOffset; // _40
    f32 mFrontOffset; // _4C
    f32 mUpperOffset; // _50
    f32 mRoll; // _54
};