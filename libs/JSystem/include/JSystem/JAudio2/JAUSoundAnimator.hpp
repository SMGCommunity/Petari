#pragma once

#include "JSystem/JGeometry.hpp"
#include <revolution.h>

class JAISoundHandles;
class JAUSoundAnimationSound;
class JASSoundParams;

class JAUSoundAnimator {
public:
    JAUSoundAnimator(JAISoundHandles*);

    virtual u32* getFreeHandle(const JAUSoundAnimationSound*);
    virtual s32 getSoundID(const JAUSoundAnimationSound*, const TVec3f&, f32);
    virtual bool playSound(const JAUSoundAnimationSound*, const TVec3f&, f32);
    virtual void modifySoundParams(JASSoundParams*, const JAUSoundAnimationSound*, f32);

    JAISoundHandles* mHandles;  // 0x04
    u32 _8;
    u32 _C;
    u32 _10;
};
