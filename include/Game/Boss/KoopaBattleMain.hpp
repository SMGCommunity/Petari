#pragma once

#include "Game/Boss/KoopaBattleBase.hpp"

class HitSensor;
class KoopaStateAttackHipDrop;
class KoopaStateAttackShockWave;
class KoopaStateAttackRoll;
class KoopaStateAttackSpin;

class KoopaBattleMain : public KoopaBattleBase {
public:
    KoopaBattleMain(const char* pName, Koopa* pKoopa);

    virtual ~KoopaBattleMain();
    virtual void init();
    virtual void appear();

    void exeChasePlayer();
    void exeWander();
    void exeSearch();
    void exeJumpAway();
    void exeGuard();
    void exeAttackFireShort();
    void exeAttackShockWave();
    void exeAttackHipDrop();
    void exeAttackSpin();
    void exeAttackRoll();
    void exeDamageEscape();

    bool tryCalcAndSetBaseMtx();
    void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    bool receiveMsgPlayerAttack(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    bool receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver);
    void startMainAttack();
    bool tryJumpAway();

    void exeFind();
    void exeAttackFireLong();

    /* 0x1C */ KoopaStateAttackHipDrop* mStateAttackHipDrop;
    /* 0x20 */ KoopaStateAttackShockWave* mStateAttackShockWave;
    /* 0x24 */ KoopaStateAttackRoll* mStateAttackRoll;
    /* 0x28 */ KoopaStateAttackSpin* mStateAttackSpin;
    /* 0x2C */ bool _2C;
};
