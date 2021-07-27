#pragma once

#include <revolution.h>
#include "JSystem/JGeometry/TVec3.h"

class LinearRailPart;
class BezierRailPart;

class RailPart
{
public:
    RailPart();

    void init(const JGeometry::TVec3f &, const JGeometry::TVec3f &, const JGeometry::TVec3f &, const JGeometry::TVec3f &);
    void initForBezier(const JGeometry::TVec3f &, const JGeometry::TVec3f &, const JGeometry::TVec3f &, const JGeometry::TVec3f &);
    void calcPos(JGeometry::TVec3f *, f32) const;
    void calcVelocity(JGeometry::TVec3f *, f32) const;
    f32 getLength(f32, f32, s32) const;
    f32 getTotalLength() const;
    f32 getParam(f32) const;
    f32 getNearestParam(const JGeometry::TVec3f &, f32) const;

    LinearRailPart* mLinearRailPart; // _0
    BezierRailPart* mBezierRailPart; // _4
};

class LinearRailPart
{
public:
    inline LinearRailPart() { }

    void set(const JGeometry::TVec3f &, const JGeometry::TVec3f &);
    f32 getNearestParam(const JGeometry::TVec3f &, f32) const;

    JGeometry::TVec3f _0;
    JGeometry::TVec3f _C;
    f32 mMagnitude; // _18
};

class BezierRailPart
{
public:
    inline BezierRailPart() { }

    void set(const JGeometry::TVec3f &, const JGeometry::TVec3f &, const JGeometry::TVec3f &, const JGeometry::TVec3f &);
    void calcPos(JGeometry::TVec3f *, f32) const;
    void calcVelocity(JGeometry::TVec3f *, f32) const;
    f32 getLength(f32, f32, s32) const;
    f32 getParam(f32) const;
    f32 getNearestParam(const JGeometry::TVec3f &, f32) const;

    const JGeometry::TVec3f _0;
    const JGeometry::TVec3f _C;
    const JGeometry::TVec3f _18;
    const JGeometry::TVec3f _24;
    f32 mLength; // _30
};