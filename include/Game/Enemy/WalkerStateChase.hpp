#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"
#include "Game/LiveActor/LiveActor.hpp"

class WalkerStateParam;

class WalkerStateChaseParam {
public:
    WalkerStateChaseParam();

    /* 0x00 */ f32 mChaseSpeed;
    /* 0x04 */ s32 mChaseTime;
    /* 0x08 */ s32 mForceChaseEndTime;
    /* 0x0C */ f32 mTurnMaxRateDegree;
    /* 0x10 */ s32 mChaseEndWaitTime;
};

class WalkerStateChase : public ActorStateBase< LiveActor > {
public:
    WalkerStateChase(LiveActor* pHost, TVec3f* pDirection, WalkerStateParam* pStateParam, WalkerStateChaseParam* pChaseParam);

    virtual void appear();

    void exeStart();
    void exeEnd();
    bool isRunning() const;

    /* 0x10 */ WalkerStateParam* mStateParam;
    /* 0x14 */ WalkerStateChaseParam* mChaseParam;
    /* 0x18 */ TVec3f* mDirection;
};
