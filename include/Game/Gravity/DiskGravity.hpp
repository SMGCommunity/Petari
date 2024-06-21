#pragma once

#include "Game/Gravity/PlanetGravity.hpp"

class DiskGravity : public PlanetGravity {
public:
    DiskGravity();

    virtual void updateMtx(const TPos3f &rMtx);
    virtual bool calcOwnGravityVector(TVec3f *pDest, f32 *pScalar, const TVec3f &rPosition) const;

    void updateLocalParam();

    void setLocalPosition(const TVec3f &rLocalPos);
    void setLocalDirection(const TVec3f &rLocalDir);
    void setSideDirection(const TVec3f &rSideDir);
    void setRadius(f32 val);
    void setValidDegee(f32 val);
    void setBothSide(bool val);
    void setEnableEdgeGravity(bool val);

    // Position
    TVec3f mLocalPosition;    // _28
    TVec3f mWorldPosition;    // _34

    // Unit normal vector to the circular surface of the disk
    TVec3f mLocalNormal;    // _40
    TVec3f mWorldNormal;    // _4C

    TVec3f mSideDirection;    // _58

    // 0 rad in local space
    TVec3f mSideVecOrtho;    // _64
                             // Transformed 0 rad
    TVec3f mWorldSideDir;    // _70

    // Radius of the circular face of the disk
    f32 mLocalRadius;    // _7C
    f32 mWorldRadius;    // _80

    // Number of degrees of the circle gravity applies to
    f32 mValidDegree;    // _84
    f32 mValidCos;       // _88

    bool mEnableBothSide;       // _8C
    bool mEnableEdgeGravity;    // _8D
};
