#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class LodCtrl;
class MapPartsRotator;

class LavaSunPlanet : public LiveActor {
public:
    LavaSunPlanet(const char*);

    virtual ~LavaSunPlanet();
    virtual void init(const JMapInfoIter&);
    virtual void control();
    virtual void calcAndSetBaseMtx();

    MapPartsRotator* mRotator;  // 0x8C
    const char* mObjName;       // 0x90
    LodCtrl* mLodCtrl;          // 0x94
};
