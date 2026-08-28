#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class LavaBallRisingPlanetLava : public MapObjActor {
public:
    LavaBallRisingPlanetLava(const char*);

    /* 0x08 */ virtual ~LavaBallRisingPlanetLava();
    /* 0x0C */ virtual void init(const JMapInfoIter&);

    void exeWait();
    void exeScaleUp();
    void exeScaleDown();
};
