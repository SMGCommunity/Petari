#pragma once

#include "Game/Enemy/AnimScaleController.hpp"
#include "Game/Enemy/AnimStampController.hpp"
#include "Game/Enemy/SpinHitController.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"

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
