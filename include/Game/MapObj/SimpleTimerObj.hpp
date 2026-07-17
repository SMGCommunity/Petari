#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class FlashingCtrl;

class SimpleTimerObj : public MapObjActor {
public:
    SimpleTimerObj(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void appear();
    virtual void kill();
    virtual void control();

    void exeTimer();

    /* 0xC4 */ FlashingCtrl* mFlashingCtrl;
    /* 0xC8 */ s32 mTimeLimit;
    /* 0xCC */ s32 mTimeLeft;
};
