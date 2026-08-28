#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class LodCtrl;
class MapPartsRotator;

class LavaSunPlanet : public LiveActor {
public:
    LavaSunPlanet(const char*);

    /* 0x08 */ virtual ~LavaSunPlanet();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x48 */ virtual void control();
    /* 0x4C */ virtual void calcAndSetBaseMtx();

    MapPartsRotator* mRotator;  // 0x8C
    const char* mObjName;       // 0x90
    LodCtrl* mLodCtrl;          // 0x94
};
