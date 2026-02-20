#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/ActorMovementUtil.hpp"

class WalkerStateRunawayParam {
public:
    WalkerStateRunawayParam();

    /* 0x00 */ const char* mWaitAction;
    /* 0x04 */ const char* mRunawayAction;
    /* 0x08 */ const char* mJumpAction;
    /* 0x0C */ f32 mGroundAccel;
    /* 0x10 */ f32 mAirAccel;
    /* 0x14 */ f32 _14;  // mGroundFriction? - fric if ._18 <= _1C
    /* 0x18 */ f32 _18;  // mAirFriction? - fric if ._18 > _1C
    /* 0x1C */ s32 _1C;  // step?
    /* 0x20 */ f32 mTurnMaxRateDegree;
    /* 0x24 */ f32 mRunawayDistance;
    /* 0x28 */ f32 mWaitDistance;
    /* 0x2C */ f32 mRunawayTurnRateMaxDegree;
    /* 0x30 */ f32 mRunawayTurnRateMinDegree;
    /* 0x34 */ s32 mRunawayTurnTime;
    /* 0x38 */ f32 mPlayerFrontLineLength;
    /* 0x3C */ f32 mRunawayBckRatio;
    /* 0x40 */ f32 mMinRunawayBckRate;
    /* 0x44 */ f32 mMaxRunawayBckRate;
    /* 0x48 */ s32 mWallJumpTime;
    /* 0x4C */ f32 mWallReboundPower;
    /* 0x50 */ f32 mWallJumpPowerH;
    /* 0x54 */ f32 mWallJumpPowerV;
};

class WalkerStateRunaway : public ActorStateBase< LiveActor > {
public:
    WalkerStateRunaway(LiveActor* pHost, TVec3f* pDirection, WalkerStateRunawayParam* pRunawayParam);

    virtual void appear();

    bool tryRunaway();
    bool tryWait();
    bool tryWallJump();
    void exeWait();
    void exeRunaway();
    void exeWallJump();
    bool isRunning() const;

    bool isInWaitRange(f32 range) const {
        return !MR::isNearPlayer(getHost(), range);
    }

    WalkerStateRunawayParam* getParam() const {
        return mRunawayParam;
    }

    // TODO: what are these values?
    bool check18() const {
        return _18 < mRunawayParam->_1C;
    }

    /* 0x10 */ WalkerStateRunawayParam* mRunawayParam;
    /* 0x14 */ TVec3f* mDirection;
    /* 0x18 */ s32 _18;
    /* 0x1C */ f32 mRunawaySpeed;
};
