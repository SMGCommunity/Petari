#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "JSystem/JGeometry/TVec.hpp"

class CollisionShadow : NameObj {
public:
    CollisionShadow(f32, f32);

    void setMode(u32);
    void create(const TVec3f&, const TVec3f&, const TVec3f&);

    /* 0x0C */ u8 _C[0x18 - 0xC];
    /* 0x18 */ f32 _18;
    /* 0x1C */ f32 _1C;
    /* 0x20 */ f32 _20;
    /* 0x24 */ u8 _24[0x3C - 0x24];
    /* 0x3C */ TVec3f _3C;
    /* 0x48 */ TVec3f _48;
    /* 0x54 */ u8 _54[0x34C - 0x54];
};
