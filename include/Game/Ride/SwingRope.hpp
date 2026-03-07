#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class SwingRopePoint;
class SledRopePoint;

class SwingRope : public LiveActor {
public:
    SwingRope(const char*);

    virtual void init(const JMapInfoIter& rIter);
    virtual void draw() const;
    virtual void updateHitSensor(HitSensor* pSensor);
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeStop();
    void exeFree();
    void exeFreeInvalid();
    void exeBindSlideDownStart();
    void exeBindSlideDown();
    void exeBindStretch();
    void exeBindLoose();

    void initPoints();

    f32 calcPointNo(f32) const;
    f32 calcFriction(s32) const;

    void addPointGravity();
    void restrictPointToTail(s32, const TVec3f*, f32);
    void restrictPointToHead(s32, const TVec3f&, f32);

    bool isAllPointsStop() const;
    bool isStretched() const;
    bool tryJump();
    void updateHangPoint();
    void updateRideMtx();
    void updateFootPos();
    bool updateStretch();
    bool updateLoose();
    void updateLooseUpperPoints();
    bool updateSlideDown();
    void updateStretchHangUpperPoints();
    void updateHangLowerPoints();
    void updateHangLowerPointPos(s32);

    void drawStop() const;
    void drawFree() const;
    void drawBind() const;

    /* 0x8C */ TVec3f mBasePos;
    /* 0x98 */ f32 mRopeLength;
    /* 0x9C */ bool mIsStretched;
    /* 0xA0 */ s32 mStretchTime;  // Time since start of stretch condition
    /* 0xA4 */ s32 mAccelTime;    // Time since start of accel
    /* 0xA8 */ s32 mNumPoints;
    /* 0xAC */ SwingRopePoint** mPoints;
    /* 0xB0 */ LiveActor* mRider;
    /* 0xB4 */ TPos3f mRideMtx;
    /* 0xE4 */ f32 mGrabCoord;
    /* 0xE8 */ f32 mGrabPointNum;
    /* 0xEC */ f32 mSlideSpeed;
    /* 0xF0 */ SledRopePoint* mSledPoint;
    /* 0xF4 */ ActorCameraInfo* mCameraInfo;
    /* 0xF8 */ f32 mFootCoord;
    /* 0xFC */ f32 mFootPointNum;
    /* 0x100 */ f32 mGrabToFootDist;
    /* 0x104 */ TVec3f mFootPos;
    /* 0x110 */ TVec3f mShadowDropPos;
    /* 0x11C */ f32 mShadowDropOffset;
    /* 0x120 */ s32 mPrevSoundLvl;
};

class SwingRopeGroup : public NameObj {
public:
    SwingRopeGroup(const char*);

    virtual void initDraw() const;

    /* 0x0C */ JUTTexture* mTexture;
};
