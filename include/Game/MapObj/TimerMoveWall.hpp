#pragma once

#include "Game/MapObj/MapObjActor.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"

class TimerMoveWall : public MapObjActor {
public:
    TimerMoveWall(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void initCaseUseSwitchB(const MapObjActorInitInfo&);

    void startMove();

    void exeWait();
    void exeMove();

    /* 0xC4 */ s32 mTimer;
    /* 0xC8 */ f32 mMoveLenght;
};
