#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class ModelObj;

class SubmarineVolcanoBigColumn : public LiveActor {
public:
    SubmarineVolcanoBigColumn(const char*);

    virtual ~SubmarineVolcanoBigColumn();
    virtual void init(const JMapInfoIter&);
    virtual void kill();
    virtual bool receiveMsgEnemyAttack(u32, HitSensor*, HitSensor*);

    void exeWait();
    void exeBreak();
    void pauseOff();
    void initBreakModel(const char*);

    /* 0x8C */ ModelObj* mBreakModel;
    /* 0x90 */ bool mIsSmallColumn;
};
