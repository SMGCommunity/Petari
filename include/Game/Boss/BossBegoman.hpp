#pragma once

#include "Game/Boss/BossBegomanHead.hpp"
#include "Game/Enemy/BegomanBaby.hpp"
#include "Game/Enemy/BegomanBase.hpp"
#include "Game/LiveActor/ActorCameraInfo.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/ParabolicPath.hpp"

class BegomanSpike;

class BossBegoman : public BegomanBase {
public:
    enum FollowerKind {
        FollowerKind_BabyFollower = 0,
        FollowerKind_SpikeFollower = 1,
        FollowerKind_BothFollower = 2,
    };

    BossBegoman(const char*);

    virtual void init(const JMapInfoIter& rIter);
    virtual void calcAnim();
    virtual void appear();
    virtual void kill();
    virtual void control();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual u32 getKind() const { return 0; };
    virtual bool onTouchElectric(const TVec3f&, const TVec3f&);
    virtual bool setNerveReturn();
    virtual const Nerve* getNerveWait();
    virtual void addVelocityOnPushedFromElectricRail(const TVec3f&, const TVec3f&);
    virtual bool requestAttack();

    void setStepBackNerve();

    void exePreDemoWait();
    void endPreDemoWait();
    void exeFirstContactDemo();

    void exeReady();
    void exeNoCalcWait();
    void endNoCalcWait();
    void exeWait();
    void exeSignAttack();
    void exePursue();
    void exeTurn();
    void endTurn();
    void exeOnWeak();
    void endOnWeak();
    void exeOnWeakTurn();
    void exeBrake();
    void exeStepBack();
    void exeStepBackOnWeak();
    void exeReturn();
    void exeProvoke();
    void exeTrampleReaction();
    void exeAware();
    void exeHitReaction();
    void exeBlow();
    void exeElectricDeath();
    void exeElectricReturn();
    void exeJumpToInitPos();
    void exeKeepDistance();

    void edgeRecoverCore();

    void tryLaunchFollower();
    void killAllFollower(FollowerKind);

    bool isDeadAllFollower();
    bool isDeadAllFollowerCore(BegomanBase**, s32);

    bool receiveMsgTrample(HitSensor* pSender, HitSensor* pReceiver);

    void startRotationLevelSound();

    /* 0x100 */ BegomanBaby** mBabyFollowers;
    // should really be begomanSpike, but the class is not defined yet
    /* 0x104 */ BegomanBase** mSpikeFollowers;
    /* 0x108 */ s32 mBabyFollowerNum;
    /* 0x10C */ s32 mSpikeFollowerNum;
    /* 0x110 */ FollowerKind mFollowerKind;
    /* 0x114 */ ParabolicPath* mPath;
    /* 0x118 */ BossBegomanHead* mHead;
    /* 0x11C */ TPos3f mHeadMtx;
    /* 0x14C */ s32 mHealth;
    f32 _150;
    /* 0x154 */ ActorCameraInfo* mOpeningDemoInfo;
};
