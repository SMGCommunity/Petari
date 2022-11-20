#pragma once

#include "Game/LiveActor/LiveActor.h"
#include "Game/Map/WaterInfo.h"
#include "Game/Map/WaterPoint.h"

class AudSoundObject;
class OceanRingDrawer;
class OceanRingPipe;
class OceanRingBloomDrawer;

namespace {
    static f32 sEdgePointNum = 2.0f;
};

class OceanRing : public LiveActor {
public:
    OceanRing(const char *);

    virtual ~OceanRing();
    virtual void init(const JMapInfoIter &);
    virtual void initAfterPlacement();
    virtual void movement();
    virtual void draw() const;
    virtual void startClipped();
    virtual void endClipped();

    bool isInWater(const TVec3f &) const;
    bool calcWaterInfo(const TVec3f &, const TVec3f &, WaterInfo *);
    f32 calcNearestPos(const TVec3f &, TVec3f *, TVec3f *, TVec3f *) const;
    f32 calcWaveHeight(const TVec3f &, f32, TVec3f *);
    void calcStreamVec(const TVec3f &, f32, TVec3f *);
    void initPoints();
    void updatePoints();
    void updatePointsInLine(s32, s32, s32, s32);
    f32 calcCurrentWidthRate(f32) const;
    f32 calcCurrentFlowSpeedRate(f32) const;
    void calcClippingBox();
    WaterPoint* getPoint(int, int) const;

    s32 mWaterPointNum;                 // _8C
    u32 mSegCount;                      // _90
    s32 mStride;                        // _94
    WaterPoint** mWaterPoints;          // _98
    f32 mWidthMax;                      // _9C
    s32 mObjArg1;                       // _A0
    f32 mWaveTheta1;                    // _A4
    f32 mWaveTheta2;                    // _A8
    f32 mWaveHeight1;                   // _AC
    f32 mWaveHeight2;                   // _B0
    u8 _B4;
    u8 _B5;
    u8 _B6;
    u8 _B7;
    f32 mNearPosToPlayer;               // _B8
    TVec3f mNearestPos;                 // _BC
    TVec3f mNearestDir;                 // _C8
    OceanRingDrawer* mRingDrawer;       // _D4
    TBox3f mBox;                        // _D8
    TBox3f mClippingBox;                // _F0
    TVec3f _108;
    TVec3f mNearestToWatchCam;          // _114
    AudSoundObject* mSoundObj1;         // _120
    TVec3f mNerarestToCam;              // _124
    AudSoundObject* mSoundObj2;         // _130
    OceanRingPipe* mOceanRingPipe;      // _134
    OceanRingBloomDrawer* mBloomDrawer; // _138
};