#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class JUTTexture;

class WhirlPoolPoint;

class WhirlPool : public LiveActor {
public:
    WhirlPool(const char* pName);

    virtual ~WhirlPool();
    virtual void init(const JMapInfoIter& rIter);
    virtual void movement();
    virtual void draw() const;

    void initPoints();
    void drawPlane(f32 x0, f32 y0, f32 x1, f32 y1, f32 u0, f32 u1, f32 u2, f32 u3) const;
    void loadMaterial() const;

    /* 0x8C */ f32 mRadius;
    /* 0x90 */ f32 mHeight;
    /* 0x94 */ TVec3f mAxis;
    /* 0xA0 */ s32 mPointCount;
    /* 0xA4 */ WhirlPoolPoint** mPoints;
    /* 0xA8 */ f32 mRotation;
    /* 0xAC */ f32 mTexOffsetU0;
    /* 0xB0 */ f32 mTexOffsetV0;
    /* 0xB4 */ f32 mTexOffsetU1;
    /* 0xB8 */ f32 mTexOffsetV1;
    /* 0xBC */ f32 mTexOffsetU2;
    /* 0xC0 */ f32 mTexOffsetV2;
    /* 0xC4 */ JUTTexture* mTexture;
    /* 0xC8 */ JUTTexture* mIndirectTexture;
    /* 0xCC */ TVec3f mClippingCenter;
};
