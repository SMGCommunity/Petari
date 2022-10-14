#pragma once

#include "JSystem/JGeometry.h"

class AnimScaleParam;

class AnimScaleController {
public:
    AnimScaleController(AnimScaleParam *);
    void update();
    void startCrush();
    void startAnim();

    void startDpdHitVibration();

    u8 _0[0xc];
    TVec3f _C;
    u8 _18[0x4];
};
