#pragma once

#include "Game/Gravity/PlanetGravity.hpp"

class CubeGravity : public PlanetGravity {
public:
    CubeGravity();

    virtual void updateMtx(const TPos3f &rMtx);
    virtual bool calcOwnGravityVector(TVec3f *pDest, f32 *pScalar, const TVec3f &rPosition) const;

    void setCube(const TPos3f &);
    // Area as in region, not area as in Area = Base * h
    int calcGravityArea(const TVec3f &rPosition) const;
    bool calcFaceGravity(const TVec3f &rPosition, s32, TVec3f *pDest, f32 *pScalar) const;
    bool calcEdgeGravity(const TVec3f &rPosition, s32, TVec3f *pDest, f32 *pScalar) const;
    bool calcCornerGravity(const TVec3f &rPosition, s32, TVec3f *pDest, f32 *pScalar) const;
    
    TPos3f mCube;        // _28
    TPos3f mPosition;    // _58
    f32 lenX;            // _88
    f32 lenY;            // _8C
    f32 lenZ;            // _90
    u8 mActiveFaces;     // _94
};
