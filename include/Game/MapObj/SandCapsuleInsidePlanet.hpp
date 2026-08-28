#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class SandCapsuleInsidePlanet : public MapObjActor {
public:
    /// @brief Creates a new `SandCapsuleInsidePlanet`.
    /// @param pName A pointer to the null-terminated name of the object.
    SandCapsuleInsidePlanet(const char* pName);

    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x7C */ virtual void initCaseUseSwitchA(const MapObjActorInitInfo&);

    void startOn();
    void startOff();

    void exeWait();
};
