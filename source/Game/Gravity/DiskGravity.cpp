#include "Game/Gravity.h"
#include "Game/Util.h"

DiskGravity::DiskGravity() :
	PlanetGravity(),
	mLocalPosition(0.0f, 50.0f, 0.0f),
	mTranslation(0.0f, 50.0f, 0.0f),
	mLocalDirection(0, 1, 0),
	mRotation(0, 1, 0),
	mSideDirection(1, 0, 0),
	_64(1, 0, 0),
	_70(1, 0, 0)
{
	mRadius = 2500.0f;
	_80 = 2500.0f;
	mDegree = 360.0f;
	_88 = -1.0f;
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
	mDegree = val;
	updateLocalParam();
}

void DiskGravity::setBothSide(bool val) {
	mEnableBothSide = val;
}

void DiskGravity::setEnableEdgeGravity(bool val) {
	mEnableEdgeGravity = val;
}

void DiskGravity::updateMtx(const TPos3f &rMtx) {
	rMtx.mult(mLocalPosition, mTranslation);
	rMtx.mult33(mLocalDirection, mRotation);
	rMtx.mult33(_64, _70);

	f32 _8;
	MR::separateScalarAndDirection(&_8, &mRotation, mRotation);
	_80 = mRadius * _8;
}
