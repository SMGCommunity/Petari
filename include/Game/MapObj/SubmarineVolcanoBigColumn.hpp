#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class ModelObj;

class SubmarineVolcanoBigColumn : public LiveActor {
public:
    SubmarineVolcanoBigColumn(const char*);

    /* 0x08 */ virtual ~SubmarineVolcanoBigColumn();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x2C */ virtual void kill();
    /* 0x60 */ virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);

    void exeWait();
    void exeBreak();
    void pauseOff();
    void initBreakModel(const char*);

    ModelObj* mBreakModel;  // 0x8C
    bool mIsSmallColumn;    // 0x90
};
