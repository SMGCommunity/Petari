#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"
#include "Game/LiveActor/LiveActor.hpp"

class WalkerStateParam;

class WalkerStateFindPlayerParam {
public:
    WalkerStateFindPlayerParam();

    /* 0x0 */ u32 mJumpStartStep;
    /* 0x4 */ f32 mJumpVelocity;
    /* 0x8 */ f32 mTurnMaxRateDegree;
};

class WalkerStateFindPlayer : public ActorStateBase< LiveActor > {
public:
    WalkerStateFindPlayer(LiveActor* pHost, TVec3f* pPosition, WalkerStateParam* pStateParam, WalkerStateFindPlayerParam* pFindPlayerParam);

    virtual void appear();

    void exeFind();
    void exeFindJumpStart();
    void exeFindJump();
    void exeFindJumpEnd();
    bool isInSightPlayer() const;
    bool isFindJumpBegin() const;
    bool isLandStart() const;

    /* 0x10 */ TVec3f* mDirection;
    /* 0x14 */ WalkerStateParam* mStateParam;
    /* 0x18 */ WalkerStateFindPlayerParam* mFindPlayerParam;
};
