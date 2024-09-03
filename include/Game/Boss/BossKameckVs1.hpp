#pragma once

#include "Game/Boss/BossKameckSequencer.hpp"

class BossKameckStateBattle;

class BossKameckVs1 : public BossKameckSequencer {
public:
    BossKameckVs1();

    virtual ~BossKameckVs1();
    virtual void start();
    virtual void init(BossKameck *, const JMapInfoIter &);
    virtual void attackSensor(HitSensor *, HitSensor *);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor *, HitSensor *);

    void exeOpeningDemo();
    void exeBattleLv1();
    void exeBattleLv2();
    void exePowerUpDemo();
    void exeBattleLv3();
    void exeEndDemo();
    bool isBattle() const;

    BossKameckStateBattle* mStateBattle;            // 0x14
};

namespace NrvBossKameckVs1 {
    NERVE_DECL_EXE(BossKameckVs1NrvOpeningDemo, BossKameckVs1, OpeningDemo);
    NERVE_DECL_EXE(BossKameckVs1NrvBattleLv1, BossKameckVs1, BattleLv1);
    NERVE_DECL_EXE(BossKameckVs1NrvBattleLv2, BossKameckVs1, BattleLv2);
    NERVE_DECL_EXE(BossKameckVs1NrvPowerUpDemo, BossKameckVs1, PowerUpDemo);
    NERVE_DECL_EXE(BossKameckVs1NrvBattleLv3, BossKameckVs1, BattleLv3);
    NERVE_DECL_EXE(BossKameckVs1NrvEndDemo, BossKameckVs1, EndDemo);
};