#pragma once

#include "Game/Boss/KoopaBattleBase.hpp"

class HitSensor;
class KoopaStateAttackRoll;

class KoopaBattleVs3Lv2 : public KoopaBattleBase {
public:
    KoopaBattleVs3Lv2(Koopa* pKoopa);

    virtual ~KoopaBattleVs3Lv2();
    virtual void init();
    virtual void appear();

    bool tryCalcAndSetBaseMtx();
    void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    bool receiveMsgEnemyAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);

    void exeAttackRoll();
    void exeAttackFire();
    void exeDamageReverse();
    void exeJumpAway();
    void exeGuard();
    void exeRecover();

    /* 0x1C */ KoopaStateAttackRoll* mStateAttackRoll;
};
