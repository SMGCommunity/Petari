#include "Game/Gravity.hpp"
#include "Game/Util.hpp"
#include "JSystem/JMath.hpp"
#include "Inline.hpp"

/*
ParallelGravity::ParallelGravity() :
	PlanetGravity(),
	mPlanePosition(0, 0, 0),
	mWorldPlanePosition(0, 0, 0),
{
	mCylinderHeight = 1000.0f;
	mCylinderRadius = 500.0f;
	mBaseDistance = 2000.0f;
	mRangeType = RangeType_Sphere;
	mDistanceCalcType = DistanceCalcType_Default;
	mLocalMtx.identity();
	mWorldMtx.identity();
}
	*/

bool ParallelGravity::calcOwnGravityVector(TVec3f *pDest, f32 *pScalar, const TVec3f &rPosition) const {
	if (!isInRange(rPosition, pScalar)) {
		return false;
	}

	if (pDest) {
		*pDest = -mWorldPlaneUpVec;
	}

	return true;
}

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

void ParallelGravity::setPlane(const TVec3f &rPlaneUp, const TVec3f &rPlanePos) {
	// Up vector
	mPlaneUpVec.setPS(rPlaneUp);
	PSVECMag(&mPlaneUpVec); // unused result
	PSVECNormalize(&mPlaneUpVec, &mPlaneUpVec);

	// Position
	mPlanePosition = rPlanePos;
}

void ParallelGravity::setRangeBox(const TPos3f &rMtx) {
    mLocalMtx = rMtx;
    updateIdentityMtx();
}

void ParallelGravity::setRangeCylinder(f32 radius, f32 height) {
	mCylinderRadius = radius;
	mCylinderHeight = height;
}

void ParallelGravity::setRangeType(RANGE_TYPE rangeType) {
	mRangeType = rangeType;
}

void ParallelGravity::setBaseDistance(f32 val) {
	if (val < 0.0f) {
		mBaseDistance = 2000.0f;
	}
	else {
		mBaseDistance = val;
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

/*
bool ParallelGravity::isInCylinderRange(const TVec3f &rPosition, f32 *pScalar) const {
	f32 height = mWorldPlaneUpVec.dot(rPosition - mWorldPlanePosition);

	if (height < 0.0f || mCylinderHeight < height) {
		return false;
	}
    
	TVec3f positionOnWorldPlane;

	// Check radius range
	positionOnWorldPlane.rejection(rPosition - mWorldPlanePosition, mWorldPlaneUpVec);

	f32 radius = PSVECMag(positionOnWorldPlane);

	if (radius > mCylinderRadius) {
		return false;
    }
    
	// Set speed
	*pScalar = mBaseDistance + radius;

	return true;
}
*/

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
