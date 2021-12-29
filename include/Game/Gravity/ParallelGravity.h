#pragma once

#include "Game/Gravity/PlanetGravity.h"

class ParallelGravity : public PlanetGravity {
public:
	enum RANGE_TYPE {
		RangeType_Sphere = 0,
		RangeType_Box = 1,
		RangeType_Cylinder = 2
	};

	enum DISTANCE_CALC_TYPE {
		DistanceCalcType_Default = -1,
		DistanceCalcType_X = 0,
		DistanceCalcType_Y = 1,
		DistanceCalcType_Z = 2
	};

	ParallelGravity();

	virtual void updateMtx(const TPos3f &rMtx);
	virtual bool calcOwnGravityVector(TVec3f *pDest, f32 *pScalar, const TVec3f &rPosition) const;

	void setPlane(const TVec3f &rPlaneUp, const TVec3f &rPlanePos);
	void setRangeBox(const TPos3f &);
	void setRangeCylinder(f32 radius, f32 height);
	void setRangeType(RANGE_TYPE rangeType);
	void setBaseDistance(f32 val);
	void setDistanceCalcType(DISTANCE_CALC_TYPE distanceCalcType);
	bool isInSphereRange(const TVec3f &rPosition, f32 *pScalar) const;
	bool isInBoxRange(const TVec3f &rPosition, f32 *pScalar) const;
	bool isInCylinderRange(const TVec3f &rPosition, f32 *pScalar) const;
	bool isInRange(const TVec3f &rPosition, f32 *pScalar) const;

	TPos3f mLocalMtx;                     // _28
	TPos3f mWorldMtx;                     // _58
	f32 mExtentX;                         // _88
	f32 mExtentY;						  // _8C
	f32 mExtentZ;						  // _90
	TVec3f mPlanePosition;                // _94
	TVec3f mPlaneUpVec;                   // _A0
	TVec3f mWorldPlanePosition;           // _AC
	TVec3f mWorldPlaneUpVec;              // _B8
	f32 mCylinderHeight;                  // _C4
	f32 mCylinderRadius;                  // _C8
	f32 mBaseDistance;                    // _CC
	RANGE_TYPE mRangeType;                // _D0
	DISTANCE_CALC_TYPE mDistanceCalcType; // _D4
};
