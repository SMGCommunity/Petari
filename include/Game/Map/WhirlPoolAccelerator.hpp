#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class JUTTexture;

class WhirlPoolPoint {
public:
    WhirlPoolPoint(const TVec3f& rPosition, const TVec3f& rDirection, const TVec3f& rNormal, f32 radius, f32 texCoord, u8 alpha);

    /* 0x00 */ TVec3f mPosition;
    /* 0x0C */ TVec3f mSide;
    /* 0x18 */ TVec3f mNormal;
    /* 0x24 */ TVec3f mDirection;
    /* 0x30 */ f32 mRadius;
    /* 0x34 */ f32 mTexCoord;
    /* 0x38 */ u8 mAlpha;
    /* 0x39 */ bool _39;
};

class WhirlPoolAccelerator : public LiveActor {
public:
    WhirlPoolAccelerator(const char* pName);

    virtual ~WhirlPoolAccelerator();
    virtual void init(const JMapInfoIter& rIter);
    virtual void movement();
    virtual void draw() const;

    bool calcInfo(const TVec3f& rPosition, TVec3f* pAcceleration) const;
    void initPoints();
    void drawPlane(f32 x0, f32 y0, f32 x1, f32 y1, f32 u0, f32 u1) const;
    void loadMaterial() const;

    /* 0x8C */ f32 mRadius;
    /* 0x90 */ f32 mHeight;
    /* 0x94 */ TVec3f mAxis;
    /* 0xA0 */ s32 mPointCount;
    /* 0xA4 */ WhirlPoolPoint** mPoints;
    /* 0xA8 */ f32 mRotation;
    /* 0xAC */ f32 mTexOffsetU0;
    /* 0xB0 */ f32 mTexOffsetV0;
    /* 0xB4 */ JUTTexture* mTexture;
    /* 0xB8 */ TVec3f mClippingCenter;
};
