#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class ModelObj;

class SubmarineVolcanoBigColumn : public LiveActor {
public:
    SubmarineVolcanoBigColumn(const char *);

    virtual ~SubmarineVolcanoBigColumn();
    virtual void init(const JMapInfoIter &);
    virtual void kill();
    virtual bool receiveMsgEnemyAttack(u32, HitSensor *, HitSensor *);

    void exeWait();
    void exeBreak();
    void pauseOff();
    void initBreakModel(const char *);

    ModelObj* mBreakModel;          // 0x8C
    bool mIsSmallColumn;            // 0x90
};

namespace NrvSubmarineVolcanoBigColumn {
    NERVE_DECL(SubmarineVolcanoBigColumnNrvWait, SubmarineVolcanoBigColumn, SubmarineVolcanoBigColumn::exeWait);
    NERVE_DECL(SubmarineVolcanoBigColumnNrvBreak, SubmarineVolcanoBigColumn, SubmarineVolcanoBigColumn::exeBreak);
};