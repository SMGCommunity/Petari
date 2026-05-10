#pragma once

#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Map/CollisionParts.hpp"

class CollisionPartsFilterBase {
public:
    inline CollisionPartsFilterBase() {
    }

    virtual bool isInvalidParts(const CollisionParts*) const = 0;
};

class CollisionPartsFilterSensor : public CollisionPartsFilterBase {
public:
    inline CollisionPartsFilterSensor(const HitSensor* pSensor) : mSensor(pSensor) {
    }

    virtual bool isInvalidParts(const CollisionParts* pParts) const;

    /* 0x04 */ const HitSensor* mSensor;
};

class CollisionPartsFilterActor : public CollisionPartsFilterBase {
public:
    inline CollisionPartsFilterActor(const LiveActor* pActor) : mActor(pActor) {
    }

    virtual bool isInvalidParts(const CollisionParts* pParts) const;

    /* 0x04 */ const LiveActor* mActor;
};
