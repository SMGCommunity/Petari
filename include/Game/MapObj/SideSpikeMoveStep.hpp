#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class SideSpikeMoveStep : public MapObjActor {
public:
    SideSpikeMoveStep(const char*);
    /* 0x08 */ virtual ~SideSpikeMoveStep();

private:
    u8 mPad[(0xD8) - sizeof(MapObjActor)];
};
