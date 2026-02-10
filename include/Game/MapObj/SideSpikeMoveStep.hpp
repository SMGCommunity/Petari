#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/MapObj/MapObjActor.hpp"


class SideSpikeMoveStep : public MapObjActor {
public:
    SideSpikeMoveStep(const char*);
    virtual ~SideSpikeMoveStep();

private:
    u8 mPad[(0xD8) - sizeof(MapObjActor)];
};
