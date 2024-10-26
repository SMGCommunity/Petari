#include "Game/Gravity.hpp"
#include "Game/Util.hpp"

PlanetGravity::PlanetGravity() {
	mRange = -1.0f;
	mDistant = 0.0f;
	mPriority = 0;
	mGravityId = -1;
	mHost = this;
	mGravityType = GRAVITY_TYPE_NORMAL;
	mGravityPower = GRAVITY_POWER_NORMAL;
	mActivated = true;
	mIsInverse = false;
	mValidFollower = true;
	mIsRegistered = false;
	mAppeared = true;
}

void PlanetGravity::setPriority(s32 priority) {
	mPriority = priority;
}

bool PlanetGravity::calcGravity(TVec3f *pDest, const TVec3f &rPosition) const {
	// Calculate raw gravity vector
	f32 radius = 0.0f;
	TVec3f gravity;
	gravity.x = 0.0f;
	gravity.y = 0.0f;
	gravity.z = 0.0f;

	if (!calcOwnGravityVector(&gravity, &radius, rPosition))
		return false;

	// Adjust radius
	radius -= mDistant;

	if (radius < 1.0f) {
		radius = 1.0f;
	}

	// Apply gravity speed
	gravity.scaleInline(4000000.0f / (radius * radius));

	// Invert vector if necessary
	if (mIsInverse) {
		TVec3f inverse;
		inverse.negateInline(gravity);
		gravity = inverse;
	}

	// Set result vector
	pDest->x = gravity.x;
	pDest->y = gravity.y;
	pDest->z = gravity.z;

	return true;
}

bool PlanetGravity::isInRangeSquare(f32 radius) const {
	f32 range = mRange;

	if (range < 0.0f) {
		return true;
	}
	else {
		f32 distance = range + mDistant;
		return radius < distance * distance;
	}
}

bool PlanetGravity::isInRangeDistance(f32 radius) const {
	f32 range = mRange;

	if (range < 0.0f) {
		return true;
	}
	else {
		f32 distance = range + mDistant;
		return radius < distance;
	}
}

bool PlanetGravity::calcGravityFromMassPosition(TVec3f *pDirection, f32 *pScalar, const TVec3f &rPosition, const TVec3f &rMassPosition) const {
	TVec3f direction;
	f32 scalar;

	direction.subInline(rMassPosition, rPosition);
	MR::separateScalarAndDirection(&scalar, &direction, direction);

	if (!isInRangeDistance(scalar))
		return false;

	if (pDirection) {
		*pDirection = direction;
	}
	if (pScalar) {
		*pScalar = scalar;
	}

	return true;
}

void PlanetGravity::updateIdentityMtx() {
	TPos3f mtx;
	mtx.identity();
	updateMtx(mtx);
}
