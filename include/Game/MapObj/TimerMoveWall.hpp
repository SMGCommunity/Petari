#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/MapObj/MapObjActor.hpp"


class TimerMoveWall : public MapObjActor {
public:
    TimerMoveWall(const char*);
    virtual ~TimerMoveWall();

private:
    u8 mPad[(0xCC) - sizeof(MapObjActor)];
};
