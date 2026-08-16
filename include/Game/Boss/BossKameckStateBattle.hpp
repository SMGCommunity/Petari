#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class BossKameck;
class BossKameckBattlePattarn;
class BossKameckMoveRail;
class HitSensor;
class KameckBeam;
class KameckBeamEventListener;

class BossKameckStateBattle : public ActorStateBase< BossKameck > {
public:
    BossKameckStateBattle(BossKameck*);

    virtual void init();
    virtual void appear();
    virtual void control();
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

    void setMoveRail(BossKameckMoveRail*);
    void setBattlePattarn(BossKameckBattlePattarn*);
    bool requestDamage(HitSensor*, HitSensor*);
    bool requestGuard(HitSensor*, HitSensor*);
    bool tryAttackWait();
    bool trySummonKameck();
    bool tryChargeBram();
    void startMove();
    void exeWait();
    void exeMove();
    void exeHideMoveStart();
    void exeHideMove();
    void exeHideMoveEnd();
    void exeSummonKameckWait();
    void exeSummonKameck();
    void exeAttackWait();
    void exeAttack();
    void exeDamage();
    void exeRecover();
    void exeGuard();
    void selectStoppablePosition();
    void selectPosition();
    bool isEnableDamage() const;
    bool isEnableGuard() const;

    /* 0x10 */ BossKameckMoveRail* mMoveRail;
    /* 0x14 */ BossKameckBattlePattarn* mBattlePattarn;
    /* 0x18 */ KameckBeam* mBeam;
    /* 0x1C */ KameckBeamEventListener* mBeamEventListener;
    /* 0x20 */ TVec3f _20;
    /* 0x2C */ s32 _2C;
    /* 0x30 */ s32 _30;
    /* 0x34 */ s32 _34;
    /* 0x38 */ s32 _38;
    /* 0x3C */ bool mIsFinal;
    /* 0x3D */ bool mIsVs2;
};
