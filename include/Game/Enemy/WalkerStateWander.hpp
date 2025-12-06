#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/System/NerveExecutor.hpp"

class WalkerStateParam;
class TerritoryMover;

class WalkerStateWanderParam {
public:
    WalkerStateWanderParam();

    s32 _0;
    s32 _4;
    f32 _8;
    f32 _C;
    f32 _10;
};

class WalkerStateWander : public ActorStateBase< LiveActor > {
public:
    WalkerStateWander(LiveActor*, TVec3f*, WalkerStateParam*, WalkerStateWanderParam*);

    virtual ~WalkerStateWander();
    virtual void appear();

    void setWanderCenter(const TVec3f&);
    void exeWait();
    void exeWalk();

    LiveActor* mParent;  // 0x0C
    TVec3f* _10;
    TerritoryMover* mTerritoryMover;       // 0x14
    WalkerStateParam* mStateParam;         // 0x18
    WalkerStateWanderParam* mWanderParam;  // 0x1C
};
