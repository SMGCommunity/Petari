#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class FlashingCtrl;

class SimpleTimerObj : public MapObjActor {
public:
    SimpleTimerObj(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x24 */ virtual void appear();
    /* 0x2C */ virtual void kill();
    /* 0x48 */ virtual void control();

    void exeTimer();

    /* 0xC4 */ FlashingCtrl* mFlashingCtrl;
    /* 0xC8 */ s32 mTimeLimit;
    /* 0xCC */ s32 mTimeLeft;
};
