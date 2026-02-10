#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/MapObj/MapObjActor.hpp"


class SimpleBreakableObj : public MapObjActor {
public:
    SimpleBreakableObj(const char*);
    virtual ~SimpleBreakableObj();

private:
    u8 mPad[(0xE8) - sizeof(MapObjActor)];
};
