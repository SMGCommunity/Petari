#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/MapObj/MapObjActor.hpp"


class SpaceShipStep : public MapObjActor {
public:
    SpaceShipStep(const char*);
    virtual ~SpaceShipStep();

private:
    u8 mPad[(0xD8) - sizeof(MapObjActor)];
};
