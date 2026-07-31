#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class JUTTexture;
class OceanBowlBloomDrawer;
class OceanBowlPoint;
class WaterInfo;

class OceanBowl : public LiveActor {
public:
    OceanBowl(const char*);

    virtual void init(const JMapInfoIter& rIter);
    virtual void movement();
    virtual void draw() const;

    bool isInWater(const TVec3f& rPos) const;
    bool calcWaterInfo(const TVec3f& rPos, const TVec3f& rGravity, WaterInfo* pInfo) const;
    void initPoints();
    void updatePoints();
    void moveToLeft();
    void moveToRight();
    void moveToUpper();
    void moveToLower();
    void loadMaterial() const;
    void loadMaterialBloom() const;
    // void drawDebug();

    // getPoint__9OceanBowlCFii from the debug maps, always inlined
    OceanBowlPoint* getPoint(int x, int y) const {
        return mPoints[x][y];
    }

    // setPoint__9OceanBowlFiiP14OceanBowlPoint from the debug maps, always inlined
    void setPoint(int x, int y, OceanBowlPoint* pPoint) {
        mPoints[x][y] = pPoint;
    }

    /* 0x08C */ TVec3f mSide;
    /* 0x098 */ TVec3f mUp;
    /* 0x0A4 */ TVec3f mFront;
    /* 0x0B0 */ f32 mRadius;
    /* 0x0B4 */ OceanBowlPoint* mPoints[25][25];
    /* 0xA78 */ f32 mWaveX;
    /* 0xA7C */ f32 mWaveZ;
    /* 0xA80 */ f32 mTexU0;
    /* 0xA84 */ f32 mTexV0;
    /* 0xA88 */ f32 mTexU1;
    /* 0xA8C */ f32 mTexV1;
    /* 0xA90 */ f32 mTexU2;
    /* 0xA94 */ f32 mTexV2;
    /* 0xA98 */ f32 _A98;
    /* 0xA9C */ f32* mVtxTex0Array;
    /* 0xAA0 */ f32* mVtxTex1Array;
    /* 0xAA4 */ f32* mVtxTex2Array;
    /* 0xAA8 */ f32* mVtxTex3Array;
    /* 0xAAC */ JUTTexture* mWaterTex;
    /* 0xAB0 */ JUTTexture* mWaterIndirectTex;
    /* 0xAB4 */ JUTTexture* mMaskTex;
    /* 0xAB8 */ OceanBowlBloomDrawer* mBloomDrawer;
    /* 0xABC */ bool mIsClipped;
};
