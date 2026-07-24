#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class AnimScaleController;
class WalkerStateBindStarPointer;

class MechanicKoopaMini : public LiveActor {
public:
    MechanicKoopaMini(const char*);

    virtual ~MechanicKoopaMini();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void makeActorAppeared();
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void updateHitSensor(HitSensor*);
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPush(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);    
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);    

    void onAppearsSwitchOn();
    void exeWait();
    void exeWalkOnRail();
    void exeTurn();
    void exeJumpStart();
    void exeJumpEnd();
    void exeFind();
    void exePursue();
    void exePreFireAttack();
    void exeFireAttack();
    void exeFireAttackEnd();
    void exeAttackHit();
    void exeSpinHit();
    void exeTrample();
    void exeHipDropped();

    /* 0x8C */ bool _8C;
    /* 0x8D */ bool _8D;
    /* 0x90 */ AnimScaleController* mScaleController;
    /* 0x94 */ WalkerStateBindStarPointer* mBindStarPointer;
};
