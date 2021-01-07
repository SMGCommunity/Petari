#pragma once

#include "NameObj/NameObj.h"
#include "Map/Collision/Triangle.h"

class CollisionZone;

class CollisionCategorizedKeeper : public NameObj
{
public:
    CollisionCategorizedKeeper(u32);

    virtual ~CollisionCategorizedKeeper();
    virtual void movement();

    void addToZone(CollisionParts *, s32);
    void removeFromZone(CollisionParts *, s32);
    void addToGlobal(CollisionParts *);
    void removeFromGlobal(CollisionParts *);

    HitInfo* getStrikeInfo(u32);

    HitInfo* mHitInfo; // _C
    u32 _10;
    u32 mNumCollisionParts; // _14
    CollisionZone* mGlobalZones; // _18
    u8 _1C[0x98-0x1C];
    u32 mNumZones; // _98
    u32 _9C;
    u8 _A0;
    u8 _A1;
};

class CollisionZone
{
public:
    CollisionZone(u32);

    void addParts(CollisionParts *);
    void eraseParts(CollisionParts *);
    void calcMinMaxAndRadius();
    void calcMinMaxAndRadiusIfMoveOuter(CollisionParts *);

    u32 _0;
    u8 _4[0x804-0x4];
    u32 mPartsCount; // _804
};