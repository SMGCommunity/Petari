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

    /* 0x08C */ s32 mWaterPointNum;
    /* 0x090 */ s32 mSegCount;
    /* 0x094 */ s32 mStride;
    /* 0x098 */ WaterPoint** mWaterPoints;
    /* 0x09C */ f32 mWidthMax;
    /* 0x0A0 */ s32 mObjArg1;
    /* 0x0A4 */ f32 mWaveTheta0;
    /* 0x0A8 */ f32 mWaveTheta1;
    /* 0x0AC */ f32 mWaveHeight1;
    /* 0x0B0 */ f32 mWaveHeight2;
    /* 0x0B4 */ bool mIsClipped;
    /* 0x0B8 */ f32 mNearPosToPlayer;
    /* 0x0BC */ TVec3f mNearestPos;
    /* 0x0C8 */ TVec3f mNearestDir;
    /* 0x0D4 */ OceanRingDrawer* mRingDrawer;
    /* 0x0D8 */ TBox3f mBox;
    /* 0x0F0 */ TBox3f mClippingBox;
    /* 0x108 */ TVec3f _108;
    /* 0x114 */ TVec3f mNearestToWatchCam;
    /* 0x120 */ AudSoundObject* mSoundObj1;
    /* 0x124 */ TVec3f mNerarestToCam;
    /* 0x130 */ AudSoundObject* mSoundObj2;
    /* 0x134 */ OceanRingPipe* mOceanRingPipe;
    /* 0x138 */ OceanRingBloomDrawer* mBloomDrawer;
};
