#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class PlanetMapWithoutHighModel : public MapObjActor {
public:
    PlanetMapWithoutHighModel(const char*);

    /* 0x08 */ virtual ~PlanetMapWithoutHighModel();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x78 */ virtual void connectToScene(const MapObjActorInitInfo&);
};
