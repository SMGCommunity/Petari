#pragma once

#include <JSystem/JGeometry.hpp>
#include <revolution.h>

class CollisionZone;
class HitInfo;
class HitSensor;
class KC_PrismData;
class KCollisionServer;
class Triangle;
class TriangleFilterBase;

class CollisionParts {
public:
    CollisionParts();

    TVec3f getTrans();
    void init(const TPos3f&, HitSensor*, const void*, const void*, s32, bool);
    void addToBelongZone();
    void removeFromBelongZone();
    void initWithAutoEqualScale(const TPos3f&, HitSensor*, const void*, const void*, s32, bool);
    void initWithNotUsingScale(const TPos3f&, HitSensor*, const void*, const void*, s32, bool);
    void resetAllMtx(const TPos3f&);
    void resetAllMtx();
    void forceResetAllMtxAndSetUpdateMtxOneTime();
    void resetAllMtxPrivate(const TPos3f&);
    void setMtx(const TPos3f&);
    void setMtx();
    void updateMtx();
    f32 makeEqualScale(MtxPtr);
    void updateBoundingSphereRange();
    void updateBoundingSphereRange(TVec3f);
    void updateBoundingSphereRangePrivate(f32);
    const char* getHostName() const;
    s32 getPlacementZoneID() const;
    bool checkStrikePoint(HitInfo*, const TVec3f&);
    u32 checkStrikeBall(HitInfo*, u32, const TVec3f&, f32, bool, const TriangleFilterBase*);
    u32 checkStrikeBallCore(HitInfo*, u32, const TVec3f&, const TVec3f&, f32, f32, f32, KC_PrismData**, f32*, u8*, const TriangleFilterBase*,
                            const TVec3f*);
    u32 checkStrikeBallWithThickness(HitInfo*, u32, const TVec3f&, f32, f32, const TriangleFilterBase*);
    void calcCollidePosition(TVec3f*, const KC_PrismData&, u8);
    void projectToPlane(TVec3f*, const TVec3f&, const TVec3f&, const TVec3f&);
    u32 checkStrikeLine(HitInfo*, u32, const TVec3f&, const TVec3f&, const TriangleFilterBase*);
    u32 createAreaPolygonList(Triangle*, u32, const TVec3f&, const TVec3f&);
    u32 createAreaPolygonListArray(Triangle*, u32, TVec3f*, u32);
    void calcForceMovePower(TVec3f*, const TVec3f&) const;

    /* 0x00 */ TPos3f* _0;
    /* 0x04 */ TPos3f mMatrix;
    /* 0x34 */ TPos3f mBaseMatrix;
    /* 0x64 */ TPos3f mInvBaseMatrix;
    /* 0x94 */ TPos3f mPrevBaseMatrix;
    /* 0xC4 */ KCollisionServer* mServer;
    /* 0xC8 */ HitSensor* mHitSensor;
    /* 0xCC */ bool _CC;
    /* 0xCD */ bool _CD;
    /* 0xCE */ bool _CE;
    /* 0xCF */ bool _CF;
    /* 0xD0 */ bool _D0;
    /* 0xD1 */ u8 _D1[3];
    /* 0xD4 */ s32 _D4;
    /* 0xD8 */ f32 _D8;
    /* 0xDC */ f32 _DC;
    /* 0xE0 */ s32 mKeeperIndex;
    /* 0xE4 */ CollisionZone* mZone;
    /* 0xE8 */ f32 _E8;
    /* 0xEC */ f32 _EC;
    /* 0xF0 */ f32 _F0;
};
