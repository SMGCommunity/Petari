#pragma once

#include "Game/MapObj/MapObjActor.hpp"
#include "Game/NameObj/NameObj.hpp"

class SimpleBreakableObj : public MapObjActor {
public:
    SimpleBreakableObj(const char*);
    virtual ~SimpleBreakableObj();

private:
    u8 mPad[(0xE8) - sizeof(MapObjActor)];
};
