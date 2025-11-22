#pragma once

#include "Game/Boss/BossKameckSequencer.hpp"

class BossKameckBarrier;
class BossKameckStateBattle;

class BossKameckVs2 : public BossKameckSequencer {
public:
    BossKameckVs2();

    virtual ~BossKameckVs2();
    virtual void start();
    virtual void init(BossKameck*, const JMapInfoIter&);
    virtual void attackSensor(HitSensor*, HitSensor*);
    virtual bool receiveMsgPlayerAttack(u32, HitSensor*, HitSensor*);

    void exeOpeningDemo();
    void exeBattleLv1();
    void exeBattleLv2();
    void exePowerUpDemo();
    void exeBattleLv3();
    void exeBattleLv4();
    void exeEndDemo();
    bool isBattle() const;

    BossKameckStateBattle* mStateBattle;  // 0x14
    BossKameckBarrier* mBarrier;          // 0x18
};
