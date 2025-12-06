#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class MapObjActorInitInfo;

class LavaShellTower : public MapObjActor {
public:
    LavaShellTower(const char*);

    virtual void init(const JMapInfoIter&);
    virtual void initCaseUseSwitchB(const MapObjActorInitInfo&);

    void exeWait();
    void exeDemo();
    void exeDone();
};
