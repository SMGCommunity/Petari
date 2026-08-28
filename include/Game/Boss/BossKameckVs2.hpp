#pragma once

#include "Game/Boss/BossKameckSequencer.hpp"

class BossKameckBarrier;
class BossKameckStateBattle;

class BossKameckVs2 : public BossKameckSequencer {
public:
    BossKameckVs2();

    /* 0x0C */ virtual void start();
    /* 0x10 */ virtual void init(BossKameck*, const JMapInfoIter&);
    /* 0x18 */ virtual void attackSensor(HitSensor*, HitSensor*);
    /* 0x1C */ virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

    void exeOpeningDemo();
    void exeBattleLv1();
    void exeBattleLv2();
    void exePowerUpDemo();
    void exeBattleLv3();
    void exeBattleLv4();
    void exeEndDemo();
    bool isBattle() const;

    /* 0x14 */ BossKameckStateBattle* mStateBattle;
    /* 0x18 */ BossKameckBarrier* mBarrier;
};
