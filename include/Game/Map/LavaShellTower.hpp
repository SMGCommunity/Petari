#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class LavaShellTower : public MapObjActor {
public:
    /// @brief Creates a new `LavaShellTower`.
    /// @param pName A pointer to the null-terminated name of the object.
    LavaShellTower(const char* pName);

    virtual void init(const JMapInfoIter&);
    virtual void initCaseUseSwitchB(const MapObjActorInitInfo&) {};

    void exeWait();
    void exeDemo();
    void exeDone();
};
