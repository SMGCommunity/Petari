#pragma once

#include <revolution.h>
#include "Game/Map/CollisionParts.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/NameObj/NameObj.hpp"

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

    void checkStrikePoint(const TVec3f &, HitInfo *);
    void checkStrikeBall(const TVec3f &, float, bool, const CollisionPartsFilterBase *, const TriangleFilterBase *);
    void checkStrikeBallWithThickness(const TVec3f &, float, float, const CollisionPartsFilterBase *, const TriangleFilterBase *);
    void checkStrikeLine(const TVec3f &, const TVec3f &, long, const CollisionPartsFilterBase *, const TriangleFilterBase *);
    void createAreaPolygonList(Triangle *, unsigned long, const TVec3f &, const TVec3f &);
    void createAreaPolygonListArray(Triangle *, unsigned long, TVec3f *, unsigned long);
    void isSphereOverlappingWithBox(const TVec3f &, const TVec3f &, const TVec3f &, float);
    void searchSameHostParts(CollisionParts **, CollisionParts *) const;
    void getStrikeInfo(unsigned long);
    CollisionZone *getZone(int);

    HitInfo* mHitInfoArray;         // 0xC
    s32 _10;
    s32 mZoneCount;                 // 0x14
    CollisionZone* mZones[0x20];    // 0x18
    s32 mZoneNum;                   // 0x98
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

    s32 mZoneID;                        // 0x0
    CollisionParts* mPartsArray[0x200]; // 0x4
    s32 mNumParts;                      // 0x804
    TVec3f _808;
    f32 mRadius;                        // 0x814
    TVec3f _818;
    TVec3f _824;
};