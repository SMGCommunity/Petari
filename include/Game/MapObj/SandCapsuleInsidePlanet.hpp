#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class SandCapsuleInsidePlanet : public MapObjActor {
public:
    SandCapsuleInsidePlanet(const char*);

    virtual ~SandCapsuleInsidePlanet();
    virtual void init(const JMapInfoIter&);
    virtual void initCaseUseSwitchA(const MapObjActorInitInfo&);

    void exeWait();

    void startOn();
    void startOff();
};
