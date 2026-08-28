#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class BumpAppearPlanet : public MapObjActor {
public:
    BumpAppearPlanet(const char*);

    /* 0x08 */ virtual ~BumpAppearPlanet();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x48 */ virtual void control();
    /* 0x7C */ virtual void initCaseUseSwitchA(const MapObjActorInitInfo&);

    void exeWait();
    void exeBumpOut();
    void exeBumpIn();
    void startBumpOut();
    void startBumpIn();

    CollisionParts* _C4;
    CollisionParts* _C8;
};
