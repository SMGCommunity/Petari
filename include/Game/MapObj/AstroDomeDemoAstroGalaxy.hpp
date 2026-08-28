#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class AstroDomeDemoAstroGalaxy : public MapObjActor {
public:
    AstroDomeDemoAstroGalaxy(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
};
