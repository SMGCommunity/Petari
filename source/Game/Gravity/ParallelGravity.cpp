#include "Game/Gravity.h"
#include "Game/Util.h"

ParallelGravity::ParallelGravity() :
	PlanetGravity(),
	mPlanePosition(0, 0, 0),
	mPlaneUpVec(0.0f, 1.0f, 0.0f),
	mWorldPlanePosition(0, 0, 0),
	_B8(0.0f, 1.0f, 0.0f)
{
	mCylinderHeight = 1000.0f;
	mCylinderRadius = 500.0f;
	mBaseDistance = 2000.0f;
	mRangeType = RangeType_Sphere;
	mDistanceCalcType = DistanceCalcType_Default;
	_28.identity();
	_58.identity();
}

bool ParallelGravity::calcOwnGravityVector(TVec3f *pDest, f32 *pScalar, const TVec3f &rPosition) const {
	if (!isInRange(rPosition, pScalar)) {
		return false;
	}

	if (pDest) {
		*pDest = -_B8;
	}

	return true;
}

void ParallelGravity::setPlane(const TVec3f &rPlaneUp, const TVec3f &rPlanePos) {
	// Up vector
	mPlaneUpVec.setInline(rPlaneUp);
	VECMag(reinterpret_cast<const Vec*>(&mPlaneUpVec)); // unused result
	VECNormalize(reinterpret_cast<const Vec*>(&mPlaneUpVec), reinterpret_cast<Vec*>(&mPlaneUpVec));

	// Position
	mPlanePosition = rPlanePos;
}

void ParallelGravity::setRangeCylinder(f32 radius, f32 height) {
	mCylinderRadius = radius;
	mCylinderHeight = height;
}

void ParallelGravity::setRangeType(RANGE_TYPE rangeType) {
	mRangeType = rangeType;
}

void ParallelGravity::setBaseDistance(f32 val) {
	if (val >= 0.0f) {
		mBaseDistance = val;
	}
	else {
		mBaseDistance = 0.0f;
	}
}

void ParallelGravity::setDistanceCalcType(DISTANCE_CALC_TYPE distanceCalcType) {
	mDistanceCalcType = distanceCalcType;
}

bool ParallelGravity::isInSphereRange(const TVec3f &rPosition, f32 *pScalar) const {
	if (pScalar) {
		*pScalar = mBaseDistance;
	}

	if (mRange < 0.0f) {
		return true;
	}
	else {
		TVec3f dirToCenter(mWorldPlanePosition - rPosition);
		f32 range = mRange;
		return dirToCenter.squared() < range * range;
	}
}

bool ParallelGravity::isInRange(const TVec3f &rPosition, f32 *pScalar) const {
	switch (mRangeType) {
	case RangeType_Sphere:
		return isInSphereRange(rPosition, pScalar);
	case RangeType_Box:
		return isInBoxRange(rPosition, pScalar);
	case RangeType_Cylinder:
		return isInCylinderRange(rPosition, pScalar);
	}

	return false;
}
