#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class AnimScaleController;
class WalkerStateBindStarPointer;
class SpinHitController;

class SamboHead : public LiveActor {
public:
    SamboHead(const char*);

    virtual ~SamboHead();
    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPush(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void exeWaitUnderGround();
    void exeHide();
    void exeAppear();
    void exeWait();
    void exePrePursue();
    void exePursue();
    void exePursueEnd();
    void exeHitToPlayer();
    void exeStarPieceHit();
    void exeStampFall();
    void endStampFall();
    void exeStampDeath();
    void endStampDeath();
    void exeHitBlow();
    void endHitBlow();
    void exeBuryDeath();
    void exeBindStarPointer();
    void endBindStarPointer();
    void exeBindStarPointerEnd();

    SpinHitController* mSpinCtrl;                  // 0x8C
    AnimScaleController* mScaleController;         // 0x90
    WalkerStateBindStarPointer* mBindStarPointer;  // 0x94
    TVec3f _98;
    TVec3f _A4;
    bool _B0;
};
