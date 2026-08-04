#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class SandCapsuleInsidePlanet : public MapObjActor {
public:
    /// @brief Creates a new `SandCapsuleInsidePlanet`.
    /// @param pName A pointer to the null-terminated name of the object.
    SandCapsuleInsidePlanet(const char* pName);

    virtual void init(const JMapInfoIter&);
    virtual void initCaseUseSwitchA(const MapObjActorInitInfo&);

    void startOn();
    void startOff();

    void exeWait();
};
