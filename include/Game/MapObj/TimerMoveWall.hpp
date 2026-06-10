#pragma once

#include "Game/MapObj/MapObjActor.hpp"
#include "Game/NameObj/NameObj.hpp"

class TimerMoveWall : public MapObjActor {
public:
    TimerMoveWall(const char*);
    virtual ~TimerMoveWall();

private:
    u8 mPad[(0xCC) - sizeof(MapObjActor)];
};
