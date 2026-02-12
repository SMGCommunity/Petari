#pragma once

#include "Game/NameObj/NameObj.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class CollisionShadow : public NameObj {
public:
    CollisionShadow(f32, f32);
    void setMode(u32);
    void create(const TVec3f&, const TVec3f&, const TVec3f&);

    u8 _C[0x18 - 0xC];
    f32 _18;
    f32 _1C;
    f32 _20;
    u8 _24[0x3C - 0x24];
    TVec3f _3C;
    TVec3f _48;
    u8 _54[0x34C - 0x54];
};
