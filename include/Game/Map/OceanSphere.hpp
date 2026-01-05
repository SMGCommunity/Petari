#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include <revolution.h>

class JUTTexture;
class OceanSpherePlane;
class OceanSpherePlaneEdge;
class OceanSpherePoint;
class WaterInfo;

class OceanSphere : public LiveActor {
public:
    OceanSphere(const char*);

    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();
    virtual void control();
    virtual void draw() const;

    bool isInWater(const TVec3f& rPos) const;
    bool calcWaterInfo(const TVec3f& rPos, const TVec3f& rGravity, WaterInfo* pInfo) const;
    void exeWait();
    void exeRiseUp();
    void initPoints();
    void initDisplayList();
    void updatePoints();
    OceanSpherePoint* getPlanePointLeftUpper(int, int) const;
    OceanSpherePoint* getPlanePointRightUpper(int, int) const;
    OceanSpherePoint* getPlanePointLeftLower(int, int) const;
    OceanSpherePoint* getPlanePointRightLower(int, int) const;
    void loadMaterialFace() const;
    void loadMaterialBack() const;
    void drawSphere(bool, bool) const;
    void sendVertex(const OceanSpherePoint*, bool, bool) const;

    /* 0x8C */ f32 mRadius;
    /* 0x90 */ f32 mRadiusTarget;
    /* 0x94 */ s32 mPointCount;
    /* 0x98 */ OceanSpherePoint* mAxisPointPX;
    /* 0x9C */ OceanSpherePoint* mAxisPointNX;
    /* 0xA0 */ OceanSpherePoint* mAxisPointPY;
    /* 0xA4 */ OceanSpherePoint* mAxisPointNY;
    /* 0xA8 */ OceanSpherePoint* mAxisPointPZ;
    /* 0xAC */ OceanSpherePoint* mAxisPointNZ;
    /* 0xB0 */ OceanSpherePlane* mPlaneLeftUpper;
    /* 0xB4 */ OceanSpherePlane* mPlaneRightUpper;
    /* 0xB8 */ OceanSpherePlane* mPlaneLeftLower;
    /* 0xBC */ OceanSpherePlane* mPlaneRightLower;
    /* 0xC0 */ OceanSpherePlaneEdge* mEdge0;
    /* 0xC4 */ OceanSpherePlaneEdge* mEdge1;
    /* 0xC8 */ OceanSpherePlaneEdge* mEdge2;
    /* 0xCC */ OceanSpherePlaneEdge* mEdge3;
    /* 0xD0 */ OceanSpherePlaneEdge* mEdge4;
    /* 0xD4 */ OceanSpherePlaneEdge* mEdge5;
    /* 0xD8 */ OceanSpherePlaneEdge* mEdge6;
    /* 0xDC */ OceanSpherePlaneEdge* mEdge7;
    /* 0xE0 */ f32 mWaveTime1;
    /* 0xE4 */ f32 mWaveTime2;
    /* 0xE8 */ f32 mTexOffs0X;
    /* 0xEC */ f32 mTexOffs0Y;
    /* 0xF0 */ f32 mTexOffs1X;
    /* 0xF4 */ f32 mTexOffs1Y;
    /* 0xF8 */ JUTTexture* mWaterTex;
    /* 0xFC */ JUTTexture* mWaterEnvTex;
    /* 0x100 */ GXColor mTevReg1Front;
    /* 0x104 */ GXColor mTevReg1Back;
    /* 0x108 */ bool mUseDisplayList;
    u8 _109;
    u8 _10A;
    u8 _10B;
    /* 0x10C */ u32 mDispListFaceLen;
    /* 0x110 */ u8* mDispListFace;
    /* 0x114 */ u32 mDispListBackLen;
    /* 0x118 */ u8* mDispListBack;
    /* 0x11C */ bool mAlwaysUseRealDrawing;
    /* 0x11D */ bool mIsCameraInside;
    /* 0x11E */ bool mIsStartPosCamera;
    /* 0x11F */ bool mIsTearDrop;
    /* 0x120 */ bool mEnableStartPosCameraSwitch;
};
