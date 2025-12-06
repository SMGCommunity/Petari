#pragma once

#include "Game/Boss/TripodBossFixParts.hpp"

class MapPartsRailMover;

class TripodBossRailMoveParts : public TripodBossFixParts {
public:
    TripodBossRailMoveParts(const char*);

    virtual ~TripodBossRailMoveParts();
    virtual void init(const JMapInfoIter&);
    virtual void makeActorDead();
    virtual void calcTripodLocalMatrix(TPos3f*);
    virtual void activateTripodBoss();
    virtual void exeWaitOwn();

    MapPartsRailMover* mRailMover;  // 0xF0
};
