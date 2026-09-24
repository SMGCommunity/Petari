#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class BindCone;

class BallOpener : public LiveActor {
public:
    BallOpener(const char* pName);

    virtual ~BallOpener();
    virtual void init(const JMapInfoIter& rIter);
    virtual void initAfterPlacement();
    virtual void control();
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeWait();
    void exeSetCenter();
    void exeOpen();
    void bindHole();

    /* 0x8C */ BindCone* mCone;
    /* 0x90 */ HitSensor* mBoundSensor;
    /* 0x94 */ TVec3f mHoleGravity;
    /* 0xA0 */ TVec3f _A0;
    /* 0xAC */ TVec3f mInitialVelocity;
    /* 0xB8 */ TVec3f mBindVelocity;
    /* 0xC4 */ s32 mSettledFrames;
};
