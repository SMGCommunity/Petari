#pragma once

#include <revolution.h>
#include "Game/Map/CollisionParts.h"
#include "Game/Map/HitInfo.h"
#include "Game/NameObj/NameObj.h"

class CollisionZone;

class CollisionCategorizedKeeper : public NameObj {
public:
    CollisionCategorizedKeeper(s32);

    virtual ~CollisionCategorizedKeeper();
    virtual void movement();

    void addToZone(CollisionParts *, s32);
    void removeFromZone(CollisionParts *, s32);
    void addToGlobal(CollisionParts *);
    void removeFromGlobal(CollisionParts *);

    HitInfo* mHitInfoArray;         // _C
    s32 _10;
    s32 mZoneCount;                 // _14
    CollisionZone* mZones[0x20];    // _18
    s32 mZoneNum;                   // _98
    u32 _9C;
    u8 _A0;
    u8 _A1;
    u8 _A2;
    u8 _A3;
};

class CollisionZone {
public:
    CollisionZone(s32);

    void addParts(CollisionParts *);
    void calcMinMaxAndRadius();
    void calcMinMaxAndRadiusIfMoveOuter(CollisionParts *);
    void addAndUpdateMinMax(TVec3f, TVec3f);
    void eraseParts(CollisionParts *);

    s32 mZoneID;                        // _0
    CollisionParts* mPartsArray[0x200]; // _4
    s32 mNumParts;                      // _804
    TVec3f _808;
    f32 mRadius;                        // _814
    TVec3f _818;
    TVec3f _824;
};