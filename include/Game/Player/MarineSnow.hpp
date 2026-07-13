#pragma once

#include "JSystem/JGeometry/TVec.hpp"

class JUTTexture;

class MarineSnow {
public:
    MarineSnow();

    void view();
    void clear();
    void draw(const TVec3f&, const TVec3f&, f32) const;

    /* 0x00 */ u32 _0;
    /* 0x04 */ TVec3f* _4;
    /* 0x08 */ f32 _8;
    /* 0x0C */ u16 _C;
    /* 0x10 */ JUTTexture* mTexture;
};
