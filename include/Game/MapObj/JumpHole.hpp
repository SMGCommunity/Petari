#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class BindCone;

class JumpHole : public LiveActor {
public:
    JumpHole(const char* pName);

    virtual ~JumpHole() {
    }

    virtual void init(const JMapInfoIter& rIter);
    virtual void initAfterPlacement();
    virtual void control();
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeWait();
    void exeSetCenter();
    void exeSetUp();
    void exeRailMove();
    void bindHole();
    void initParabola(const TVec3f& rPosition);

    /* 0x8C */ BindCone* mCone;
    /* 0x90 */ HitSensor* mBoundSensor;
    /* 0x94 */ f32 mLaunchTime;
    /* 0x98 */ TVec3f mLaunchStart;
    /* 0xA4 */ TVec3f mBindVelocity;
    /* 0xB0 */ TVec3f mHoleGravity;
    /* 0xBC */ TVec3f mLaunchGravity;
    /* 0xC8 */ TVec3f mLaunchDirection;
    /* 0xD4 */ u32 mLaunchDuration;
    /* 0xD8 */ f32 mLaunchDistance;
    /* 0xDC */ f32 mLaunchQuadratic;
    /* 0xE0 */ f32 mLaunchLinear;
    /* 0xE4 */ s32 mSettledFrames;
};
