#pragma once

#include "Game/Gravity/PlanetGravity.hpp"

class SegmentGravity : public PlanetGravity {
public:
	SegmentGravity();

	virtual void updateMtx(const TPos3f &rMtx);
	virtual bool calcOwnGravityVector(TVec3f *pDest, f32 *pScalar, const TVec3f &rPosition) const;

	void setGravityPoint(u32 index, const TVec3f &rGravityPoint);
	void setSideVector(const TVec3f &rSideVec);
	void setValidSideDegree(f32 val);
	void setEdgeValid(u32 index, bool val);
	void updateLocalParam();

	TVec3f mGravityPoints[2]; // _28
	TVec3f _40[2];
	TVec3f mSideVector;       // _58
	TVec3f _64;
	TVec3f _70;
	TVec3f _7C;
	f32 _88;
	f32 mSideDegree;          // _8C
	f32 _90;
	f32 _94[2];
	bool mEdges[2];           // _9C
};
