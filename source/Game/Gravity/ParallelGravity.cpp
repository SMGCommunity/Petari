#include "Game/Gravity.h"
#include "Game/Util.h"
#include "JSystem/JMath.h"

ParallelGravity::ParallelGravity() :
	PlanetGravity(),
	mPlanePosition(0, 0, 0),
	mPlaneUpVec(0.0f, 1.0f, 0.0f),
	mWorldPlanePosition(0, 0, 0),
	mWorldPlaneUpVec(0.0f, 1.0f, 0.0f)
{
	mCylinderHeight = 1000.0f;
	mCylinderRadius = 500.0f;
	mBaseDistance = 2000.0f;
	mRangeType = RangeType_Sphere;
	mDistanceCalcType = DistanceCalcType_Default;
	mLocalMtx.identity();
	mWorldMtx.identity();
}

bool ParallelGravity::calcOwnGravityVector(TVec3f *pDest, f32 *pScalar, const TVec3f &rPosition) const {
	if (!isInRange(rPosition, pScalar)) {
		return false;
	}

	if (pDest) {
		*pDest = -mWorldPlaneUpVec;
	}

	return true;
}

#ifdef NON_MATCHING
void ParallelGravity::updateMtx(const TPos3f &rMtx) {
	rMtx.mult33Inline(mPlaneUpVec, mWorldPlaneUpVec);
	rMtx.mult(mPlanePosition, mWorldPlanePosition);
	MR::normalizeOrZero(&mWorldPlaneUpVec);

	if (mRangeType == RangeType_Box) {
		mWorldMtx.concat(rMtx, mLocalMtx);

		TVec3f tempDir;
		mWorldMtx.getXDir(tempDir);
		mExtentX = tempDir.squared();
		mWorldMtx.getYDir(tempDir);
		mExtentY = tempDir.squared();
		mWorldMtx.getZDir(tempDir);
		mExtentZ = tempDir.squared();
	}
}
#endif

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

bool ParallelGravity::isInBoxRange(const TVec3f &rPosition, f32 *pScalar) const {
	// Get direction to center
	TVec3f translation;
	mWorldMtx.getTransInline(translation);
	TVec3f dirToCenter(rPosition - translation);

	// Check in X direction
	TVec3f dirX;
	mWorldMtx.getXDir(dirX);
	f32 dotX = dirToCenter.dot(dirX);

	if (dotX < -mExtentX || mExtentX < dotX)
		return false;

	// Check in Y direction
	TVec3f dirY;
	mWorldMtx.getYDir(dirY);
	f32 dotY = dirToCenter.dot(dirY);

	if (dotY < -mExtentY || mExtentY < dotY)
		return false;

	// Check in Z direction
	TVec3f dirZ;
	mWorldMtx.getYDir(dirZ);
	f32 dotZ = dirToCenter.dot(dirZ);

	if (dotZ < -mExtentZ || mExtentZ < dotZ)
		return false;

	// Calculate distance scalar
	if (pScalar) {
		f32 abs;
		switch (mDistanceCalcType) {
		case DistanceCalcType_X:
			abs = __fabsf(dotX);
			*pScalar = mBaseDistance + abs / MR::sqrt(mExtentX);
			break;
		case DistanceCalcType_Y:
			abs = __fabsf(dotY);
			*pScalar = mBaseDistance + abs / MR::sqrt(mExtentY);
			break;
		case DistanceCalcType_Z:
			abs = __fabsf(dotZ);
			*pScalar = mBaseDistance + abs / MR::sqrt(mExtentZ);
			break;
		case DistanceCalcType_Default:
			*pScalar = mBaseDistance;
			break;
		}
	}

	return true;
}

bool ParallelGravity::isInCylinderRange(const TVec3f &rPosition, f32 *pScalar) const {
	TVec3f v12;

	// Check height range
	TVec3f v11(rPosition - mWorldPlanePosition);
	f32 v6 = mWorldPlaneUpVec.dot(v11);

	if (v6 < 0.0f || mCylinderHeight < v6) {
		return false;
	}

	// Check radius range
	TVec3f v10(rPosition - mWorldPlanePosition);
	f32 v8 = mWorldPlaneUpVec.dot(v10);
	JMAVECScaleAdd(reinterpret_cast<const Vec*>(&mWorldPlaneUpVec), reinterpret_cast<const Vec*>(&v10), reinterpret_cast<Vec*>(&v12), -v8);

	f32 radius = VECMag(reinterpret_cast<const Vec*>(&v12));

	if (radius > mCylinderRadius)
		return false;

	// Set speed
	*pScalar = mBaseDistance + radius;

	return true;
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
