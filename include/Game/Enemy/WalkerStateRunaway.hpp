#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/System/NerveExecutor.hpp"

class WalkerStateRunawayParam {
public:
    WalkerStateRunawayParam();

    const char* _0;
    const char* _4;
    const char* _8;
    f32 _C;
    f32 _10;
    f32 _14;
    f32 _18;
    s32 _1C;
    f32 _20;
    f32 _24;
    f32 _28;
    f32 _2C;
    f32 _30;
    u32 _34;
    f32 _38;
    f32 _3C;
    f32 _40;
    f32 _44;
    s32 _48;
    f32 _4C;
    f32 _50;
    f32 _54;
};

class WalkerStateRunaway : public ActorStateBase< LiveActor > {
public:
    WalkerStateRunaway(LiveActor* pActor, TVec3f* a2, WalkerStateRunawayParam* pParam);

    virtual ~WalkerStateRunaway();
    virtual void appear();

    bool tryRunaway();
    bool tryWait();
    bool tryWallJump();
    void exeWait();
    void exeRunaway();
    void exeWallJump();
    bool isRunning() const;

    LiveActor* mParentActor;          // 0xC
    WalkerStateRunawayParam* mParam;  // 0x10
    TVec3f* _14;
    s32 _18;
    f32 _1C;
};
