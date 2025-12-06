#pragma once

#include "Game/Boss/TripodBossFixParts.hpp"

class MapPartsRotator;

class TripodBossRotateParts : public TripodBossFixParts {
public:
    TripodBossRotateParts(const char*);

    virtual ~TripodBossRotateParts();
    virtual void init(const JMapInfoIter&);
    virtual void makeActorDead();
    virtual void calcTripodLocalMatrix(TPos3f*);
    virtual void activateTripodBoss();
    virtual void exeWaitOwn();

    MapPartsRotator* mRotator;  // 0xF0
};
