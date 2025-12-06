#pragma once

#include "Game/System/NerveExecutor.hpp"
#include <JSystem/JGeometry.hpp>

class AnimStampController : public NerveExecutor {
public:
    AnimStampController();

    virtual ~AnimStampController();

    void init(s32, f32, f32, f32, f32, f32, f32);
    void startAnim(const TVec3f&);
    void exeAnim();
    bool isAnimEnd() const;

    TVec3f _8;
    TPos3f _14;
};
