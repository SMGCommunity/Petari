#pragma once

#include "Game/Boss/BossKameckAction.hpp"
#include "Game/LiveActor/LiveActor.hpp"

class ActorCameraInfo;
class BossKameckDemoPosition;

class BossKameckBattleDemo : public BossKameckAction {
public:
    BossKameckBattleDemo(BossKameck*, const JMapInfoIter&);

    /* 0x0C */ virtual void init();
    /* 0x10 */ virtual void appear();
    /* 0x14 */ virtual void kill();

    void startTryDemo(const char*, const Nerve*);
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

    /* 0x10 */ BossKameckDemoPosition* mDemoPos;
    /* 0x14 */ const Nerve* mDemoNerve;
    /* 0x18 */ const char* mCurDemoName;
};

class BossKameckDemoPosition : public LiveActor {
public:
    BossKameckDemoPosition();

    /* 0x0C */ virtual void init(const JMapInfoIter&);

    /* 0x8C */ ActorCameraInfo* mCameraInfo;
};
