#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class AstroDomeDemoAstroGalaxy : public MapObjActor {
public:
    AstroDomeDemoAstroGalaxy(const char*);

    virtual ~AstroDomeDemoAstroGalaxy();
    virtual void init(const JMapInfoIter&);
};