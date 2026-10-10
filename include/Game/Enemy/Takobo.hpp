#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class SpinHitController;
class AnimScaleController;
class SpinningBox;

class Takobo : public LiveActor {
public:
    Takobo(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void initSensor();
    void generateCoin() NO_INLINE;
    bool tryPress();
    void exeMove();
    void endMove();
    void exeWait();
    void endWait();
    void exePress();
    void exeHitPunch();
    void exeStunEnd();
    void exeHitReaction();
    void exeAttack();
    void endAttack();
    void exeStunStart();
    void exeStun();
    void exeIce();
    void endDpdPointed();
    void exeDpdPointed();

    /* 0x8C */ u32 _8C;
    /* 0x90 */ TVec3f _90;
    /* 0x9C */ s32 _9C;
    /* 0xA0 */ u8 _A0;
    /* 0xA1 */ u8 _A1;
    /* 0xA2 */ u8 _A2;
    /* 0xA3 */ u8 _A3;
    /* 0xA4 */ TVec3f _A4;
    /* 0xB0 */ TVec3f _B0;
    /* 0xBC */ bool _BC;
    /* 0xC0 */ f32 _C0;
    /* 0xC4 */ f32 _C4;
    /* 0xC8 */ f32 _C8;
    /* 0xCC */ f32 _CC;
    /* 0xD0 */ s32 _D0;
    /* 0xD4 */ SpinningBox* mBox;
    /* 0xD8 */ AnimScaleController* mScaleController;
    /* 0xDC */ SpinHitController* mSpinController;
};
