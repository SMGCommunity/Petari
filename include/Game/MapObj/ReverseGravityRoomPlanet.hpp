#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class ReverseGravityRoomPlanet : public MapObjActor {
public:
    ReverseGravityRoomPlanet(const char*);

    /* 0x08 */ virtual ~ReverseGravityRoomPlanet();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x7C */ virtual void initCaseUseSwitchA(const MapObjActorInitInfo&);

    void exeDownStart();
    void exeDownWait();
    void exeUpStart();
    void exeUpWait();
    void startSwitchOn();
    void startSwitchOff();
};
