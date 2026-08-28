#pragma once

#include "Game/Boss/TripodBossFixParts.hpp"

class MapPartsRotator;

class TripodBossRotateParts : public TripodBossFixParts {
public:
    TripodBossRotateParts(const char*);

    /* 0x08 */ virtual ~TripodBossRotateParts();
    /* 0x0C */ virtual void init(const JMapInfoIter&);
    /* 0x30 */ virtual void makeActorDead();
    /* 0x78 */ virtual void calcTripodLocalMatrix(TPos3f*);
    /* 0x7C */ virtual void activateTripodBoss();
    /* 0x80 */ virtual void exeWaitOwn();

    MapPartsRotator* mRotator;  // 0xF0
};
