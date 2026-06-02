#pragma once

#include "Game/Boss/KoopaBattleBase.hpp"

class KoopaStateChaseRoll;
class KoopaStateAttackShockWave;

class KoopaBattleVs3Lv1 : public KoopaBattleBase {
public :
    KoopaBattleVs3Lv1(Koopa*);

    virtual ~KoopaBattleVs3Lv1();
    virtual void init();
    virtual void appear();

    void exeChaseRoll();
    void exeAttackShockWave();
    void exeDamageReverse();
    
    bool tryCalcAndSetBaseMtx();
    bool attackSensor(HitSensor*, HitSensor*);
    bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

    void exeAttackFire();
    void exeJumpAway();
    void exeGuard();
    void exeRecover();

    /* 0x1C */ KoopaStateChaseRoll* _1C;
    /* 0x20 */ KoopaStateAttackShockWave* _20;
};
