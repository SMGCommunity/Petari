#include "Game/Gravity.hpp"
#include "Game/Util.hpp"

SegmentGravity::SegmentGravity() :
	PlanetGravity(),
	mSideVector(1, 0, 0),
	_64(1, 0, 0),
	_70(1, 0, 0),
	_7C(0, 0, 0)
{
	_88 = 0.0f;
	mSideDegree = 360.0f;
	_90 = -1.0f;

	for (u32 i = 0 ; i < 2 ; i++) {
		mGravityPoints[i].zero();
		_40[i].zero();
		_94[i] = 0.0f;
		mEdges[i] = true;
	}
}

void SegmentGravity::setGravityPoint(u32 index, const TVec3f &rGravityPoint) {
	mGravityPoints[index != 0] = rGravityPoint;
	updateLocalParam();
}

void SegmentGravity::setSideVector(const TVec3f &rSideVec) {
	mSideVector = rSideVec;
	MR::normalizeOrZero(&mSideVector);
	updateLocalParam();
}

void SegmentGravity::setValidSideDegree(f32 val) {
	mSideDegree = val;;
	updateLocalParam();
}

void SegmentGravity::setEdgeValid(u32 index, bool val) {
	mEdges[index != 0] = val;
}
