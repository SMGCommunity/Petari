#pragma once 

#include "Game/LiveActor/LiveActor.h"
#include "Game/LiveActor/PartsModel.h"

class WatchTowerRotateStep : public LiveActor {
public:
    WatchTowerRotateStep(const char *);

    virtual void init(const JMapInfoIter &);
    virtual void calcAndSetBaseMtx();

    void exeMoveStart();
    void initLift(const JMapInfoIter &);
    void attachLift();

    TVec3f mRotDeg;         // _8C
    PartsModel** mLifts;    // _98
};

namespace NrvWatchTowerRotateStep {
    NERVE(WatchTowerRotateStepNrvWait);
    NERVE(WatchTowerRotateStepNrvMoveStart);
    NERVE(WatchTowerRotateStepNrvMove);
};