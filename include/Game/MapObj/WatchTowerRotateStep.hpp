#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/PartsModel.hpp"

class WatchTowerRotateStep : public LiveActor {
public:
    WatchTowerRotateStep(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x4C */ virtual void calcAndSetBaseMtx();

    void exeWait();
    void exeMoveStart();
    void exeMove();
    void initLift(const JMapInfoIter&);
    void attachLift();

    TVec3f mRotDeg;       // 0x8C
    PartsModel** mLifts;  // 0x98
};
