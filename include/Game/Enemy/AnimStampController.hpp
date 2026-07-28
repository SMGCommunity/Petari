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
    void exeAnimEnd();
    bool isAnimEnd() const;

    /* 0x8  */ TVec3f _8;
    /* 0x14 */ TPos3f _14;
    /* 0x44 */ s32 _44;
    /* 0x50 */ TVec3f _50;  // unused
    /* 0x5C */ TVec3f _5C;  // unused
};
