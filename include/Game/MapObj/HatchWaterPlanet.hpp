#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class CollisionParts;
class LodCtrl;

class HatchWaterPlanet : public LiveActor {
public:
    HatchWaterPlanet(const char*);

    /* 0x08 */ virtual ~HatchWaterPlanet();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x48 */ virtual void control();

    void exeWait();
    void exeOpen();
    void exeWaitAfterOpen();

    LodCtrl* mPlanetLODCtrl;          // 0x8C
    CollisionParts* mCollisionParts;  // 0x90
};
