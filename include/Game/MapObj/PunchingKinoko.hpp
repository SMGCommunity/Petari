#pragma once

#include "Game/LiveActor/LiveActor.h"
#include "Game/Map/GroundChecker.h"
#include "Game/Util/JointController.h"
#include "Game/Enemy/AnimScaleController.h"

class PunchingKinoko : public LiveActor {
public:
    PunchingKinoko(const char *);
    ~PunchingKinoko();

    virtual void init(const JMapInfoIter &);
    virtual void makeActorAppeared();
    virtual void kill();
    virtual void makeActorDead();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor *, HitSensor *);
    virtual bool receiveMsgPush(HitSensor *, HitSensor *);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor *, HitSensor *);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor *, HitSensor *);

    void initShadow();
    void initSensor();
    void initCamera(const JMapInfoIter &);
    void initJointControl();
    void initNerve(const Nerve *);

    bool requestPunch(HitSensor *, HitSensor *);
    bool requestEnemyBlow(HitSensor *, HitSensor *);
    bool requestTrample(HitSensor *, HitSensor *);
    bool requestCrush();

    void exeWait();
    void exeSwing();
    void exePointSnaped();
    void exePunched();
    void exePunchedBrake();
    void exeHitted();
    void exeCrushed();
    void exeCrushedEnd();

    bool ballMtxCallBack(TPos3f *, const JointControllerInfo &);

    // some callers will only match if these are marked NO_INLINE, even though they aren't being inlined (???)
    bool isEnablePunched() const;
    bool isEnableHitPlayer() const NO_INLINE;
    bool isEnableEnemyAttack() const;
    bool isEnableCrushed() const;
    bool isEnableTrample() const;
    bool isEnableBlowed() const NO_INLINE;
    bool isCrushed() const NO_INLINE;

    void addVelocityKeepHeight();

private:
    GroundChecker *mGroundChecker; // _8C
    AnimScaleController *mScaleController; // _90
    JointControlDelegator<PunchingKinoko> *mDelegator; // _94
    s32 _98;
    TVec3f _9C;
    TVec3f _A8;
    s32 mStarPointerHitCoolDown; // _B4
    bool _B8;
    s32 mInvincibleHitCoolDown; // _BC
};

namespace NrvPunchingKinoko {
    NERVE(PunchingKinokoNrvWait);
    NERVE(PunchingKinokoNrvSwing);
    NERVE(PunchingKinokoNrvPunched);
    NERVE(PunchingKinokoNrvPunchedBrake);
    NERVE(PunchingKinokoNrvHitted);
    NERVE(PunchingKinokoNrvPointSnaped);
    NERVE(PunchingKinokoNrvCrushed);
    NERVE(PunchingKinokoNrvCrushedEnd);
};
