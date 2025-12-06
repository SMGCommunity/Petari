#pragma once

#include "Game/Gravity/PlanetGravity.hpp"

class CubeGravity : public PlanetGravity {
public:
    CubeGravity();

    virtual void updateMtx(const TPos3f& rMtx);
    virtual bool calcOwnGravityVector(TVec3f* pDest, f32* pScalar, const TVec3f& rPosition) const;

    void setCube(const TPos3f&);

    /*
     * Area as in region, not area as in Area = Base * h
     *
     * The region is made up of 3 components: x, y, z. For each component, a value of 0 indicates
     * that the position is beyond the negative face of that component, a value of 1 indicates that
     * the position is between the two faces for that component, and a value of 2 indicates that
     * the position is beyond the positive face of that component.
     *
     * The region is encoded as follows (in pseudocode):
     *      `area = area_x * 3^0 + area_y * 3^1 + area_z * 3^2`
     *
     * `calcGravityArea` returns the encoded region
     */
    int calcGravityArea(const TVec3f& rPosition) const;
    bool calcFaceGravity(const TVec3f& rPosition, s32, TVec3f* pDest, f32* pScalar) const;
    bool calcEdgeGravity(const TVec3f& rPosition, s32, TVec3f* pDest, f32* pScalar) const;
    bool calcCornerGravity(const TVec3f& rPosition, s32, TVec3f* pDest, f32* pScalar) const;

    TPos3f mCube;      // 0x28
    TPos3f mPosition;  // 0x58
    f32 lenX;          // 0x88
    f32 lenY;          // 0x8C
    f32 lenZ;          // 0x90
    u8 mActiveFaces;   // 0x94
};
