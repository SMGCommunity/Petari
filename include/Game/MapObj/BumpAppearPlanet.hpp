#pragma once

#include "Game/MapObj/MapObjActor.hpp"

class BumpAppearPlanet : public MapObjActor {
public:
    /// @brief Creates a new `BumpAppearPlanet`.
    /// @param pName A pointer to the null-terminated name of the object.
    BumpAppearPlanet(const char* pName);

    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual void initCaseUseSwitchA(const MapObjActorInitInfo&);

    void exeWait();
    void exeBumpOut();
    void exeBumpIn();
    void startBumpOut();
    void startBumpIn();

    /* 0xC4 */ CollisionParts* _C4;
    /* 0xC8 */ CollisionParts* _C8;
};
