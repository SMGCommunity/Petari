#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class OceanWaveFloater : public MapObjActor {
public:
    OceanWaveFloater(const char*);
    /* 0x08 */ virtual ~OceanWaveFloater();

private:
    u8 mPad[(0xF0) - sizeof(MapObjActor)];
};
