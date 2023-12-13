#pragma once

#include "Game/LiveActor/LiveActor.hpp"

class HatchWaterPlanet : public LiveActor {
public:
    HatchWaterPlanet(const char *);

    virtual ~HatchWaterPlanet();
    virtual void init(const JMapInfoIter &);
    virtual void control();

    void exeOpen();
    void exeWaitAfterOpen();

    LodCtrl* mPlanetLODCtrl;            // _8C
    CollisionParts* mCollisionParts;    // _90
};

namespace NrvHatchWaterPlanet {
    NERVE(HatchWaterPlanetNrvWait);
    NERVE(HatchWaterPlanetNrvOpen);
    NERVE(HatchWaterPlanetNrvWaitAfterOpen);  
};
