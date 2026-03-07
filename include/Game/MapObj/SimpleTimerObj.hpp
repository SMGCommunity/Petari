#pragma once

#include "Game/LiveActor/FlashingCtrl.hpp"
#include "Game/MapObj/MapObjActor.hpp"

class SimpleTimerObj : public MapObjActor {
public:
    SimpleTimerObj(const char*);

    virtual ~SimpleTimerObj();
    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual void kill();
    virtual void control();

    void exeTimer();

    FlashingCtrl* mFlashingCtrl;  // 0xC4
    s32 mTimer;                   // 0xC8
    s32 mTimeLeft;                // 0xCC
};
