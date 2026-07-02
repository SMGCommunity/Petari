#pragma once

#include "Game/Boss/TripodBossFixPartsBase.hpp"

class ModelObj;

class TripodBossCore : public TripodBossFixPartsBase {
public:
    TripodBossCore(const char*);

    virtual ~TripodBossCore();
    virtual void init(const JMapInfoIter&);
    virtual void kill();
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);

    virtual void activateTripodBoss();

    void exeNonActive();
    void exeWait();
    void exeDamageDemo();
    void exeWarning();
    void exeBreak();

    /* 0xE4 */ ModelObj* mBreakModel;
    /* 0xE8 */ ModelObj* mBloomModel;
};
