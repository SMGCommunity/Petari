#pragma once

#include "Game/Boss/KoopaBattleBase.hpp"

class KoopaStateAttackHipDrop;
class KoopaStateAttackShockWave;
class KoopaStateAttackRoll;
class KoopaStateAttackSpin;

class KoopaBattleMain : public KoopaBattleBase {
public:
    KoopaBattleMain(const char*, Koopa*);

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
    bool attackSensor(HitSensor*, HitSensor*);
    bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);
    bool receiveOtherMsg(u32, HitSensor*, HitSensor*);
    void startMainAttack();
    bool tryJumpAway();

    void exeFind();
    void exeAttackFireLong();

    /* 0x1C */ KoopaStateAttackHipDrop* _1C;
    /* 0x20 */ KoopaStateAttackShockWave* _20;
    /* 0x24 */ KoopaStateAttackRoll* _24;
    /* 0x28 */ KoopaStateAttackSpin* _28;
    /* 0x2C */ bool _2C;
};
