#pragma once

#include "Game/MapObj/MapObjActor.hpp"
#include "Game/NameObj/NameObj.hpp"

class SideSpikeMoveStep : public MapObjActor {
public:
    SideSpikeMoveStep(const char*);
    virtual ~SideSpikeMoveStep();

private:
    u8 mPad[(0xD8) - sizeof(MapObjActor)];
};
