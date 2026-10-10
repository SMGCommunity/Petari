#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class CollisionParts;
class LodCtrl;

class HatchWaterPlanet : public LiveActor {
public:
    HatchWaterPlanet(const char*);

    virtual ~HatchWaterPlanet();
    virtual void init(const JMapInfoIter&);
    virtual void control();

    void exeWait();
    void exeOpen();
    void exeWaitAfterOpen();

    /* 0x8C */ LodCtrl* mPlanetLODCtrl;
    /* 0x90 */ CollisionParts* mCollisionParts;
};
