#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Map/WaterInfo.hpp"
#include <revolution.h>

class JUTTexture;
class OceanSpherePlane;
class OceanSpherePlaneEdge;
class OceanSpherePoint;

class OceanSphere : public LiveActor {
public:
    OceanSphere(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual void control();
    virtual void draw() const;

    bool isInWater(const TVec3f&) const;
    bool calcWaterInfo(const TVec3f&, const TVec3f&, WaterInfo*) const;
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

    f32 mRadius;                       // 0x8C
    f32 mRadiusTarget;                 // 0x90
    s32 mPointCount;                   // 0x94
    OceanSpherePoint* mAxisPointPX;    // 0x98
    OceanSpherePoint* mAxisPointNX;    // 0x9C
    OceanSpherePoint* mAxisPointPY;    // 0xA0
    OceanSpherePoint* mAxisPointNY;    // 0xA4
    OceanSpherePoint* mAxisPointPZ;    // 0xA8
    OceanSpherePoint* mAxisPointNZ;    // 0xAC
    OceanSpherePlane* mPlaneLeftUpper;   // 0xB0
    OceanSpherePlane* mPlaneRightUpper;  // 0xB4
    OceanSpherePlane* mPlaneLeftLower;   // 0xB8
    OceanSpherePlane* mPlaneRightLower;  // 0xBC
    OceanSpherePlaneEdge* mEdge0;        // 0xC0
    OceanSpherePlaneEdge* mEdge1;        // 0xC4
    OceanSpherePlaneEdge* mEdge2;        // 0xC8
    OceanSpherePlaneEdge* mEdge3;        // 0xCC
    OceanSpherePlaneEdge* mEdge4;        // 0xD0
    OceanSpherePlaneEdge* mEdge5;        // 0xD4
    OceanSpherePlaneEdge* mEdge6;        // 0xD8
    OceanSpherePlaneEdge* mEdge7;        // 0xDC
    f32 mWaveTime1;                    // 0xE0
    f32 mWaveTime2;                    // 0xE4
    f32 mTexOffs0X;                    // 0xE8
    f32 mTexOffs0Y;                    // 0xEC
    f32 mTexOffs1X;                    // 0xF0
    f32 mTexOffs1Y;                    // 0xF4
    JUTTexture* mWaterTex;             // 0xF8
    JUTTexture* mWaterEnvTex;          // 0xFC
    GXColor mTevReg1Front;             // 0x100
    GXColor mTevReg1Back;              // 0x104
    bool mUseDisplayList;              // 0x108
    u8 _109;
    u8 _10A;
    u8 _10B;
    u32 mDispListFaceLen;              // 0x10C
    u8* mDispListFace;                 // 0x110
    u32 mDispListBackLen;              // 0x114
    u8* mDispListBack;                 // 0x118
    bool mAlwaysUseRealDrawing;        // 0x11C
    bool mIsCameraInside;              // 0x11D
    bool mIsStartPosCamera;            // 0x11E
    bool mIsTearDrop;                  // 0x11F
    bool mEnableStartPosCameraSwitch;  // 0x120
};
