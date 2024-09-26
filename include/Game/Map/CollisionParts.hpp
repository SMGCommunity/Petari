#pragma once

#include <revolution.h>
#include "JSystem/JGeometry.hpp"

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
    void init(const TPos3f &, HitSensor *, const void *, const void *, long, bool);
    void addToBelongZone();
    void removeFromBelongZone();
    void initWithAutoEqualScale(const TPos3f &, HitSensor *, const void *, const void *, long, bool);
    void initWithNotUsingScale(const TPos3f &, HitSensor *, const void *, const void *, long, bool);
    void resetAllMtx(const TPos3f &);
    void resetAllMtx();
    void forceResetAllMtxAndSetUpdateMtxOneTime();
    void resetAllMtxPrivate(const TPos3f &);
    void setMtx(const TPos3f &);
    void setMtx();
    void updateMtx();
    f32 makeEqualScale(MtxPtr);
    void updateBoundingSphereRange();
    void updateBoundingSphereRange(TVec3f);
    void updateBoundingSphereRangePrivate(float);
    const char *getHostName() const;
    s32 getPlacementZoneID() const;
    void checkStrikePoint(HitInfo *, const TVec3f &);
    bool checkStrikeBall(HitInfo *, unsigned long, const TVec3f &, float, bool, const TriangleFilterBase *);
    void checkStrikeBallCore(HitInfo *, unsigned long, const TVec3f &, const TVec3f &, float, float, float, KC_PrismData **, float *, unsigned char *, const TriangleFilterBase *, const TVec3f *);
    void checkStrikeBallWithThickness(HitInfo *, unsigned long, const TVec3f &, float, float, const TriangleFilterBase *);
    void calcCollidePosition(TVec3f *, const KC_PrismData &, unsigned char);
    void projectToPlane(TVec3f *, const TVec3f &, const TVec3f &, const TVec3f &);
    void checkStrikeLine(HitInfo *, unsigned long, const TVec3f &, const TVec3f &, const TriangleFilterBase *);
    void createAreaPolygonList(Triangle *, unsigned long, const TVec3f &, const TVec3f &);
    void createAreaPolygonListArray(Triangle *, unsigned long, TVec3f *, unsigned long);
    void calcForceMovePower(TVec3f *, const TVec3f &) const;

    TMtx34f *_0;
    TMtx34f mMatrix;            // 0x4
    TPos3f mBaseMatrix;         // 0x34
    TPos3f mInvBaseMatrix;      // 0x64
    TPos3f mPrevBaseMatrix;     // 0x94
    KCollisionServer *mServer;  // 0xC4
    HitSensor *mHitSensor;      // 0xC8
    bool _CC;
    bool _CD;
    bool _CE;
    bool _CF;
    bool _D0;
    u8 _D1[3];
    s32 _D4;
    f32 _D8;
    f32 _DC;
    s32 mKeeperIndex;           // 0xE0
    CollisionZone *mZone;       // 0xE4   
    f32 _E8;
    f32 _EC;
    f32 _F0;
};

class CollisionPartsFilterBase {
public:
    inline CollisionPartsFilterBase() {

    }

    virtual bool isInvalidParts(const CollisionParts *) const = 0;
};