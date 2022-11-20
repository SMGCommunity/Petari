#pragma once

#include "JSystem/JGeometry.h"
#include "Game/System/NerveExecutor.h"

class AnimScaleParam {
public:
    AnimScaleParam();

    f32 _0;
    f32 _4;
    f32 _8;
    f32 _C;
    f32 _10;
    f32 _14;
    f32 _18;
    f32 _1C;
    f32 _20;
    f32 _24;
    f32 _28;
    u32 _2C;
    f32 _30;
};

class AnimScaleController : public NerveExecutor {
public:
    AnimScaleController(AnimScaleParam *);

    virtual ~AnimScaleController();

    void update();
    void startCrush();
    void startAnim();

    void setParamTight();

    void startDpdHitVibration();

    AnimScaleParam* _8;
    TVec3f _C;
    f32 _18;
};
