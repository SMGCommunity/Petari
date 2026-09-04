#pragma once

#include "Game/Boss/TripodBossFixParts.hpp"

class MapPartsRotator;

class TripodBossRotateParts : public TripodBossFixParts {
public:
    /// @brief Creates a new `TripodBossRotateParts`.
    /// @param pName A pointer to the null-terminated name of the object.
    TripodBossRotateParts(const char* pName);

    virtual ~TripodBossRotateParts();
    virtual void init(const JMapInfoIter&);
    virtual void makeActorDead();
    virtual void calcTripodLocalMatrix(TPos3f*);
    virtual void activateTripodBoss();
    virtual void exeWaitOwn();

    /* 0xF0 */ MapPartsRotator* mRotator;
};
