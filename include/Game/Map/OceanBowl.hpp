#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <revolution.h>

class OceanBowlPoint;
class OceanBowlBloomDrawer;
class WaterInfo;

class OceanBowl : public LiveActor {
public:
    OceanBowl(const char*);

    virtual ~OceanBowl();
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
    // void drawDebug(); // We know this existed thanks to a debug symbol map
    
    // getPoint__9OceanBowlCFii from the debug maps, always inlined
    OceanBowlPoint* getPoint(int x, int y) const { return mPoints[x][y]; }
    // setPoint__9OceanBowlFiiP14OceanBowlPoint from the debug maps, always inlined
    void setPoint(int x, int y, OceanBowlPoint* pPoint) { mPoints[x][y] = pPoint; }

    TVec3f mSide;
    TVec3f mUp;
    TVec3f mFront;
    f32 mRadius;
    OceanBowlPoint* mPoints[25][25]; // 0xB4 : 25x25
    f32 mWaveX; // 0xA78
    f32 mWaveZ; // 0xA7C
    f32 mTexU0; // 0xA80
    f32 mTexV0; // 0xA84
    f32 mTexU1; // 0xA88
    f32 mTexV1; // 0xA8C
    f32 mTexU2; // 0xA90
    f32 mTexV2; // 0xA94
    f32 _A98; // 0xA98 : I have no idea what this does
    f32* mVtxTex0Array; // 0xA9C
    f32* mVtxTex1Array; // 0xAA0
    f32* mVtxTex2Array; // 0xAA4
    f32* mVtxTex3Array; // 0xAA8
    JUTTexture* mWaterTex;               // _AAC
    JUTTexture* mWaterIndirectTex;       // _AB0
    JUTTexture* mMaskTex;                // _AB4
    OceanBowlBloomDrawer* mBloomDrawer;  // _AB8
    bool mIsClipped; // 0xABC
};