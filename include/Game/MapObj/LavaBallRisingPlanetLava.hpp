#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class LavaBallRisingPlanetLava : public MapObjActor {
public:
    LavaBallRisingPlanetLava(const char*);

    virtual ~LavaBallRisingPlanetLava();
    virtual void init(const JMapInfoIter&);

    void exeWait();
    void exeScaleUp();
    void exeScaleDown();
};
