#pragma once

#include "Game/Boss/KoopaBattleBase.hpp"

class HitSensor;
class KoopaStateAttackShockWave;
class KoopaStateChaseRoll;

class KoopaBattleVs3Lv1 : public KoopaBattleBase {
public:
    KoopaBattleVs3Lv1(Koopa* pKoopa);

    virtual ~KoopaBattleVs3Lv1();
    virtual void init();
    virtual void appear();

    bool tryCalcAndSetBaseMtx();
    void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeChaseRoll();
    void exeAttackFire();
    void exeAttackShockWave();
    void exeDamageReverse();
    void exeJumpAway();
    void exeGuard();
    void exeRecover();

    /* 0x1C */ KoopaStateChaseRoll* mStateChaseRoll;
    /* 0x20 */ KoopaStateAttackShockWave* mStateAttackShockWave;
};
