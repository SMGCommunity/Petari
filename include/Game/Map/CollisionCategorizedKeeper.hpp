#pragma once

#include "Game/Map/CollisionParts.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/NameObj/NameObj.hpp"
#include <revolution.h>

class CollisionZone;

class CollisionCategorizedKeeper : public NameObj {
public:
    CollisionCategorizedKeeper(s32);

    virtual ~CollisionCategorizedKeeper();
    virtual void movement();

    void addToZone(CollisionParts*, s32);
    void removeFromZone(CollisionParts*, s32);
    void addToGlobal(CollisionParts*);
    void removeFromGlobal(CollisionParts*);

    s32 checkStrikePoint(const TVec3f&, HitInfo*);
    s32 checkStrikeBall(const TVec3f&, f32, bool, const CollisionPartsFilterBase*, const TriangleFilterBase*);
    s32 checkStrikeBallWithThickness(const TVec3f&, f32, f32, const CollisionPartsFilterBase*, const TriangleFilterBase*);
    s32 checkStrikeLine(const TVec3f&, const TVec3f&, s32, const CollisionPartsFilterBase*, const TriangleFilterBase*);
    u32 createAreaPolygonList(Triangle*, u32, const TVec3f&, const TVec3f&);
    u32 createAreaPolygonListArray(Triangle*, u32, TVec3f*, u32);
    void isSphereOverlappingWithBox(const TVec3f&, const TVec3f&, const TVec3f&, f32);
    bool searchSameHostParts(CollisionParts**, CollisionParts*) const;
    HitInfo* getStrikeInfo(u32);
    CollisionZone* getZone(int);

    HitInfo* mHitInfoArray;  // 0xC
    s32 _10;
    s32 mZoneCount;               // 0x14
    CollisionZone* mZones[0x20];  // 0x18
    s32 mZoneNum;                 // 0x98
    u32 _9C;
    u8 _A0;
    u8 _A1;
    u8 _A2;
    u8 _A3;
};

class CollisionZone {
public:
    CollisionZone(s32);

    void addParts(CollisionParts*);
    void calcMinMaxAndRadius();
    void calcMinMaxAndRadiusIfMoveOuter(CollisionParts*);
    void addAndUpdateMinMax(TVec3f, TVec3f);
    void eraseParts(CollisionParts*);

    s32 mZoneID;                         // 0x0
    CollisionParts* mPartsArray[0x200];  // 0x4
    s32 mNumParts;                       // 0x804
    TVec3f _808;
    f32 mRadius;  // 0x814
    TVec3f _818;
    TVec3f _824;
};