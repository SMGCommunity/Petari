#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Enemy/WalkerStateBindStarPointer.hpp"

class PackunPetit : public LiveActor {
public:
    PackunPetit(const char *);

    virtual ~PackunPetit();
    virtual void init(const JMapInfoIter &);
    virtual void initAfterPlacement();
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor *, HitSensor *);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor *, HitSensor *);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor *, HitSensor *);
    virtual bool receiveOtherMsg(u32, HitSensor *, HitSensor *);

    void exeNonActive();
    void exeWait();
    void exeFind();
    void exeThreat();
    void exeTurn();
    void exeAttackStart();
    void exeHitWaitForAttack();
    inline void exeAttackBack();    
    inline void exeAttack();
    void exeHit();
    void exeTrampleDown();
    void exePunchDown();
    void exeSwoonStart();
    void exeSwoon();
    void exeSwoonToThreat();
    void initBlowModel();
    void punchDown(HitSensor *, HitSensor *);
    void selectNrvWait();
    bool tryNonActive();
    bool tryTurn();
    bool tryDPDSwoon();
    inline void exeSwoonOnEnd();
    inline void exeDPDSwoon();
    inline void exeNonActiveOnEnd();

    AnimScaleController* mScaleController;          // 0x8C
    WalkerStateBindStarPointer* mStarPointerState;  // 0x90
    TVec3f _94;
    ModelObj* mBlownModel;                          // 0xA0
    bool mDontTurn;                                 // 0xA4
};

namespace NrvPackunPetit {
    NERVE_DECL_ONEND(PackunPetitNrvNonActive, PackunPetit, PackunPetit::exeNonActive, PackunPetit::exeNonActiveOnEnd);
    NERVE_DECL(PackunPetitNrvWait, PackunPetit, PackunPetit::exeWait);
    NERVE_DECL(PackunPetitNrvFind, PackunPetit, PackunPetit::exeFind);
    NERVE_DECL(PackunPetitNrvThreat, PackunPetit, PackunPetit::exeThreat);
    NERVE_DECL(PackunPetitNrvLeftTurn, PackunPetit, PackunPetit::exeTurn);
    NERVE_DECL(PackunPetitNrvRightTurn, PackunPetit, PackunPetit::exeTurn);
    NERVE_DECL(PackunPetitNrvAttackStart, PackunPetit, PackunPetit::exeAttackStart);
    NERVE_DECL(PackunPetitNrvAttack, PackunPetit, PackunPetit::exeAttack);
    NERVE_DECL(PackunPetitNrvAttackBack, PackunPetit, PackunPetit::exeAttackBack);
    NERVE_DECL(PackunPetitNrvHitWaitForAttack, PackunPetit, PackunPetit::exeHitWaitForAttack);
    NERVE_DECL(PackunPetitNrvHit, PackunPetit, PackunPetit::exeHit);
    NERVE_DECL(PackunPetitNrvTrampleDown, PackunPetit, PackunPetit::exeTrampleDown);
    NERVE_DECL(PackunPetitNrvPunchDown, PackunPetit, PackunPetit::exePunchDown);
    NERVE_DECL(PackunPetitNrvSwoonStart, PackunPetit, PackunPetit::exeSwoonStart);
    NERVE_DECL(PackunPetitNrvSwoon, PackunPetit, PackunPetit::exeSwoon);
    NERVE_DECL(PackunPetitNrvSwoonToThreat, PackunPetit, PackunPetit::exeSwoonToThreat);
    NERVE_DECL_ONEND(PackunPetitNrvDPDSwoon, PackunPetit, PackunPetit::exeDPDSwoon, PackunPetit::exeSwoonOnEnd);
};