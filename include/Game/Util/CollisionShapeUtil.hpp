#pragma once

#include <JSystem/JGeometry/TVec.hpp>

class HitSphere {
public:
    HitSphere() {
    }

    HitSphere(const TVec3f& rPos, f32 radius) {
        mPos.set(rPos);
        mRadius = radius;
    }

    void set(const TVec3f& rPos, f32 radius) {
        mPos.set(rPos);
        mRadius = radius;
    }

    bool isHit(const HitSphere& other) const {
        f32 rad = mRadius + other.mRadius;
        return mPos.squared(other.mPos) <= rad * rad;
    }

    /* 0x00 */ TVec3f mPos;
    /* 0x0C */ f32 mRadius;
};

class HitCylinder {
public:
    HitCylinder() {
    }

    HitCylinder(const TVec3f& rPos, const TVec3f& axis) {
        mPos.set(rPos);
        mAxis.set(axis);
    }

    void set(const TVec3f& rPos, const TVec3f& axis) {
        mPos.set(rPos);
        mAxis.set(axis);
    }

    void getParams(const TVec3f& rPos, f32* proj, f32* ortho) {
        TVec3f diff;
        diff.sub(rPos, mPos);
        f32 dist = diff.squared();
        f32 para = mAxis.squared();

        f32 dot = mAxis.dot(diff);

        *proj = dot / para;
        *ortho = JGeometry::TUtil< f32 >::sqrt((para * dist - dot * dot) / para);
    }

    /* 0x00 */ TVec3f mPos;
    /* 0x0C */ TVec3f mAxis;
};
