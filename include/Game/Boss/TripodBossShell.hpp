#pragma once

#include "Game/Boss/TripodBossFixPartsBase.hpp"

class ModelObj;

class TripodBossShell : public TripodBossFixPartsBase {
public:
    TripodBossShell(const char*);

    /* 0x08 */ virtual ~TripodBossShell();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x2C */ virtual void kill();
    /* 0x60 */ virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);

    /* 0x7C */ virtual void activateTripodBoss();

    void exeNonActive();
    void exeWait();
    void exeBreak();

    /* 0xE4 */ ModelObj* mBreakModel;
};
