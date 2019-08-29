#ifndef RAILPART_H
#define RAILPART_H

#include "types.h"
#include "JGeometry/TVec3.h"

class LinearRailPart;
class BezierRailPart;

class RailPart
{
public:
    RailPart();

    void init(const JGeometry::TVec3<f32> &, const JGeometry::TVec3<f32> &, const JGeometry::TVec3<f32> &, const JGeometry::TVec3<f32> &);
    void initForBezier(const JGeometry::TVec3<f32> &, const JGeometry::TVec3<f32> &, const JGeometry::TVec3<f32> &, const JGeometry::TVec3<f32> &);
    void calcPos(JGeometry::TVec3<f32> *, f32) const;
    void calcVelocity(JGeometry::TVec3<f32> *, f32) const;
    f32 getLength(f32, f32, s32) const;
    f32 getTotalLength() const;
    f32 getParam(f32) const;
    f32 getNearestParam(const JGeometry::TVec3<f32> &) const;

    LinearRailPart* mLinearRailPart; // _0
    BezierRailPart* mBezierRailPart; // _4
};

class LinearRailPart
{
public:
    inline LinearRailPart() { }

    void set(const JGeometry::TVec3<f32> &, const JGeometry::TVec3<f32> &);
    f32 getNearestParam(const JGeometry::TVec3<f32> &) const;

    JGeometry::TVec3<f32> _0;
    JGeometry::TVec3<f32> _C;
    f32 mMagnitude; // _18
};

class BezierRailPart
{
public:
    inline BezierRailPart() { }

    void set(const JGeometry::TVec3<f32> &, const JGeometry::TVec3<f32> &, const JGeometry::TVec3<f32> &, const JGeometry::TVec3<f32> &);
    void calcPos(JGeometry::TVec3<f32> *, f32) const;
    void calcVelocity(JGeometry::TVec3<f32> *, f32) const;
    f32 getLength(f32, f32, s32) const;
    f32 getParam(f32) const;
    f32 getNearestParam(const JGeometry::TVec3<f32> &) const;

    const JGeometry::TVec3<f32> _0;
    const JGeometry::TVec3<f32> _C;
    const JGeometry::TVec3<f32> _18;
    const JGeometry::TVec3<f32> _24;
    f32 mLength; // _30
};

#endif // RAILPART_H