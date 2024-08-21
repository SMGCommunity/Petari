#pragma once

#include "Game/Boss/BossKameckSequencer.hpp"
#include "Game/Boss/BossKameckBarrier.hpp"

class BossKameckStateBattle;

class BossKameckVs2 : public BossKameckSequencer {
public:
    BossKameckVs2();

    virtual ~BossKameckVs2();
    virtual void start();
    virtual void init(BossKameck *, const JMapInfoIter &);
    virtual void attackSensor(HitSensor *, HitSensor *);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor *, HitSensor *);

    void exeOpeningDemo();
    void exeBattleLv1();
    void exeBattleLv2();
    void exePowerUpDemo();
    void exeBattleLv3();
    void exeBattleLv4();
    void exeEndDemo();
    bool isBattle() const;

    BossKameckStateBattle* mStateBattle;            // 0x14
    BossKameckBarrier* mBarrier;                    // 0x18
};

namespace NrvBossKameckVs2 {
    NERVE_DECL_EXE(BossKameckVs2NrvOpeningDemo, BossKameckVs2, OpeningDemo);
    NERVE_DECL_EXE(BossKameckVs2NrvBattleLv1, BossKameckVs2, BattleLv1);
    NERVE_DECL_EXE(BossKameckVs2NrvBattleLv2, BossKameckVs2, BattleLv2);
    NERVE_DECL_EXE(BossKameckVs2NrvPowerUpDemo, BossKameckVs2, PowerUpDemo);
    NERVE_DECL_EXE(BossKameckVs2NrvBattleLv3, BossKameckVs2, BattleLv3);
    NERVE_DECL_EXE(BossKameckVs2NrvBattleLv4, BossKameckVs2, BattleLv4);
    NERVE_DECL_EXE(BossKameckVs2NrvEndDemo, BossKameckVs2, EndDemo);
};