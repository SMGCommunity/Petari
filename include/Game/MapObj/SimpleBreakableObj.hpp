#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class SimpleBreakableObj : public MapObjActor {
public:
    SimpleBreakableObj(const char*);
    /* 0x08 */ virtual ~SimpleBreakableObj();

private:
    u8 mPad[(0xE8) - sizeof(MapObjActor)];
};
