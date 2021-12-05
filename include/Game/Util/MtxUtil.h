#pragma once

#include "revolution.h"
#include "JSystem/JGeometry/TVec.h"

namespace MR {

    void makeMtxRotate(MtxPtr, f32, f32, f32);

    void makeMtxTR(MtxPtr, f32, f32, f32, f32, f32, f32);
    void makeMtxTR(MtxPtr, const TVec3f &, const TVec3f &);

    MtxPtr tmpMtxRotXDeg(f32);
    MtxPtr tmpMtxRotYDeg(f32);
    MtxPtr tmpMtxRotZDeg(f32);
};
