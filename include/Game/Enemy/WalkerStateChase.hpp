#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/System/NerveExecutor.hpp"

class WalkerStateParam;

class WalkerStateChaseParam {
public:
    WalkerStateChaseParam();

    f32 _0;
    u32 _4;
    u32 _8;
    f32 _C;
    u32 _10;
};

class WalkerStateChase : public ActorStateBase< LiveActor > {
public:
    WalkerStateChase(LiveActor*, TVec3f*, WalkerStateParam*, WalkerStateChaseParam*);

    virtual ~WalkerStateChase();
    virtual void appear();

    void exeStart();
    void exeEnd();
    bool isRunning() const;

    LiveActor* mParentActor;             // 0xC
    WalkerStateParam* mStateParam;       // 0x10
    WalkerStateChaseParam* mChaseParam;  // 0x14
    TVec3f* _18;
};
