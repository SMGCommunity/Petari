#pragma once

#include "JSystem/JAudio2/JASLfo.hpp"
#include "JSystem/JAudio2/JASOscillator.hpp"
#include <revolution.h>

class JASChannel {
public:
    void setKey(s32) NO_INLINE;
    void setVelocity(u32) NO_INLINE;
    void setUpdateTimer(u32) NO_INLINE;
    void setKeySweepTarget(s32, u32) NO_INLINE;
    void setOscInit(u32, const JASOscillator::Data*) NO_INLINE;
    void setVibrate(f32, f32) NO_INLINE;
    void setTremolo(f32, f32) NO_INLINE;
    void setParams(const JASChannelParams&) NO_INLINE;
    void release(u16) NO_INLINE;
    void free() NO_INLINE;
    void setPauseFlag(bool) NO_INLINE;
    u32 _0;
    bool _4;
    u8 _5[0xf];
    u32 _14;
    u8 _18[0x44];
    JASLfo _5C;
    JASLfo _74;
    u8 _8C[0x3C];
    s16 _C8;
    u16 _CA;
    u8 _CC[0x15];
    u8 _E1;
};
