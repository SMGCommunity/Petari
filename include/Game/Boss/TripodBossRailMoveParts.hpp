#pragma once

#include "Game/Boss/TripodBossFixParts.hpp"

class MapPartsRailMover;

class TripodBossRailMoveParts : public TripodBossFixParts {
public:
    TripodBossRailMoveParts(const char*);

    /* 0x08 */ virtual ~TripodBossRailMoveParts();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x30 */ virtual void makeActorDead();
    /* 0x78 */ virtual void calcTripodLocalMatrix(TPos3f*);
    /* 0x7C */ virtual void activateTripodBoss();
    /* 0x80 */ virtual void exeWaitOwn();

    MapPartsRailMover* mRailMover;  // 0xF0
};
