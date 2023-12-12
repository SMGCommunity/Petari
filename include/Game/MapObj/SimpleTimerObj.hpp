#pragma once

#include "Game/MapObj/MapObjActor.h"
#include "Game/LiveActor/FlashingCtrl.h"

class SimpleTimerObj : public MapObjActor {
public:
    SimpleTimerObj(const char *);

    virtual ~SimpleTimerObj();
    virtual void init(const JMapInfoIter &);
    virtual void appear();
    virtual void kill();
    virtual void control();

    void exeTimer();

    FlashingCtrl* mFlashingCtrl;    // _C4
    s32 mTimer;                     // _C8
    s32 mTimeLeft;                  // _CC
};

namespace NrvSimpleTimerObj {
    NERVE(SimpleTimerObjNrvTimer);
};