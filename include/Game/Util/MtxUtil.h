#pragma once

#include <revolution.h>
#include "JSystem/JGeometry/TVec.h"
#include "JSystem/JGeometry/TMatrix.h"

namespace MR {

    void makeMtxRotate(MtxPtr, f32, f32, f32);

    void makeMtxTR(MtxPtr, f32, f32, f32, f32, f32, f32);
    void makeMtxTR(MtxPtr, const TVec3f &, const TVec3f &);

    void makeMtxTRS(MtxPtr, const TVec3f &rTranslation, const TVec3f &rRotation, const TVec3f &rScale);

    void makeMtxSideUp(TPos3f *, const TVec3f &, const TVec3f &);

    MtxPtr tmpMtxRotXDeg(f32);
    MtxPtr tmpMtxRotYDeg(f32);
    MtxPtr tmpMtxRotZDeg(f32);

    void setMtxTrans(MtxPtr, f32, f32, f32);
    void addTransMtx(MtxPtr, const TVec3f &);
    void addTransMtxLocal(MtxPtr, const TVec3f &);
    void addTransMtxLocalX(MtxPtr, f32);
    void addTransMtxLocalY(MtxPtr, f32);
    void addTransMtxLocalZ(MtxPtr, f32);
    void extractMtxXDir(MtxPtr, TVec3f *);
    void extractMtxYDir(MtxPtr, TVec3f *);
    void extractMtxZDir(MtxPtr, TVec3f *);
    void extractMtxXYZDir(MtxPtr, TVec3f *, TVec3f *, TVec3f *);
    void extractMtxTrans(MtxPtr, TVec3f *);
    bool isSameMtx(MtxPtr, MtxPtr);
};
