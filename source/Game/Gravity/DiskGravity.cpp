#include "Game/Gravity.h"
#include "Game/Util.h"

DiskGravity::DiskGravity() :
	PlanetGravity(),
	mLocalPosition(0.0f, 50.0f, 0.0f),
	mTranslation(0.0f, 50.0f, 0.0f),
	mLocalDirection(0, 1, 0),
	mRotation(0, 1, 0),
	mSideDirection(1, 0, 0),
	mSideVecOrtho(1, 0, 0),
	mWorldSideDir(1, 0, 0)
{
	mRadius = 2500.0f;
	mWorldRadius = 2500.0f;
	mValidDegree = 360.0f;
	mValidCos = -1.0f;
	mEnableBothSide = true;
	mEnableEdgeGravity = true;
}

void DiskGravity::setLocalPosition(const TVec3f &rLocalPos) {
	mLocalPosition = rLocalPos;
}

void DiskGravity::setLocalDirection(const TVec3f &rLocalDir) {
	mLocalDirection.set(rLocalDir);
	MR::normalizeOrZero(&mLocalDirection);
	updateLocalParam();
}

void DiskGravity::setSideDirection(const TVec3f &rSideDir) {
	mSideDirection.set(rSideDir);
	updateLocalParam();
}

void DiskGravity::setRadius(f32 val) {
	mRadius = val;
}

void DiskGravity::setValidDegee(f32 val) {
	mValidDegree = val;
	updateLocalParam();
}

void DiskGravity::setBothSide(bool val) {
	mEnableBothSide = val;
}

void DiskGravity::setEnableEdgeGravity(bool val) {
	mEnableEdgeGravity = val;
}

#ifdef NON_MATCHING
bool DiskGravity::calcOwnGravityVector(TVec3f *pDest, f32 *pScalar, const TVec3f &rPosition) const {
	TVec3f dirToPos;
	dirToPos = rPosition - mTranslation;
	f32 dot = dirToPos.dot(mRotation);

	if (!mEnableBothSide && dot < 0.0f)
		return false;

	TVec3f v33 = dirToPos - mRotation * dot;
	f32 scalar;
	MR::separateScalarAndDirection(&scalar, &v33, v33);

	if (mValidCos > -1.0f && v33.dot(mWorldSideDir) < mValidCos)
		return false;

	TVec3f gravity(0, 0, 0);
	f32 distance = 0.0f;

	if (scalar <= mWorldRadius) {
		// This entire thing doesn't make much sense
		const TVec3f* grav;

		if (dot >= 0.0f) {
			TVec3f neg;
			neg.negateInline(mRotation);
			grav = &neg;
		}
		else {
			grav = &mRotation;
		}

		gravity = *grav;
		distance = __fabsf(dot);
	}
	else {
		if (!mEnableBothSide) {
			return false;
		}

		TVec3f v31;
		v31.set(v33 * mWorldRadius);
		v31.addInline2(mTranslation);

		gravity = v31 - rPosition;
		MR::separateScalarAndDirection(&distance, &gravity, gravity);
	}

	if (!isInRangeDistance(distance)) {
		return false;
	}

	// Set gravity and scalar results
	if (pDest) {
		*pDest = gravity;
	}
	if (pScalar) {
		*pScalar = scalar;
	}

	return true;
}
#endif

void DiskGravity::updateMtx(const TPos3f &rMtx) {
	rMtx.mult(mLocalPosition, mTranslation);
	rMtx.mult33(mLocalDirection, mRotation);
	rMtx.mult33(mSideVecOrtho, mWorldSideDir);

	f32 _8;
	MR::separateScalarAndDirection(&_8, &mRotation, mRotation);
	mWorldRadius = mRadius * _8;
}
