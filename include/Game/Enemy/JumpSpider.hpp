#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class SpinHitController;
class AnimScaleController;

class JumpSpider : public LiveActor {
public:
    JumpSpider(const char*);

    virtual ~JumpSpider();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void kill();
    virtual void startClipped();
    virtual void endClipped();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void exeWait();
    void exePreJump();
    void exeJump();
    void exeJumpEnd();
    void exeHitToPlayer();
    void exeStampFall();
    void endStampFall();
    void exeStampDeath();
    void endStampDeath();
    void exeHitBlow();
    void endHitBlow();
    void exeDpdPointed();
    void endDpdPointed();
    void exeNoPowerStart();
    void exeNoPowerLand();
    void exeNoPowerWait();
    void exeNoPowerEnd();

    /* 0x8C */ SpinHitController* mSpinHitController;
    /* 0x90 */ AnimScaleController* mScaleController;
    /* 0x94 */ TVec3f _94;
    /* 0xA0 */ TVec3f _A0;
};
