#pragma once

#include "Game/NameObj/NameObj.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class CollisionShadow : NameObj {
public:
    CollisionShadow(f32, f32);

    void setMode(u32);
    void create(const TVec3f&, const TVec3f&, const TVec3f&);

    /* 0x00C */ u8 _C[0x18 - 0xC];
    /* 0x018 */ f32 _18;
    /* 0x01C */ f32 _1C;
    /* 0x020 */ f32 _20;
    /* 0x024 */ u8 _24[0x3C - 0x24];
    /* 0x03C */ TVec3f _3C;
    /* 0x048 */ TVec3f _48;
    /* 0x054 */ u8 _54[0x304 - 0x54];
    /* 0x304 */ u8 _304;
    /* 0x305 */ bool _305;
    /* 0x306 */ u8 _306[0x34C - 0x306];
};
