#pragma once

#include "Game/Boss/BossKameckAction.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/ActorCameraInfo.hpp"

class BossKameck;

class BossKameckDemoPosition : public LiveActor {
public:
    BossKameckDemoPosition();

    virtual ~BossKameckDemoPosition();
    virtual void init(const JMapInfoIter &);

    ActorCameraInfo* mCameraInfo;           // 0x8C
};

class BossKameckBattleDemo : public BossKameckAction {
public:
    BossKameckBattleDemo(BossKameck *, const JMapInfoIter &);

    virtual ~BossKameckBattleDemo();
    virtual void init();
    virtual void appear();
    virtual void kill();

    void startTryDemo(const char *, const Nerve *);
    void exeTryStartDemo();
    void startDemoAppearVs1();
    void startDemoAppearVs2();
    void startDemoPowerUpVs1();
    void startDemoPowerUpVs2();
    void startDemoDownVs1();
    void startDemoDownVs2();
    void exeWaitAppearVs1();
    void exeAppearVs1();
    void exeWaitAppearVs2();
    void exeAppearVs2();
    void exePowerUpVs1();
    void exePowerUpVs2();
    void exeDownVs1();
    void exeDownVs2();
    void updateCastPose();

    BossKameckDemoPosition* mDemoPos;           // 0x10
    const Nerve* mDemoNerve;                    // 0x14
    const char* mCurDemoName;                   // 0x18
};

namespace NrvBossKamecBattleDemo {
    NERVE_DECL_EXE(BossKameckBattleDemoNrvTryStartDemo, BossKameckBattleDemo, TryStartDemo);
    NERVE_DECL_EXE(BossKameckBattleDemoNrvWaitAppearVs1, BossKameckBattleDemo, WaitAppearVs1);
    NERVE_DECL_EXE(BossKameckBattleDemoNrvAppearVs1, BossKameckBattleDemo, AppearVs1);
    NERVE_DECL_EXE(BossKameckBattleDemoNrvPowerUpVs1, BossKameckBattleDemo, PowerUpVs1);
    NERVE_DECL_EXE(BossKameckBattleDemoNrvDownVs1, BossKameckBattleDemo, DownVs1);
    NERVE_DECL_EXE(BossKameckBattleDemoNrvWaitAppearVs2, BossKameckBattleDemo, WaitAppearVs2);
    NERVE_DECL_EXE(BossKameckBattleDemoNrvAppearVs2, BossKameckBattleDemo, AppearVs2);
    NERVE_DECL_EXE(BossKameckBattleDemoNrvPowerUpVs2, BossKameckBattleDemo, PowerUpVs2);
    NERVE_DECL_EXE(BossKameckBattleDemoNrvDownVs2, BossKameckBattleDemo, DownVs2);
};