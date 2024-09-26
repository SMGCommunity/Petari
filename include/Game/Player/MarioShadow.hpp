#pragma once

#include "Game/NameObj/NameObj.hpp"

class CollisionShadow : NameObj {
public:
    CollisionShadow(f32, f32);

    u8 _C[0x34C - 0xC];
};
