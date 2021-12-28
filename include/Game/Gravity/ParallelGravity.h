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
		DistanceCalcType_0 = 0,
		DistanceCalcType_1 = 1,
		DistanceCalcType_2 = 2
	};

	ParallelGravity();

	virtual void updateMtx(const TPos3f &rMtx);
	virtual bool calcOwnGravityVector(TVec3f *pDest, f32 *pScalar, const TVec3f &rPosition) const;

	void setPlane(const TVec3f &, const TVec3f &);
	void setRangeBox(const TPos3f &);
	void setRangeCylinder(f32, f32);
	void setRangeType(RANGE_TYPE rangeType);
	void setBaseDistance(f32 val);
	void setDistanceCalcType(DISTANCE_CALC_TYPE distanceCalcType);
	bool isInSphereRange(const TVec3f &, f32 *) const;
	bool isInBoxRange(const TVec3f &, f32 *) const;
	bool isInCylinderRange(const TVec3f &, f32 *) const;
	bool inInRange(const TVec3f &, f32 *) const;

	TPos3f _28;
	TPos3f _58;
	f32 _88;
	f32 _8C;
	f32 _90;
	TVec3f _94;
	TVec3f _A0;
	TVec3f _AC;
	TVec3f _B8;
	f32 mRangeCylinder1;                  // _C4
	f32 mRangeCylinder2;                  // _C8
	f32 mBaseDistance;                    // _CC
	RANGE_TYPE mRangeType;                // _D0
	DISTANCE_CALC_TYPE mDistanceCalcType; // _D4
};
