#ifndef J3DTRANSFORM_H
#define J3DTRANSFORM_H

#include "JGeometry/TVec3.h"
#include "types.h"

class J3DTransformInfo
{
public:
    void operator=(const J3DTransformInfo &);

    JGeometry::TVec3<f32> mScale; // _0
    u16 _C;
    u16 _E;
    u16 _10;
    u16 _12; // padding?
    f32 _14;
    f32 _18;
    f32 _1C;
};

void J3DScaleNrmMtx(Mtx44 *, const Vec &);
void J3DScaleNrmMtx33(Mtx33 *, const Vec &);
void J3DMtxProjConcat(Mtx44 *, Mtx44 *, Mtx44 *);
void J3DPSMTxArrayConcat(Mtx44 *, Mtx44 *, Mtx44 *, u64);

const static J3DTransformInfo j3dDefaultTransformInfo;
const static JGeometry::TVec3<f32> j3dDefaultScale;
const static Mtx j3dDefaultMtx;

#endif // J3DTRANSFORM_H