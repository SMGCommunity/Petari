#pragma once

#include "Game/MapObj/MapObjActor.h"

class LavaBallRisingPlanetLava : public MapObjActor {
public:
    LavaBallRisingPlanetLava(const char *);

    virtual ~LavaBallRisingPlanetLava();
    virtual void init(const JMapInfoIter &);

    void exeWait();
    void exeScaleUp();
    void exeScaleDown();
};

namespace NrvLavaBallRisingPlanetLava {
    NERVE_DECL(LavaBallRisingPlanetLavaNrvWaitSmall, LavaBallRisingPlanetLava, LavaBallRisingPlanetLava::exeWait);
    NERVE_DECL(LavaBallRisingPlanetLavaNrvWaitBig, LavaBallRisingPlanetLava, LavaBallRisingPlanetLava::exeWait);
    NERVE_DECL(LavaBallRisingPlanetLavaNrvScaleUp, LavaBallRisingPlanetLava, LavaBallRisingPlanetLava::exeScaleUp);
    NERVE_DECL(LavaBallRisingPlanetLavaNrvScaleDown, LavaBallRisingPlanetLava, LavaBallRisingPlanetLava::exeScaleDown);
};