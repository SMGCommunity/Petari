#pragma once

#include <revolution/mtx.h>
#include "JSystem/JGeometry/TVec3.h"
#include "JSystem/JGeometry/TMatrix34.h"

namespace MR
{
    void makeMtxSideUp(TPosition3Mtxf *, const JGeometry::TVec3f &, const JGeometry::TVec3f &);

    void makeMtxRotate(Mtx *, const JGeometry::TVec3f &);

    void extractMtxTrans(Mtx *, JGeometry::TVec3f *);
    bool isSameMtx(Mtx *, Mtx *);

    void extractMtxXDir(Mtx *, JGeometry::TVec3f *);
    void extractMtxYDir(Mtx *, JGeometry::TVec3f *);
};