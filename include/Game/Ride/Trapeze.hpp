#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Ride/SwingRopePoint.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

class Trapeze : public LiveActor {
public:
    Trapeze(const char*);

    virtual void init(const JMapInfoIter& rIter);
    virtual void draw() const;
    virtual void exeFree();  // ????
    virtual void updateHitSensor(HitSensor* pSensor);
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeStop();
    void exeFreeInvalid();
    void exeSwingWait();
    void exeSwingSlideDownStart();
    void exeSwingSlideDown();
    void exeSwingFrontStart();
    void exeSwingFrontEnd();
    void exeSwingBackStart();
    void exeSwingBackEnd();

    bool tryJump();

    void updateStick(const TVec3f&, f32);
    void updateStickMtx();
    void updateHangPoint();
    bool updateBind();
    bool updateSwing();
    bool updateSlideDown();

    bool isStartSwingFront() const;
    bool isStartSwingBack() const;

    void drawRope(const TVec3f&, const TVec3f&, const TVec3f&, const TVec3f&, f32, f32) const;

    /* 0x8C */ TVec3f mSide;
    /* 0x98 */ TVec3f mUp;
    /* 0xA4 */ TVec3f mFront;
    /* 0xB0 */ f32 mRopeLength;
    /* 0xB4 */ SwingRopePoint* mSwingPoint;
    /* 0xB8 */ PartsModel* mTrapezeModel;
    /* 0xBC */ TPos3f mPosMtx;
    /* 0xEC */ LiveActor* mRider;
    /* 0xF0 */ SwingRopePoint* mGrabPoint;
    /* 0xF4 */ f32 mGrabCoord;
    /* 0xF8 */ f32 mSwingVel;
    /* 0xFC */ bool mSwingReverse;
    /* 0xFD */ bool mIsSwingFront;
    /* 0xFE */ bool mIsSwingBack;
    /* 0xFF */ bool mWasSwingFront;
    /* 0x100 */ bool mWasSwingBack;
    /* 0x104 */ ActorCameraInfo* mCameraInfo;
    /* 0x108 */ s32 mPrevSoundLvl;
};

class TrapezeRopeDrawInit : public NameObj {
public:
    TrapezeRopeDrawInit(const char*);

    virtual void initDraw() const;

    /* 0x0C */ JUTTexture* mTexture;
};

// these have to be header bodies because for some reason they are actually linked in NameObjFactory
namespace NrvTrapeze {
    NEW_NERVE(TrapezeNrvStop, Trapeze, Stop);
    NEW_NERVE(TrapezeNrvFree, Trapeze, Free);
    NEW_NERVE(TrapezeNrvFreeInvalid, Trapeze, FreeInvalid);
    NEW_NERVE(TrapezeNrvSwingWait, Trapeze, SwingWait);
    NEW_NERVE(TrapezeNrvSwingSlideDownStart, Trapeze, SwingSlideDownStart);
    NEW_NERVE(TrapezeNrvSwingSlideDown, Trapeze, SwingSlideDown);
    NEW_NERVE(TrapezeNrvSwingFrontStart, Trapeze, SwingFrontStart);
    NEW_NERVE(TrapezeNrvSwingFrontEnd, Trapeze, SwingFrontEnd);
    NEW_NERVE(TrapezeNrvSwingBackStart, Trapeze, SwingBackStart);
    NEW_NERVE(TrapezeNrvSwingBackEnd, Trapeze, SwingBackEnd);
    NEW_NERVE(TrapezeNrvSlowSwingFrontStart, Trapeze, SwingFrontStart);
    NEW_NERVE(TrapezeNrvSlowSwingFrontEnd, Trapeze, SwingFrontEnd);
    NEW_NERVE(TrapezeNrvSlowSwingBackStart, Trapeze, SwingBackStart);
    NEW_NERVE(TrapezeNrvSlowSwingBackEnd, Trapeze, SwingBackEnd);
}  // namespace NrvTrapeze
