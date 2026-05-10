#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"
#include "Game/LiveActor/LiveActor.hpp"

class WalkerStateParam;
class WalkerStateStaggerParam;

class WalkerStateStaggerParam {
public:
    WalkerStateStaggerParam();

    /* 0x00 */ f32 mPunchPowerH;
    /* 0x04 */ f32 mPunchPowerV;
    /* 0x08 */ f32 mPunchPowerSideH;
    /* 0x0C */ s32 mStopSceneFrames;
    /* 0x10 */ s32 mKickEnableStep;
    /* 0x14 */ s32 mStaggerTime;
    /* 0x18 */ s32 mRotateStartTime;
    /* 0x1C */ s32 mRotateEndTime;
    /* 0x20 */ f32 mStaggerFrontPower;
    /* 0x24 */ f32 mStaggerSidePower;
    /* 0x28 */ f32 mStaggerSideCircleRateDegree;
    /* 0x2C */ f32 mRotateRateDegree;
};

class WalkerStateStagger : public ActorStateBase< LiveActor > {
public:
    WalkerStateStagger(LiveActor* pHost, TVec3f* pDirection, WalkerStateParam* pStateParam, WalkerStateStaggerParam* pStaggerParam);

    virtual void appear();

    void setPunchDirection(HitSensor* pSender, HitSensor* pReceiver);
    void exeStagger();
    void exeStaggerEnd();
    void reboundWall();
    bool isEnableKick() const;
    bool isUpsideDown() const;
    bool isStaggerStart() const;
    bool isSwooning(s32 swoonStep) const;
    bool isSpinning(s32 spinStartStep, s32 spinEndStep) const;
    bool isRecoverStart() const;

    /* 0x10 */ WalkerStateParam* mStateParam;
    /* 0x14 */ WalkerStateStaggerParam* mStaggerParam;
    /* 0x18 */ TVec3f mVelH;
    /* 0x24 */ TVec3f* mDirection;
};
