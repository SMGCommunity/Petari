#pragma once

#include "Game/Boss/BossKameckSequencer.hpp"

class BossKameckStateBattle;

class BossKameckVs1 : public BossKameckSequencer {
public:
    BossKameckVs1();

    /* 0x0C */ virtual void start();
    /* 0x10 */ virtual void init(BossKameck*, const JMapInfoIter&);
    /* 0x18 */ virtual void attackSensor(HitSensor*, HitSensor*);
    /* 0x1C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

    void exeOpeningDemo();
    void exeBattleLv1();
    void exeBattleLv2();
    void exePowerUpDemo();
    void exeBattleLv3();
    void exeEndDemo();
    bool isBattle() const;

    /* 0x14 */ BossKameckStateBattle* mStateBattle;
};
