#pragma once

#include "Game/Boss/KoopaBattleBase.hpp"

class KoopaStateAttackRoll;

class KoopaBattleVs3Lv2 : public KoopaBattleBase {
public :
    KoopaBattleVs3Lv2(Koopa*);

    virtual ~KoopaBattleVs3Lv2();
    virtual void init();
    virtual void appear();

    void exeAttackRoll();
    void exeAttackFire();
    void exeDamageReverse();

    bool tryCalcAndSetBaseMtx();
    bool attackSensor(HitSensor*, HitSensor*);
    bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);

    void exeJumpAway();
    void exeGuard();
    void exeRecover();

    /* 0x1C */ KoopaStateAttackRoll* _1C;
};
