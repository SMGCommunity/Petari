#pragma once

#include "Game/LiveActor/ActorCameraInfo.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class CreeperPoint {
public:
    CreeperPoint(const TVec3f&, const TVec3f&, const CreeperPoint*);

    void updateFree();
    void updateBend(bool, const TVec3f&, f32, f32);
    void updateLocalAxis();

    /* 0x00 */ TVec3f mPosition;
    /* 0x0C */ TVec3f mNeutralPos;
    /* 0x18 */ TVec3f mVelocity;
    /* 0x24 */ TVec3f mSide;
    /* 0x30 */ TVec3f mUp;
    /* 0x3C */ TVec3f mFront;
    /* 0x48 */ TVec3f mProjection;
    /* 0x54 */ const CreeperPoint* mPrevPoint;
};

class Creeper : public LiveActor {
public:
    Creeper(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual void draw() const;
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeFree();
    void exeFreeInvalid();
    void exeHangStart();
    void exeHangUp();
    void exeHangDown();

    void initPoints();
    void updatePoints();
    void bendPoints();

    bool tryHangUp();
    bool tryJump();
    bool updateHangUp(f32);

    void calcAndGetCurrentInfo(TVec3f*, TVec3f*) const;

    void loadMaterial() const;
    const CreeperPoint* getHeadPoint() const;

    inline const CreeperPoint* getPoint(s32 idx) const { return mPoints[idx]; }

    /* 0x8C */ s32 mNumPoints;
    /* 0x90 */ CreeperPoint** mPoints;
    /* 0x94 */ LiveActor* mRider;
    /* 0x98 */ TVec3f mFront;
    /* 0xA4 */ TVec3f mUp;
    /* 0xB0 */ f32 mClimbSpeed;
    /* 0xB4 */ f32 mCoord;  // this is a ratio along stalk length
    /* 0xB8 */ f32 mBendFactorNormal;
    /* 0xBC */ f32 mBendFactorBee;
    /* 0xC0 */ TVec3f mBendDirection;
    /* 0xCC */ f32 mLaunchHorizontalSpeed;
    /* 0xD0 */ f32 mLaunchVerticalSpeed;
    /* 0xD4 */ ActorCameraInfo* mCameraInfo;
    /* 0xD8 */ TVec3f mClippingCenter;
    /* 0xE4 */ JUTTexture* mTexture;
    /* 0xE8 */ PartsModel* mFlowerModel;
    /* 0xEC */ PartsModel* mLeafModel;
    /* 0xF0 */ TPos3f mTopMtx;
};
