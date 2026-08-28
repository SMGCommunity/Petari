#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class AnimScaleController;
class WalkerStateBindStarPointer;
class SpinHitController;

class SamboHead : public LiveActor {
public:
    SamboHead(const char*);

    /* 0x08 */ virtual ~SamboHead();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x24 */ virtual void appear();
    /* 0x2C */ virtual void kill();
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);
    /* 0x58 */ virtual bool receiveMsgPush(HitSensor*, HitSensor*);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

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
