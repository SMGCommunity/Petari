#pragma once

#include "JSystem/JGeometry.hpp"
#include <revolution.h>

class JAISoundHandles;
class JAUSoundAnimation;
class JAUSoundAnimationSound;
class JASSoundParams;

class JAUSoundAnimator {
public:
    JAUSoundAnimator(JAISoundHandles*);

    virtual u32* getFreeHandle(const JAUSoundAnimationSound*);
    virtual s32 getSoundID(const JAUSoundAnimationSound*, const TVec3f&, f32);
    virtual bool playSound(const JAUSoundAnimationSound*, const TVec3f&, f32);
    virtual void modifySoundParams(JASSoundParams*, const JAUSoundAnimationSound*, f32);

    void setLoopFrame(f32, f32);
    void startAnimation(const JAUSoundAnimation*, bool, f32, f32);
    void removeAnimation();

    /* 0x04 */ u32 _4;
    /* 0x08 */ union {
        JAISoundHandles* mHandles;
        JAUSoundAnimation* mSoundAnimation;
    };
    /* 0x0C */ u32 _C;
    /* 0x10 */ u32 _10;
    /* 0x14 */ u32 _14;
    /* 0x18 */ u32 mLoopStartSoundIndex;
    /* 0x1C */ u32 mLoopEndSoundIndex;
    /* 0x20 */ f32 mLoopStartFrame;
    /* 0x24 */ f32 mLoopEndFrame;
};
