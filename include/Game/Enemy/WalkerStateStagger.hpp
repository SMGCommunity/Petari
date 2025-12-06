#pragma once

#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/System/NerveExecutor.hpp"

class WalkerStateParam;
class WalkerStateStaggerParam;

class WalkerStateStaggerParam {
public:
    WalkerStateStaggerParam();

    f32 _0;
    f32 _4;
    f32 _8;
    u32 _C;
    u32 _10;
    u32 _14;
    u32 _18;
    u32 _1C;
    f32 _20;
    f32 _24;
    f32 _28;
    f32 _2C;
};

class WalkerStateStagger : public ActorStateBase< LiveActor > {
public:
    WalkerStateStagger(LiveActor*, TVec3f*, WalkerStateParam*, WalkerStateStaggerParam*);

    virtual ~WalkerStateStagger();
    virtual void appear();

    void setPunchDirection(HitSensor*, HitSensor*);
    void exeStagger();
    void exeStaggerEnd();
    void reboundWall();
    bool isEnableKick() const;
    bool isUpsideDown() const;
    bool isStaggerStart() const;
    bool isSwooning(s32) const;
    bool isSpinning(s32, s32) const;
    bool isRecoverStart() const;

    LiveActor* mParent;                      // 0x0C
    WalkerStateParam* mStateParam;           // 0x10
    WalkerStateStaggerParam* mStaggerParam;  // 0x14
    TVec3f _18;
    TVec3f* _24;
};
