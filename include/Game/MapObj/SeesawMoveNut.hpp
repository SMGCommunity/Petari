#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/MapObj/MapObjActor.hpp"


class SeesawMoveNut : public MapObjActor {
public:
    SeesawMoveNut(const char*);
    virtual ~SeesawMoveNut();

private:
    u8 mPad[(0xD0) - sizeof(MapObjActor)];
};
