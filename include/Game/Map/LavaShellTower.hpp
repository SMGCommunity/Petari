#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class LavaShellTower : public MapObjActor {
public:
    LavaShellTower(const char*);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x84 */ virtual void initCaseUseSwitchB(const MapObjActorInitInfo&) {};

    void exeWait();
    void exeDemo();
    void exeDone();
};
