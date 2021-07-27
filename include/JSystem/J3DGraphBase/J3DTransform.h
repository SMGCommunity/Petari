#pragma once

#include "JSystem/JGeometry/TVec3.h"
#include <revolution.h>

class J3DTransformInfo
{
public:
    void operator=(const J3DTransformInfo &);

    JGeometry::TVec3f mScale; // _0
    u16 _C;
    u16 _E;
    u16 _10;
    u16 _12; // padding?
    JGeometry::TVec3f mPosition; // _14
};

void J3DScaleNrmMtx(Mtx44 *, const Vec &);
void J3DScaleNrmMtx33(Mtx33 *, const Vec &);
void J3DMtxProjConcat(Mtx44 *, Mtx44 *, Mtx44 *);
void J3DPSMTxArrayConcat(Mtx44 *, Mtx44 *, Mtx44 *, u64);

const static J3DTransformInfo j3dDefaultTransformInfo;
const static JGeometry::TVec3f j3dDefaultScale;
const static Mtx j3dDefaultMtx;