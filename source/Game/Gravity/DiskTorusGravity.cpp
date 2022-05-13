#include "Game/Gravity.h"
#include "Game/Util.h"
#include "Inline.h"

DiskTorusGravity::DiskTorusGravity() :
	PlanetGravity(),
    CALL_INLINE_FUNC(mLocalPosition, 0.0f, 50.0f, 0.0f),
    CALL_INLINE_FUNC(mTranslation, 0.0f, 50.0f, 0.0f),
	mLocalDirection(0, 1, 0),
	mRotation(0, 1, 0)
{
	mRadius = 2000.0f;
	_5C = 2000.0f;
	mDiskRadius = 0.0f;
	mEdgeType = 3;
	mEnableBothSide = true;
}

void DiskTorusGravity::setPosition(const TVec3f &rPosition) {
	mLocalPosition = rPosition;
}

void DiskTorusGravity::setDirection(const TVec3f &rDirection) {
	mLocalDirection = rDirection;
}

void DiskTorusGravity::setRadius(f32 val) {
	mRadius = val;
}

void DiskTorusGravity::setDiskRadius(f32 val) {
	if (val < 0.0f) {
		mDiskRadius = 0.0f;
	}
	else {
		mDiskRadius = val;
	}
}

void DiskTorusGravity::setEdgeType(s32 val) {
	mEdgeType = val;
}

void DiskTorusGravity::setBothSide(bool val) {
	mEnableBothSide = val;
}
