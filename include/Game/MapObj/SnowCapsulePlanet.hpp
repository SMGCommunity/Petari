#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class SnowCapsulePlanet : public MapObjActor {
public:
    SnowCapsulePlanet(const char*);

    /* 0x08 */ virtual ~SnowCapsulePlanet();
    /* 0x0C */ virtual void init(const JMapInfoIter&);

    void exeCloseWait();
    void exeOpenWait();
    void exeOpen();

    CollisionParts* mDoor1;
    CollisionParts* mDoor2;
};
