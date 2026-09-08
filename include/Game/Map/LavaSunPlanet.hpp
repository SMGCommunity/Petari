#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class LodCtrl;
class MapPartsRotator;

class LavaSunPlanet : public LiveActor {
public:
    /// @brief Creates a new `LavaSunPlanet`.
    /// @param pName A pointer to the null-terminated name of the object.
    LavaSunPlanet(const char* pName);

    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual void calcAndSetBaseMtx();

    /* 0x8C */ MapPartsRotator* mRotator;
    /* 0x90 */ const char* mObjName;
    /* 0x94 */ LodCtrl* mLodCtrl;
};
