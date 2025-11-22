#pragma once

#include "Game/Gravity/PlanetGravity.hpp"

class SegmentGravity : public PlanetGravity {
public:
    SegmentGravity();

    virtual void updateMtx(const TPos3f& rMtx);
    virtual bool calcOwnGravityVector(TVec3f* pDest, f32* pScalar, const TVec3f& rPosition) const;

    void setGravityPoint(u32 index, const TVec3f& rGravityPoint);
    void setSideVector(const TVec3f& rSideVec);
    void setValidSideDegree(f32 val);
    void setEdgeValid(u32 index, bool val);
    void updateLocalParam();

    // Represents the centers of each base of the cylinder
    TVec3f mGravityPoints[2];       // 0x28
    TVec3f mWorldGravityPoints[2];  // 0x40

    TVec3f mSideVector;  // 0x58
    TVec3f mOppositeSideVecOrtho;
    TVec3f mWorldOppositeSideVecOrtho;

    TVec3f mAxis;     // 0x7C
    f32 mAxisLength;  // 0x88

    f32 mValidSideDegree;         // 0x8C
    f32 mValidSideCos;            // 0x90
    f32 mHemisphereExtension[2];  // 0x94
    bool mEdges[2];               // 0x9C
};
