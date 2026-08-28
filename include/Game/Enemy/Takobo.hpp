#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class SpinHitController;
class AnimScaleController;
class SpinningBox;

class Takobo : public LiveActor {
public:
    Takobo(const char*);

    /* 0x08 */ virtual ~Takobo();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x10 */ virtual void initAfterPlacement();
    /* 0x2C */ virtual void kill();
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    /* 0x60 */ virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

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

    u32 _8C;
    TVec3f _90;
    s32 _9C;
    u8 _A0;
    u8 _A1;
    u8 _A2;
    u8 _A3;
    TVec3f _A4;
    TVec3f _B0;
    bool _BC;
    f32 _C0;
    f32 _C4;
    f32 _C8;
    f32 _CC;
    s32 _D0;
    SpinningBox* mBox;                      // 0xD4
    AnimScaleController* mScaleController;  // 0xD8
    SpinHitController* mSpinController;     // 0xDC
};
