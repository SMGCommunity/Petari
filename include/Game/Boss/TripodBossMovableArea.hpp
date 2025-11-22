#pragma once

#include <JSystem/JGeometry.hpp>
#include <revolution.h>

class HitResult {
public:
    HitResult();

    TVec3f _0;
    TVec3f _C;
    TVec3f _18;
};

class TripodBossMovableArea {
public:
    TripodBossMovableArea();

    virtual bool collideSphere(HitResult*, const TVec3f&, f32, const TVec3f&) const;
    virtual void calcNearLandingPosition(TVec3f*, const TVec3f&) const;
    virtual void calcLandingNormal(TVec3f*, const TVec3f&) const;
    virtual void calcLandingFront(TVec3f*, const TVec3f&) const;

    void setCenter(const TVec3f&);
    void setBaseAxis(const TVec3f&);
    void setFrontVector(const TVec3f&);
    void setRadius(f32);

    TVec3f mCenter;    // 0x04
    TVec3f mBaseAxis;  // 0x10
    TVec3f mFront;     // 0x1C
    f32 mRadius;       // 0x28
    f32 _2C;
    f32 _30;
};
