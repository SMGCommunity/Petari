#pragma once

#include <revolution.h>
#include "JSystem/JGeometry/TVec.h"

namespace MR {
    void closeSystemWipeCircleWithCaptureScreen(s32);

    void setWipeCircleCenterPos(const TVec3f &);

    void createCenterScreenBlur();

    void createNormalBloom();
    void createSimpleBloom();

    void createScreenBlur();
    void startCenterScreenBlur(long, float, unsigned char, long, long);

    void createDepthOfFieldBlur();
    void turnOffDOFInSubjective();
};
