#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class AnimScaleController;
class ModelObj;
class WalkerStateBindStarPointer;

class PackunPetit : public LiveActor {
public:
    PackunPetit(const char*);

    /* 0x08 */ virtual ~PackunPetit();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x10 */ virtual void initAfterPlacement();
    /* 0x2C */ virtual void kill();
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    /* 0x60 */ virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void exeNonActive();
    void endNonActive();
    void exeWait();
    void exeFind();
    void exeThreat();
    void exeTurn();
    void exeAttackStart();
    void exeAttack();
    void exeAttackBack();
    void exeHitWaitForAttack();
    void exeHit();
    void exeTrampleDown();
    void exePunchDown();
    void exeSwoonStart();
    void exeSwoon();
    void exeSwoonToThreat();
    void exeDPDSwoon();
    void endDPDSwoon();

    void initBlowModel();
    void punchDown(HitSensor*, HitSensor*);
    void selectNrvWait();
    bool tryNonActive();
    bool tryTurn();
    bool tryDPDSwoon();

    AnimScaleController* mScaleController;          // 0x8C
    WalkerStateBindStarPointer* mStarPointerState;  // 0x90
    TVec3f _94;
    ModelObj* mBlownModel;  // 0xA0
    bool mDontTurn;         // 0xA4
};
