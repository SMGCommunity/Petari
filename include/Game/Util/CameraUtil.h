#pragma once

#include "JSystem/JGeometry/TVec.h"
#include <revolution.h>

namespace MR {
    TVec3f getCamPos();

    bool isCameraInterpolateNearlyEnd();

    void setShakeOffset(f32, f32);

    f32 getFarZ();

    void loadProjectionMtx();
    MtxPtr getCameraViewMtx();
};