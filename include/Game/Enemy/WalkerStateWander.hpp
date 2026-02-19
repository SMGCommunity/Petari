#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"
#include "Game/LiveActor/LiveActor.hpp"

class WalkerStateParam;
class TerritoryMover;

class WalkerStateWanderParam {
public:
    WalkerStateWanderParam();

    /* 0x00 */ s32 mWaitTime;
    /* 0x04 */ s32 mWalkTime;
    /* 0x08 */ f32 mSpeed;
    /* 0x0C */ f32 mTurnDegree;
    /* 0x10 */ f32 mTargetDistance;
};

class WalkerStateWander : public ActorStateBase< LiveActor > {
public:
    WalkerStateWander(LiveActor*, TVec3f*, WalkerStateParam*, WalkerStateWanderParam*);

    virtual void appear();

    void setWanderCenter(const TVec3f&);
    void exeWait();
    void exeWalk();

    /* 0x10 */ TVec3f* mDirection;
    /* 0x14 */ TerritoryMover* mTerritoryMover;
    /* 0x18 */ WalkerStateParam* mStateParam;
    /* 0x1C */ WalkerStateWanderParam* mWanderParam;
};
