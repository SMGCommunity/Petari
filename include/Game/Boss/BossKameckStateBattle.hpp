#pragma once

#include "Game/LiveActor/ActorStateBase.hpp"
#include <JSystem/JGeometry/TVec.hpp>

class BossKameck;
class BossKameckMoveRail;
class BossKameckBattlePattarn;
class KameckBeamEventListener;
class HitSensor;
class KameckBeam;

class BossKameckStateBattle : public ActorStateBase< BossKameck > {
public:
    BossKameckStateBattle(BossKameck*);

    virtual ~BossKameckStateBattle();
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

    BossKameck* mBossKameck;                  // 0xC
    BossKameckMoveRail* mMoveRail;            // 0x10
    BossKameckBattlePattarn* mBattlePattarn;  // 0x14
    KameckBeam* mBeam;                        // 0x18
    KameckBeamEventListener* mBeamListener;   // 0x1C
    TVec3f _20;
    s32 _2C;
    s32 _30;
    s32 _34;
    s32 _38;
    u8 _3C;
    u8 _3D;
};
