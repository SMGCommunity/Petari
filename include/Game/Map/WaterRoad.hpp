#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/JMapInfo.hpp"
#include <JSystem/JGeometry.hpp>

class JUTTexture;
class WaterRoadModelInfo;
class AudSoundObject;
class ActorCameraInfo;

class WaterRoad : public LiveActor {
public:
    WaterRoad(const char*);

    virtual void init(const JMapInfoIter& rIter);
    virtual void appear();
    virtual void movement();
    virtual void draw() const;
    virtual void updateHitSensor(HitSensor* pSensor);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeWait();
    void exeWaitInvalid();
    void exeRideStart();
    void exeRideWait();
    void exeRideSpin();
    void exeDemoStart();
    void exeDemoGrowUp();
    void exeDemoWaitEnd();
    void exeDisappear();

    bool updateRide();
    void updateDemo(f32);

    /* 0x08C */ s32 _8C;  // arg0, unused?
    /* 0x090 */ WaterRoadModelInfo* mModelInfoHigh;
    /* 0x094 */ WaterRoadModelInfo* mModelInfoLow;
    /* 0x098 */ f32 mRadius;
    /* 0x09C */ u8 mAlpha;
    /* 0x0A0 */ TVec3f mBaseUp;
    /* 0x0AC */ TPos3f mBaseMtx;
    /* 0x0DC */ LiveActor* mRider;
    /* 0x0E0 */ TPos3f mRiderMtx;
    /* 0x110 */ TVec2f mTexUV0;
    /* 0x118 */ TVec2f mTexUV1;
    /* 0x120 */ TVec2f mTexUV2;
    /* 0x128 */ JUTTexture* mTexture;
    /* 0x12C */ JUTTexture* mIndirect;
    /* 0x130 */ JUTTexture* mEnvMap;
    /* 0x134 */ TBox3f mBoundingBox;
    /* 0x14C */ TVec3f mClippingCenter;
    /* 0x158 */ TVec3f mCamRailNearestPos;
    /* 0x164 */ AudSoundObject* mSoundObj;
    /* 0x168 */ ActorCameraInfo* mCameraInfo;
};

class WaterRoadModelInfo {
public:
    WaterRoadModelInfo(WaterRoad*, bool);

    void initPoints(WaterRoad*);
    void initDisplayList();
    void sendGD() const;
    void drawGD() const;
    void drawDirect(const WaterRoad*) const;
    s32 calcDemoDrawPointNum(const WaterRoad*) const;
    void loadMaterialHigh(const WaterRoad*) const;
    void loadMaterialLow() const;

    s32 calcPointIndex(int i, int j) const {
        return j + i * mNumLoopPoints;
    }

    /* 0x00 */ bool mIsLow;
    /* 0x04 */ s32 mNumPoints;
    /* 0x08 */ s32 mNumLinePoints;
    /* 0x0C */ s32 mNumLoopPoints;
    /* 0x10 */ TVec3f* mPoints;
    /* 0x14 */ TVec3s* mNormals;
    /* 0x18 */ f32* mRailCoords;
    /* 0x1C */ u32 mDispListLength;
    /* 0x20 */ u8* mDispList;
};
