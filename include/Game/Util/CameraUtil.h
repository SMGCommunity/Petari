#pragma once

#include "JSystem/JGeometry/TVec.h"

namespace MR {
    TVec3f getCamPos();

    bool isCameraInterpolateNearlyEnd();

    void setShakeOffset(float, float);
};