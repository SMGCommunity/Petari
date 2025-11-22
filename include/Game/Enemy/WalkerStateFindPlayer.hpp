#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/System/NerveExecutor.hpp"
#include "JSystem/JGeometry/TVec.hpp"

class WalkerStateParam;

class WalkerStateFindPlayerParam {
public:
    WalkerStateFindPlayerParam();

    u32 _0;
    f32 _4;
    f32 _8;
};

class WalkerStateFindPlayer : public ActorStateBase< LiveActor > {
public:
    WalkerStateFindPlayer(LiveActor*, TVec3f*, WalkerStateParam*, WalkerStateFindPlayerParam*);

    virtual ~WalkerStateFindPlayer();
    virtual void appear();

    void exeFind();
    void exeFindJumpStart();
    void exeFindJump();
    void exeFindJumpEnd();
    bool isInSightPlayer() const;
    bool isFindJumpBegin() const;
    bool isLandStart() const;

    LiveActor* mParent;  // 0x0C
    TVec3f* _10;
    WalkerStateParam* mStateParam;                 // 0x14
    WalkerStateFindPlayerParam* mFindPlayerParam;  // 0x18
};
