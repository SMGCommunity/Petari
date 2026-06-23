#pragma once

#include "Game/Boss/TripodBossFixPartsBase.hpp"

class ModelObj;

class TripodBossShell : public TripodBossFixPartsBase {
public:
    TripodBossShell(const char*);

    virtual ~TripodBossShell();
    virtual void init(const JMapInfoIter&);
    virtual void kill();
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);

    virtual void activateTripodBoss();

    void exeNonActive();
    void exeWait();
    void exeBreak();

    /* 0xE4 */ ModelObj* mBreakModel;
};
