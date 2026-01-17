#pragma once

#include "JSystem/JGeometry/TVec.hpp"

class MarineSnow {
public:
    MarineSnow();
    void view();
    void clear();
    void draw(const TVec3f&, const TVec3f&, f32) const;

    u8 _0[0x14];
};
