#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class HatchWaterPlanet : public LiveActor {
public:
    HatchWaterPlanet(const char*);

    virtual ~HatchWaterPlanet();
    virtual void init(const JMapInfoIter&);
    virtual void control();

    void exeWait();
    void exeOpen();
    void exeWaitAfterOpen();

    LodCtrl* mPlanetLODCtrl;          // 0x8C
    CollisionParts* mCollisionParts;  // 0x90
};
