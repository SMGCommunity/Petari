#include "Game/Util/CollisionPartsFilter.hpp"
#include "Game/LiveActor/HitSensor.hpp"

bool CollisionPartsFilterSensor::isInvalidParts(const CollisionParts* pParts) const {
    return pParts->mHitSensor == mSensor;
}

bool CollisionPartsFilterActor::isInvalidParts(const CollisionParts* pParts) const {
    return pParts->mHitSensor->mHost == mActor;
}
