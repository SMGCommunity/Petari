#pragma once

#include "Game/MapObj/MapObjActor.hpp"
#include "Game/MapObj/MapObjActorInitInfo.hpp"

class LavaShellTower : public MapObjActor {
public:
    LavaShellTower(const char *);

    virtual ~LavaShellTower();
    virtual void init(const JMapInfoIter &);
    virtual void initCaseUseSwitchB(const MapObjActorInitInfo &);
    
    inline void exeDone();
    void exeDemo();
    inline void exeWait();
};
