#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class SeesawMoveNut : public MapObjActor {
public:
    SeesawMoveNut(const char*);
    /* 0x08 */ virtual ~SeesawMoveNut();

private:
    u8 mPad[(0xD0) - sizeof(MapObjActor)];
};
