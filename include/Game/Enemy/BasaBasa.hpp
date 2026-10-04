#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class AnimScaleController;
class AnimStampController;
class ModelObj;
class SpinHitController;

class BasaBasa : public LiveActor {
public:
    BasaBasa(const char*);

    virtual ~BasaBasa();
    virtual void init(const JMapInfoIter&);
    virtual void initAfterPlacement();
    virtual void kill();
    virtual void control();
    virtual void calcAndSetBaseMtx();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPush(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);
    virtual bool receiveOtherMsg(u32, HitSensor*, HitSensor*);

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

    /* 0x8C */ ModelObj* mHangModel;
    /* 0x90 */ AnimScaleController* mScaleController;
    /* 0x94 */ AnimStampController* mStampController;
    /* 0x98 */ SpinHitController* mSpinHitController;
    /* 0x9C */ TVec3f _9C;
    /* 0xA8 */ f32 _A8;
    /* 0xAC */ f32 _AC;
    /* 0xB0 */ u32 _B0;
    /* 0xB4 */ const TVec3f* _B4;
    /* 0xB8 */ f32 _B8;
    /* 0xBC */ TVec3f _BC;
    /* 0xC8 */ bool mIsIceModel;
    /* 0xCC */ TVec3f _CC;
    /* 0xD8 */ TVec3f _D8;
    /* 0xE4 */ f32 _E4;
    /* 0xE8 */ s32 _E8;
    /* 0xEC */ u8 _EC;
};
