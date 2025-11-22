#pragma once

#include "Game/MapObj/MapObjActor.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"

class PlanetMapWithoutHighModel : public MapObjActor {
public:
    PlanetMapWithoutHighModel(const char*);

    virtual ~PlanetMapWithoutHighModel();
    virtual void init(const JMapInfoIter&);
    virtual void connectToScene(const MapObjActorInitInfo&);
};
