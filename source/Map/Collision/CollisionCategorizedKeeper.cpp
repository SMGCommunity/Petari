#include "Map/Collision/CollisionCategorizedKeeper.h"

CollisionCategorizedKeeper::CollisionCategorizedKeeper(u32 unk) : NameObj("地形コリジョンカテゴリキーパー")
{
    mHitInfo = 0;
    _10 = 0;
    mNumCollisionParts = 0;
    mNumZones = 0;
    _9C = unk;
    _A0 = 0;
    _A1 = 1;
    mHitInfo = new HitInfo[0x20];
}

void CollisionCategorizedKeeper::addToGlobal(CollisionParts *pParts)
{
    mGlobalZones->addParts(pParts);
    mNumCollisionParts++;
}

void CollisionCategorizedKeeper::removeFromGlobal(CollisionParts *pParts)
{
    mGlobalZones->eraseParts(pParts);
    mNumCollisionParts--;
}

HitInfo* CollisionCategorizedKeeper::getStrikeInfo(u32 idx)
{
    return &mHitInfo[idx];
}

void CollisionZone::addParts(CollisionParts *pParts)
{
    
}