#include "Game/Gravity.hpp"
#include "Game/Util.hpp"
#include "Inline.hpp"
#include <JSystem/JMath/JMATrigonometric.hpp>
#include <math_types.hpp>

DiskGravity::DiskGravity() :
	PlanetGravity(),
	CALL_INLINE_FUNC(mLocalPosition, 0.0f, 50.0f, 0.0f),
	CALL_INLINE_FUNC(mWorldPosition, 0.0f, 50.0f, 0.0f),
	mLocalNormal(0, 1, 0),
	mWorldNormal(0, 1, 0),
	mSideDirection(1, 0, 0),
	mOppositeSideVecOrtho(1, 0, 0),
	mWorldOppositeSideVecOrtho(1, 0, 0)
{
	mLocalRadius = 2500.0f;
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
	mLocalNormal.set(rLocalDir);
	MR::normalizeOrZero(&mLocalNormal);
	updateLocalParam();
}

void DiskGravity::setSideDirection(const TVec3f &rSideDir) {
	mSideDirection.set(rSideDir);
	updateLocalParam();
}

void DiskGravity::setRadius(f32 val) {
	mLocalRadius = val;
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

bool DiskGravity::calcOwnGravityVector(TVec3f *pDest, f32 *pDistance, const TVec3f &rPosition) const {
    
	TVec3f relativePos;
	relativePos = rPosition - mWorldPosition;

    f32 centralAxisY = relativePos.dot(mWorldNormal);

	if (!mEnableBothSide && centralAxisY < 0.0f)
		return false;

	TVec3f dirOnDiskPlane = relativePos - mWorldNormal * centralAxisY;
	f32 distanceToCentralAxis;
	MR::separateScalarAndDirection(&distanceToCentralAxis, &dirOnDiskPlane, dirOnDiskPlane);

	if (mValidCos > -1.0f && dirOnDiskPlane.dot(mWorldOppositeSideVecOrtho) < mValidCos)
		return false;

	TVec3f gravity(0, 0, 0);
	f32 distance = 0.0f;

	if (distanceToCentralAxis <= mWorldRadius) {
		gravity = centralAxisY >= 0.0f ? mWorldNormal.negateInline_2() : mWorldNormal;
		distance = __fabsf(centralAxisY);
	}
	else {
		if (!mEnableEdgeGravity) {
			return false;
		}
        
		TVec3f closestEdgePoint;
		closestEdgePoint.set(dirOnDiskPlane * mWorldRadius);
		closestEdgePoint.addInline2(mWorldPosition);

		gravity = closestEdgePoint - rPosition;
		MR::separateScalarAndDirection(&distance, &gravity, gravity);
	}

	if (!isInRangeDistance(distance)) {
		return false;
	}

	// Set gravity and scalar results
	if (pDest) {
		*pDest = gravity;
	}
	if (pDistance) {
		*pDistance = distance;
	}

	return true;
}

void DiskGravity::updateLocalParam() {
    TRot3f rot;

    // Both of these variables are present because the codegen indicates they should be.
    // In the final game, however, they have no behavioral effect and are not given any memory.
    bool artifact = false;
    bool &rArtifact = artifact;

    mValidCos = JMath::sSinCosTable.cosLap(0.5f * mValidDegree);
    if(MR::isNearZero(mLocalNormal, 0.00100000005f)) {
        rArtifact = true;
        mOppositeSideVecOrtho.zero();
        return;
    }
    JMAVECScaleAdd(mLocalNormal.toCVec(), mSideDirection.toCVec(), mOppositeSideVecOrtho.toVec(), -mLocalNormal.dot(mSideDirection));
    MR::normalizeOrZero(&mOppositeSideVecOrtho);
    if(MR::isNearZero(mOppositeSideVecOrtho, 0.00100000005f)) {
        mOppositeSideVecOrtho.zero();
        return;
    }

    rot.identity();
    rot.setRotateInline(mLocalNormal, 0.5f * mValidDegree * (PI / 180));
    rArtifact = false;
    if(!artifact) {
        rot.mult(mOppositeSideVecOrtho, mOppositeSideVecOrtho);
    }
}

void DiskGravity::updateMtx(const TPos3f &rMtx) {
	rMtx.mult(mLocalPosition, mWorldPosition);
	rMtx.mult33(mLocalNormal, mWorldNormal);
	rMtx.mult33(mOppositeSideVecOrtho, mWorldOppositeSideVecOrtho);

	f32 axisScale;
	MR::separateScalarAndDirection(&axisScale, &mWorldNormal, mWorldNormal);
	mWorldRadius = mLocalRadius * axisScale;
}
