#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class JUTTexture;
class OceanSpherePoint;
class WaterInfo;

class OceanSpherePlane {
public:
    OceanSpherePlane(s32 pointCount, const TVec3f* pCenter, const TVec3f& rAxis1, const TVec3f& rAxis2, const TVec2f& rTex1, const TVec2f& rTex2,
                     const TVec2f& rTex3);

    void update(f32 radius, f32 wave1Time, f32 wave2Time);
    OceanSpherePoint* getPoint(int col, int row) const {
        return mPoints[(mAxisPointCount * row) + col];
    }

    /* 0x00 */ s32 mGridPointCount;
    /* 0x04 */ s32 mAxisPointCount;
    /* 0x08 */ OceanSpherePoint** mPoints;
};

class OceanSpherePlaneEdge {
public:
    OceanSpherePlaneEdge(s32 pointCount, const TVec3f* pCenter, const TVec3f& rAxis1, const TVec3f& rAxis2, const TVec2f& rTex1, const TVec2f& rTex2);

    void update(f32 radius, f32 wave1Time, f32 wave2Time);

    /* 0x00 */ s32 mPointCount;
    /* 0x04 */ OceanSpherePoint** mPoints;
};

class OceanSphere : public LiveActor {
public:
    OceanSphere(const char* pName);

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
    OceanSpherePoint* getPlanePointLeftUpper(int row, int col) const;
    OceanSpherePoint* getPlanePointRightUpper(int row, int col) const;
    OceanSpherePoint* getPlanePointLeftLower(int row, int col) const;
    OceanSpherePoint* getPlanePointRightLower(int row, int col) const;
    void loadMaterialFace() const;
    void loadMaterialBack() const;
    void drawSphere(bool useEnvMap, bool useGD) const;
    void sendVertex(const OceanSpherePoint* pPoint, bool useEnvMap, bool useGD) const;

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
    /* 0x109 */ u8 _109;
    /* 0x10A */ u8 _10A;
    /* 0x10B */ u8 _10B;
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
