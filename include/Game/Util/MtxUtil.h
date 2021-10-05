#pragma once

#include "revolution.h"

namespace MR {

    void makeMtxRotate(MtxPtr, f32, f32, f32);

    void makeMtxTR(MtxPtr, f32, f32, f32, f32, f32, f32);

    MtxPtr tmpMtxRotXDeg(f32);
    MtxPtr tmpMtxRotYDeg(f32);
    MtxPtr tmpMtxRotZDeg(f32);
};
