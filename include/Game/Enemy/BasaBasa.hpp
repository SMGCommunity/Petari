#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class AnimScaleController;
class AnimStampController;
class ModelObj;
class SpinHitController;

class BasaBasa : public LiveActor {
public:
    BasaBasa(const char*);

    /* 0x08 */ virtual ~BasaBasa();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x10 */ virtual void initAfterPlacement();
    /* 0x2C */ virtual void kill();
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();
    /* 0x54 */ virtual void attackSensor(HitSensor*, HitSensor*);
    /* 0x58 */ virtual bool receiveMsgPush(HitSensor*, HitSensor*);
    /* 0x5C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    /* 0x60 */ virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);
    /* 0x74 */ virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

    void exeWait();
    void exeAirWait();
    void exeChaseStart();
    void exeChase();
    void exeQuickTurnStart();
    void exeQuickTurn();
    void exeTrampleDown();
    void exePunchDown();
    void exeAttackStart();
    void exeAttack();
    void exeAttackEnd();
    void exeAttackEndRecover();
    void exeHitBack();
    void exeHitBackEnd();
    void exeComeHome();
    void exeAttachCelling();
    void exeDPDSwoon();
    void endDPDSwoon();
    void exeStun();
    void initHangModel();
    bool tryClippingAndResetPos();
    bool trySetNerveDPDSwoon();
    bool tryComeHome();
    void updateRailType();
    void controlVelocity();
    void tuneHeight();
    bool isNearTarget(f32) const;
    bool isNrvEnableStun() const;

    ModelObj* mHangModel;                   // 0x8C
    AnimScaleController* mScaleController;  // 0x90
    AnimStampController* mStampController;  // 0x94
    SpinHitController* mSpinHitController;  // 0x98
    TVec3f _9C;
    f32 _A8;
    f32 _AC;
    u32 _B0;
    const TVec3f* _B4;
    f32 _B8;
    TVec3f _BC;
    bool mIsIceModel;  // 0xC8
    TVec3f _CC;
    TVec3f _D8;
    f32 _E4;
    s32 _E8;
    u8 _EC;
};
