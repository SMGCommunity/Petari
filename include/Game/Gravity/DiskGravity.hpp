#pragma once

#include "Game/Gravity/PlanetGravity.hpp"

class DiskGravity : public PlanetGravity {
public:
    DiskGravity();

    virtual void updateMtx(const TPos3f& rMtx);
    virtual bool calcOwnGravityVector(TVec3f* pDest, f32* pScalar, const TVec3f& rPosition) const;

    void updateLocalParam();

    void setLocalPosition(const TVec3f& rLocalPos);
    void setLocalDirection(const TVec3f& rLocalDir);
    void setSideDirection(const TVec3f& rSideDir);
    void setRadius(f32 val);
    void setValidDegee(f32 val);
    void setBothSide(bool val);
    void setEnableEdgeGravity(bool val);

    // Position
    /* 0x28 */ TVec3f mLocalPosition;
    /* 0x34 */ TVec3f mWorldPosition;

    // Unit normal vector to the circular surface of the disk
    /* 0x40 */ TVec3f mLocalNormal;
    /* 0x4C */ TVec3f mWorldNormal;

    /* 0x58 */ TVec3f mSideDirection;

    /* 0x64 */ TVec3f mOppositeSideVecOrtho;
    /* 0x70 */ TVec3f mWorldOppositeSideVecOrtho;

    // Radius of the circular face of the disk
    /* 0x7C */ f32 mLocalRadius;
    /* 0x80 */ f32 mWorldRadius;

    // Number of degrees of the circle gravity applies to
    /* 0x84 */ f32 mValidDegree;
    /* 0x88 */ f32 mValidCos;

    /* 0x8C */ bool mEnableBothSide;
    /* 0x8D */ bool mEnableEdgeGravity;
};
