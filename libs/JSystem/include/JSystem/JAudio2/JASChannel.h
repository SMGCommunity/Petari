#pragma once

#include <revolution.h>
#include "JSystem/JAudio2/JASOscillator.h"

class JASChannel {
public:
    void setKey(s32);
    void setVelocity(u32);
    void setUpdateTimer(u32);
    void setKeySweepTarget(s32, u32);
    void setOscInit(u32, const JASOscillator::Data *);
    void setVibrate(f32, f32);
    void setTremolo(f32, f32);
    void setParams(const JASChannelParams &);
    void release(u16);
    void free();
    void setPauseFlag(bool);
    u8 _0[0xE1];
    u8 _E1;
};
