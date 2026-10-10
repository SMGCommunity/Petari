#pragma once

#include <JSystem/JGeometry.hpp>
#include <revolution.h>

class Swinger {
public:
    Swinger(const TVec3f*, MtxPtr, f32, f32, f32, const TVec3f*);

    void update();
    void accel(const TVec3f&);
    void updateSwingMtx(const TVec3f&);

    /* 0x00 */ const TVec3f* _0;
    /* 0x04 */ TPos3f* _4;
    /* 0x08 */ TVec3f _8;
    /* 0x14 */ TVec3f mAcceleration;
    /* 0x20 */ TVec3f _20;
    /* 0x2C */ f32 _2C;
    /* 0x30 */ f32 _30;
    /* 0x34 */ f32 _34;
    /* 0x38 */ const TVec3f* _38;
    /* 0x3C */ TVec3f _3C;
    /* 0x48 */ TVec3f _48;
    /* 0x54 */ TVec3f _54;
    /* 0x60 */ TPos3f _60;
};
