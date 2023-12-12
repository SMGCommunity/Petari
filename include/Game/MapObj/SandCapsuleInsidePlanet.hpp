#pragma once

#include "Game/MapObj/MapObjActor.h"

class SandCapsuleInsidePlanet : public MapObjActor {
public:
    SandCapsuleInsidePlanet(const char *);

    virtual ~SandCapsuleInsidePlanet();
    virtual void init(const JMapInfoIter &);
    virtual void initCaseUseSwitchA(const MapObjActorInitInfo &);

    void startOn();
    void startOff();
};

namespace NrvSandCapsuleInsidePlanet {
    NERVE_DECL_NULL(SandCapsuleInsidePlanetNrvWait);
};