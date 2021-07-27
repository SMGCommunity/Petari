#pragma once

#include <revolution/mtx.h>
#include "JSystem/JGeometry/TVec3.h"
#include "JSystem/JGeometry/TMatrix34.h"

namespace MR
{
    void makeMtxSideUp(TPosition3Mtxf *, const JGeometry::TVec3<f32> &, const JGeometry::TVec3<f32> &);

    void makeMtxRotate(Mtx *, const JGeometry::TVec3<f32> &);

    void extractMtxTrans(Mtx *, JGeometry::TVec3<f32> *);
    bool isSameMtx(Mtx *, Mtx *);

    void extractMtxXDir(Mtx *, JGeometry::TVec3<f32> *);
    void extractMtxYDir(Mtx *, JGeometry::TVec3<f32> *);
};