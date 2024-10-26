#include "Game/Map/CollisionCategorizedKeeper.hpp"
#include "Game/Map/CollisionParts.hpp"

TVec3f CollisionParts::getTrans() {
    TVec3f translation;
    mBaseMatrix.getTransInline(translation);

    return translation;
}

void CollisionCategorizedKeeper::addToZone(CollisionParts *pParts, s32 zone) {
    mZones[zone]->addParts(pParts);
    mZoneCount++;
}

void CollisionCategorizedKeeper::removeFromZone(CollisionParts *pParts, s32 zone) {
    mZones[zone]->eraseParts(pParts);
    mZoneCount--;
}

void CollisionCategorizedKeeper::addToGlobal(CollisionParts *pParts) {
    mZones[0]->addParts(pParts);
    mZoneCount++;
}

void CollisionCategorizedKeeper::removeFromGlobal(CollisionParts *pParts) {
    mZones[0]->eraseParts(pParts);
    mZoneCount--;
}

CollisionZone::CollisionZone(s32 zoneID)
    : mZoneID(zoneID), mNumParts(0), _808(0, 0, 0), mRadius(0.0f), _818(0, 0, 0), _824(0, 0, 0) {
}

void CollisionZone::addParts(CollisionParts* pParts) {
    s32 cnt = mNumParts;
    mNumParts++;
    mPartsArray[cnt] = pParts;

    if (mZoneID) {
        calcMinMaxAndRadius();
    }
}