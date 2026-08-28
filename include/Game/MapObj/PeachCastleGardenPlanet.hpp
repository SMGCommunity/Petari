#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class PartsModel;

class PeachCastleGardenPlanet : public MapObjActor {
public:
    PeachCastleGardenPlanet(const char*);

    /* 0x08 */ virtual ~PeachCastleGardenPlanet();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x78 */ virtual void connectToScene(const MapObjActorInitInfo&);
    /* 0x7C */ virtual void initCaseUseSwitchA(const MapObjActorInitInfo&);

    void startDamage();
    void exeWait();
    void exeDamage();

    PartsModel* _C4;
};
