#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class SnowCapsulePlanet : public MapObjActor {
    public:
        SnowCapsulePlanet(const char *);

        virtual ~SnowCapsulePlanet();
        virtual void init(const JMapInfoIter &);

        void exeOpen();

        CollisionParts *mDoor1;
        CollisionParts *mDoor2;
};

namespace NrvSnowCapsulePlanet {
    NERVE(SnowCapsulePlanetNrvCloseWait);
    NERVE(SnowCapsulePlanetNrvOpenWait);
    NERVE(SnowCapsulePlanetNrvOpen);
};