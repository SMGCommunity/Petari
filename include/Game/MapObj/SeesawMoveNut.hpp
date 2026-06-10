#pragma once

#include "Game/MapObj/MapObjActor.hpp"
#include "Game/NameObj/NameObj.hpp"

class SeesawMoveNut : public MapObjActor {
public:
    SeesawMoveNut(const char*);
    virtual ~SeesawMoveNut();

private:
    u8 mPad[(0xD0) - sizeof(MapObjActor)];
};
