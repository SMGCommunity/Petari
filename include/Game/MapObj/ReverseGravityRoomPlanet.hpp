#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class ReverseGravityRoomPlanet : public MapObjActor {
public:
    ReverseGravityRoomPlanet(const char*);

    virtual ~ReverseGravityRoomPlanet();
    virtual void init(const JMapInfoIter&);
    virtual void initCaseUseSwitchA(const MapObjActorInitInfo&);

    void exeDownStart();
    void exeUpStart();
    void startSwitchOn();
    void startSwitchOff();
};

namespace NrvReverseGravityRoomPlanet {
    NERVE(ReverseGravityRoomPlanetNrvDownStart);
    NERVE(ReverseGravityRoomPlanetNrvDownWait);
    NERVE(ReverseGravityRoomPlanetNrvUpStart);
    NERVE(ReverseGravityRoomPlanetNrvUpWait);
};  // namespace NrvReverseGravityRoomPlanet