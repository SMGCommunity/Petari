#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class AnimScaleController;
class WalkerStateBindStarPointer;

class MechanicKoopaMini : public LiveActor {
public:
    MechanicKoopaMini(const char*);

    /* 0x08 */ virtual ~MechanicKoopaMini();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x10 */ virtual void initAfterPlacement();
    /* 0x28 */ virtual void makeActorAppeared();
    /* 0x2C */ virtual void kill();
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x50 */ virtual void updateHitSensor(HitSensor*);
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);
    /* 0x58 */ virtual bool receiveMsgPush(HitSensor*, HitSensor*);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    /* 0x60 */ virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void onAppearSwitchOn();
    void exeWait();
    void exeWalkOnRail();
    void exeTurn();
    void exeJumpStart();
    void exeJumpEnd();
    void exeFind();
    void exePursue();
    void exePreFireAttack();
    void exeFireAttack();
    void endFireAttack();
    void exeFireAttackEnd();
    void exeAttackHit();
    void exeSpinHit();
    void exeTrample();
    void exeHipDropped();
    void exeBindStarPointer();
    void endBindStarPointer();

    /* 0x8C */ bool mIsAppearJumping;
    /* 0x8D */ bool mIsForbidPause;
    /* 0x90 */ AnimScaleController* mScaleController;
    /* 0x94 */ WalkerStateBindStarPointer* mBindStarPointer;
};
