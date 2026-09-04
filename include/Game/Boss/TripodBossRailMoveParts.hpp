#pragma once

#include "Game/Boss/TripodBossFixParts.hpp"

class MapPartsRailMover;

class TripodBossRailMoveParts : public TripodBossFixParts {
public:
    /// @brief Creates a new `TripodBossRailMoveParts`.
    /// @param pName A pointer to the null-terminated name of the object.
    TripodBossRailMoveParts(const char* pName);

    virtual void init(const JMapInfoIter&);
    virtual void makeActorDead();
    virtual void calcTripodLocalMatrix(TPos3f*);
    virtual void activateTripodBoss();
    virtual void exeWaitOwn();

    /* 0xF0 */ MapPartsRailMover* mRailMover;
};
