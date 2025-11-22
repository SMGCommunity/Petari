#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class BumpAppearPlanet : public MapObjActor {
public:
    BumpAppearPlanet(const char*);

    virtual ~BumpAppearPlanet();
    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual void initCaseUseSwitchA(const MapObjActorInitInfo&);

    void exeWait();
    void exeBumpOut();
    void exeBumpIn();
    void startBumpOut();
    void startBumpIn();

    CollisionParts* _C4;
    CollisionParts* _C8;
};
