#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Map/WaterInfo.hpp"
#include "Game/Map/WaterPoint.hpp"

class AudSoundObject;
class OceanRingDrawer;
class OceanRingPipe;
class OceanRingBloomDrawer;

class OceanRing : public LiveActor {
public:
    OceanRing(const char*);

    virtual ~OceanRing();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void movement();
    virtual void draw() const;
    virtual void startClipped();
    virtual void endClipped();

    bool isInWater(const TVec3f&) const;
    bool calcWaterInfo(const TVec3f&, const TVec3f&, WaterInfo*) const;
    f32 calcNearestPos(const TVec3f&, TVec3f*, TVec3f*, TVec3f*) const;
    f32 calcWaveHeight(const TVec3f&, f32, TVec3f*) const;
    void calcStreamVec(const TVec3f&, f32, TVec3f*) const;
    void initPoints();
    void updatePoints();
    void updatePointsInLine(s32, s32, s32, s32);
    f32 calcCurrentWidthRate(f32) const;
    f32 calcCurrentFlowSpeedRate(f32) const;
    void calcClippingBox();
    WaterPoint* getPoint(int) const;
    WaterPoint* getPoint(int, int) const NO_INLINE;

    s32 mWaterPointNum;         // 0x8C
    s32 mSegCount;              // 0x90
    s32 mStride;                // 0x94
    WaterPoint** mWaterPoints;  // 0x98
    f32 mWidthMax;              // 0x9C
    s32 mObjArg1;               // 0xA0
    f32 mWaveTheta0;            // 0xA4
    f32 mWaveTheta1;            // 0xA8
    f32 mWaveHeight1;           // 0xAC
    f32 mWaveHeight2;           // 0xB0
    bool mIsClipped;            // 0xB4
    f32 mNearPosToPlayer;          // 0xB8
    TVec3f mNearestPos;            // 0xBC
    TVec3f mNearestDir;            // 0xC8
    OceanRingDrawer* mRingDrawer;  // 0xD4
    TBox3f mBox;                   // 0xD8
    TBox3f mClippingBox;           // 0xF0
    TVec3f _108;
    TVec3f mNearestToWatchCam;           // 0x114
    AudSoundObject* mSoundObj1;          // 0x120
    TVec3f mNerarestToCam;               // 0x124
    AudSoundObject* mSoundObj2;          // 0x130
    OceanRingPipe* mOceanRingPipe;       // 0x134
    OceanRingBloomDrawer* mBloomDrawer;  // 0x138
};
