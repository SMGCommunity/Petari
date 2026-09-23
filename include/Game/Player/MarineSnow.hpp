#pragma once

#include <JSystem/JGeometry/TVec.hpp>

class JUTTexture;

class MarineSnow {
public:
    MarineSnow();

    void view();
    void clear();
    void draw(const TVec3f& rPosition, const TVec3f& rUp, f32 surfaceDistance) const;

    /* 0x00 */ u32 mParticleCount;
    /* 0x04 */ TVec3f* mParticlePositions;
    /* 0x08 */ f32 mCellSize;
    /* 0x0C */ u16 mFadeStep;
    /* 0x10 */ JUTTexture* mTexture;
};
