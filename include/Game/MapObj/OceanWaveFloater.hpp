#pragma once

#include "Game/NameObj/NameObj.hpp"
#include "Game/MapObj/MapObjActor.hpp"


class OceanWaveFloater : public MapObjActor {
public:
    OceanWaveFloater(const char*);
    virtual ~OceanWaveFloater();

private:
    u8 mPad[(0xF0) - sizeof(MapObjActor)];
};
