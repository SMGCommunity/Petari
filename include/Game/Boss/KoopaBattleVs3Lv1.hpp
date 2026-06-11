#pragma once

#include "Game/Boss/KoopaBattleBase.hpp"

class KoopaStateChaseRoll;
class KoopaStateAttackShockWave;

class KoopaBattleVs3Lv1 : public KoopaBattleBase {
public:
    KoopaBattleVs3Lv1(Koopa* pKoopa);

    virtual ~KoopaBattleVs3Lv1();
    virtual void init();
    virtual void appear();

    void exeChaseRoll();
    void exeAttackShockWave();
    void exeDamageReverse();

    bool tryCalcAndSetBaseMtx();
    bool attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeAttackFire();
    void exeJumpAway();
    void exeGuard();
    void exeRecover();

    /* 0x1C */ KoopaStateChaseRoll* mStateChaseRoll;
    /* 0x20 */ KoopaStateAttackShockWave* mStateAttackShockWave;
};
