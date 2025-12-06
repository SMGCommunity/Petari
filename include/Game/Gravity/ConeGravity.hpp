#pragma once

#include "Game/Gravity/PlanetGravity.hpp"

class ConeGravity : public PlanetGravity {
public:
    ConeGravity();

    virtual void updateMtx(const TPos3f& rMtx);
    virtual bool calcOwnGravityVector(TVec3f* pDest, f32* pScalar, const TVec3f& rPosition) const;

    void setLocalMatrix(const TPos3f& rMtx);
    void setEnableBottom(bool val);
    void setTopCutRate(f32 val);

    /*
     * The x column represents the radius vector. The y column represents the central axis vector,
     * point from the center of the base of the cone to the apex. The translation column represents
     * the location of the center of the base of the cone, and the z column is unused.
     */
    TPos3f mLocalMtx;  // 0x28
    TPos3f mWorldMtx;  // 0x58

    f32 mValidDegree;    // 0x88; set in ctor but never used
    f32 mValidCos;       // 0x8C; set in ctor but never used
    f32 mWorldRadius;    // 0x90
    bool mEnableBottom;  // 0x94
    f32 mTopCutRate;     // 0x98
};
