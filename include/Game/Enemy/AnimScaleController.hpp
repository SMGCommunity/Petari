#pragma once

#include "Game/System/NerveExecutor.hpp"
#include "JSystem/JGeometry.hpp"

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
    AnimScaleController(AnimScaleParam*);

    virtual ~AnimScaleController();

    void update();
    void updateScale(f32, f32);
    void startCrush();
    void startAnim();
    void startAndAddScaleVelocityY(f32);

    void setParamTight();
    void startHitReaction();
    void startDpdHitVibration();
    bool isHitReaction(s32) const;

    void stopAndReset();
    void resetScale();

    bool tryStop();

    void exeAnim();
    inline void exeStop();
    void exeDpdVibration();
    void exeHitReaction();
    void exeCrush();

    AnimScaleParam* _8;
    TVec3f _C;
    f32 _18;
};
