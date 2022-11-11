#pragma once

#include "JSystem/JGeometry.h"
#include "Game/System/NerveExecutor.h"

class AnimScaleParam;

class AnimScaleController : public NerveExecutor {
public:
    AnimScaleController(AnimScaleParam *);

    virtual ~AnimScaleController();

    void update();
    void startCrush();
    void startAnim();

    void startDpdHitVibration();

    AnimScaleParam* _8;
    TVec3f _C;
    f32 _18;
};
