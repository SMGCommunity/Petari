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
    BossBegoman(const char*);

    enum FollowerKind {
        babyFollower = 0,
        spikeFollower = 1,
        bothFollower = 2,
    };

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

    virtual u32 getKind() const {return 0;};

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

    BegomanBaby** mBabyFollowers; //9x100
    //should really be begomanSpike, but the class is not defined yet
    BegomanBase** mSpikeFollowers; //0x104
    s32 mBabyFollowerNum; //0x108
    s32 mSpikeFollowerNum; //0x10C
    FollowerKind mFollowerKind; //110
    ParabolicPath* mPath; // 0x114;
    BossBegomanHead* mHead; // 0x118;
    TPos3f mHeadMtx; // 0x11C
    s32 mHealth; // 0x14C
    f32 _150;
    ActorCameraInfo* mOpeningDemoInfo; // 0x154
};
