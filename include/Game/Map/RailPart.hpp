#pragma once

#include "JSystem/JGeometry/TVec.hpp"
#include "Game/Map/BezierRail.hpp"

class LinearRailPart;

class RailPart {
public:

    RailPart();

    void init(const TVec3f &, const TVec3f &, const TVec3f &, const TVec3f &);
    void initForBezier(const TVec3f &, const TVec3f &, const TVec3f &, const TVec3f &);
    void calcPos(TVec3f *, f32) const;
    void calcVelocity(TVec3f *, f32) const;
    f32 getLength(f32, f32, int) const;
    f32 getTotalLength() const;
    f32 getParam(f32) const;
    f32 getNearestParam(const TVec3f &, f32) const;

    LinearRailPart* mRailPartLinear; // 0x0
    BezierRailPart* mRailPartBezier; // 0x4
};

class LinearRailPart {
public:
    inline LinearRailPart() { }

    void set(const TVec3f &, const TVec3f &);

    f32 getNearestParam(const TVec3f &, f32) const;

    TVec3f _0;
    TVec3f _C;
    f32 _18;
};