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

    /* 0x0C */ virtual void init(const JMapInfoIter& rIter);
    /* 0x1C */ virtual void calcAnim();
    /* 0x24 */ virtual void appear();
    /* 0x2C */ virtual void kill();
    /* 0x48 */ virtual void control();
    /* 0x54 */ virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    /* 0x58 */ virtual bool receiveMsgPush(HitSensor* pSender, HitSensor* pReceiver);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    /* 0x60 */ virtual bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    /* 0x74 */ virtual bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    virtual u32 getKind() const {
        return 0;
    };
    /* 0x78 */ virtual bool onTouchElectric(const TVec3f&, const TVec3f&);
    /* 0x7C */ virtual bool setNerveReturn();
    /* 0x84 */ virtual const Nerve* getNerveWait();
    /* 0x88 */ virtual void addVelocityOnPushedFromElectricRail(const TVec3f&, const TVec3f&);
    /* 0x8C */ virtual bool requestAttack();

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
