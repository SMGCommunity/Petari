#pragma once

#include "Game/Gravity/PlanetGravity.hpp"

class ParallelGravity : public PlanetGravity {
public:
    enum RANGE_TYPE { RangeType_Sphere = 0, RangeType_Box = 1, RangeType_Cylinder = 2 };

    enum DISTANCE_CALC_TYPE { DistanceCalcType_Default = -1, DistanceCalcType_X = 0, DistanceCalcType_Y = 1, DistanceCalcType_Z = 2 };

    ParallelGravity();

    virtual void updateMtx(const TPos3f& rMtx);
    virtual bool calcOwnGravityVector(TVec3f* pDest, f32* pScalar, const TVec3f& rPosition) const;

    void setPlane(const TVec3f& rPlaneUp, const TVec3f& rPlanePos);
    void setRangeBox(const TPos3f&);
    void setRangeCylinder(f32 radius, f32 height);
    void setRangeType(RANGE_TYPE rangeType);
    void setBaseDistance(f32 val);
    void setDistanceCalcType(DISTANCE_CALC_TYPE distanceCalcType);
    bool isInSphereRange(const TVec3f& rPosition, f32* pScalar) const;
    bool isInBoxRange(const TVec3f& rPosition, f32* pScalar) const;
    bool isInCylinderRange(const TVec3f& rPosition, f32* pScalar) const;
    bool isInRange(const TVec3f& rPosition, f32* pScalar) const;

    TPos3f mLocalMtx;                      // 0x28
    TPos3f mWorldMtx;                      // 0x58
    f32 mExtentX;                          // 0x88
    f32 mExtentY;                          // 0x8C
    f32 mExtentZ;                          // 0x90
    TVec3f mPlanePosition;                 // 0x94
    TVec3f mPlaneUpVec;                    // 0xA0
    TVec3f mWorldPlanePosition;            // 0xAC
    TVec3f mWorldPlaneUpVec;               // 0xB8
    f32 mCylinderHeight;                   // 0xC4
    f32 mCylinderRadius;                   // 0xC8
    f32 mBaseDistance;                     // 0xCC
    RANGE_TYPE mRangeType;                 // 0xD0
    DISTANCE_CALC_TYPE mDistanceCalcType;  // 0xD4
};
