#pragma once

#include "Game/LiveActor/LiveActor.h"

class ArrowSwitchTarget : public NameObj {
public:
    ArrowSwitchTarget(const char *);

    virtual ~ArrowSwitchTarget();
    virtual void init(const JMapInfoIter &);
    virtual void initAfterPlacement();

    void onTarget();
    void offTarget();

    JMapIdInfo* mJMapIDInfo;                // _C
    StageSwitchCtrl* mStageSwitchCtrl;      // _10
    s32 _14;
};