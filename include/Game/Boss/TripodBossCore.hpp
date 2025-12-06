#pragma once

#include "Game/Boss/TripodBossFixPartsBase.hpp"
#include "Game/LiveActor/HitSensor.hpp"

class TripodBossCore : public TripodBossFixPartsBase {
public:
    TripodBossCore(const char*);

    virtual ~TripodBossCore();
    virtual void init(const JMapInfoIter&);
    virtual void kill();
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);
    virtual void activateTripodBoss();

    void exeWait();
    void exeDamageDemo();
    void exeWarning();
    void exeBreak();
    inline void exeNonActive();

    ModelObj* mBreakModel;  // 0xE4
    ModelObj* mBloomModel;  // 0xE8
};
