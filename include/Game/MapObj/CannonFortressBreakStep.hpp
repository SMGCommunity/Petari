#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class MapObjActorInitInfo;

class CannonFortressBreakStep : public MapObjActor {
public:
    /// @brief Creates a new `CannonFortressBreakStep`.
    /// @param pName A pointer to the null-terminated name of the object.
    CannonFortressBreakStep(const char* pName);

    virtual void init(const JMapInfoIter&);
    virtual void initCaseUseSwitchB(const MapObjActorInitInfo&);
    virtual void initCaseNoUseSwitchB(const MapObjActorInitInfo&);

    void exeWait();
    void exeFall();
    void exeFallStart();
    void exeBreak();
    void startFall();
};
