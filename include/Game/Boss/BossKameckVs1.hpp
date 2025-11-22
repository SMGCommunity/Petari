#pragma once

#include "Game/Boss/BossKameckSequencer.hpp"

class BossKameckStateBattle;

class BossKameckVs1 : public BossKameckSequencer {
public:
    BossKameckVs1();

    virtual ~BossKameckVs1();
    virtual void start();
    virtual void init(BossKameck*, const JMapInfoIter&);
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

    void exeOpeningDemo();
    void exeBattleLv1();
    void exeBattleLv2();
    void exePowerUpDemo();
    void exeBattleLv3();
    void exeEndDemo();
    bool isBattle() const;

    BossKameckStateBattle* mStateBattle;  // 0x14
};
