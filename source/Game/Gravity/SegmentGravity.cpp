#include "Game/Gravity.hpp"
#include "Game/Util.hpp"
#include <JSystem/JMath/JMATrigonometric.hpp>

SegmentGravity::SegmentGravity() :
	PlanetGravity(),
	mSideVector(1, 0, 0),
	mOppositeSideVecOrtho(1, 0, 0),
	mWorldOppositeSideVecOrtho(1, 0, 0),
	mAxis(0, 0, 0)
{
	mAxisLength = 0.0f;
	mValidSideDegree = 360.0f;
	mValidSideCos = -1.0f;

	for (u32 i = 0 ; i < 2 ; i++) {
		mGravityPoints[i].zero();
		mWorldGravityPoints[i].zero();
		mHemisphereExtension[i] = 0.0f;
		mEdges[i] = true;
	}
}

bool SegmentGravity::calcOwnGravityVector(TVec3f *pDest, f32 *pScalar, const TVec3f &rPos) const {
    TVec3f relPosFromBase = rPos - mWorldGravityPoints[0];
    f32 axisY = relPosFromBase.dot(mAxis);
    if(-1.0f < mValidSideCos) {
        bool cmp = mWorldOppositeSideVecOrtho.squaredInline() <= 3.81469727e-06f;
        if(!cmp) {
            TVec3f dirOnBasePlane = relPosFromBase - mAxis * axisY;
            MR::normalizeOrZero(&dirOnBasePlane);
            if(dirOnBasePlane.dot(mWorldOppositeSideVecOrtho) < mValidSideCos) {
                return false;
            }
        }
    }
    TVec3f attraction;
    if(axisY < mHemisphereExtension[0]) {
        if(!mEdges[0]) {
            return false;
        }
        else {
            attraction = mWorldGravityPoints[0];
        }
    }
    else {
        if(axisY > mAxisLength - mHemisphereExtension[1]) {
            if(!mEdges[1]) {
                return false;
            }
            attraction = mWorldGravityPoints[1];
        }
        else {
            attraction = mWorldGravityPoints[0].madd(axisY, mAxis);
            
        }
    }
    TVec3f gravity = attraction - rPos;
    f32 distance;
    MR::separateScalarAndDirection(&distance, &gravity, gravity);
    if(!isInRangeDistance(distance)) {
        return false;
    }
    if(pDest) {
        *pDest = gravity;
    }
    if(pScalar) {
        *pScalar = distance;
    }
    return true;
}


void SegmentGravity::updateLocalParam() {
    TRot3f rot;

    // Both of these variables are present because the codegen indicates they should be.
    // In the final game, however, they have no behavioral effect and are not given any memory.
    bool artifact = true;
    bool &rArtifact = artifact;

    mValidSideCos = JMath::sSinCosTable.cosLap(0.5f * mValidSideDegree);

    TVec3f localAxis = mGravityPoints[1] - mGravityPoints[0];
    MR::normalizeOrZero(&localAxis);
    if(MR::isNearZero(localAxis, 0.00100000005f)) {
        rArtifact = false;
        mOppositeSideVecOrtho.zero();
        return;
    }
    JMAVECScaleAdd(localAxis.toCVec(), mSideVector.toCVec(), mOppositeSideVecOrtho.toVec(), -localAxis.dot(mSideVector));
    MR::normalize(&mOppositeSideVecOrtho);
    if(MR::isNearZero(mOppositeSideVecOrtho, 0.00100000005f)) {
        mOppositeSideVecOrtho.zero();
        return;
    }
    
    rot.identity();
    rot.setRotateInline2(localAxis, 0.5f * mValidSideDegree * (PI / 180));

    // Necessary to force the compiler to load the arguments to `mult` in the correct order
    rArtifact = false;
    if(!artifact) {
        rot.mult(mOppositeSideVecOrtho, mOppositeSideVecOrtho);
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
	mValidSideDegree = val;;
	updateLocalParam();
}

void SegmentGravity::setEdgeValid(u32 index, bool val) {
	mEdges[index != 0] = val;
}

void SegmentGravity::updateMtx(const TPos3f &rMtx) {
    for(s32 i = 0; i < 2; i++) {
        rMtx.mult(mGravityPoints[i], mWorldGravityPoints[i]);
    }
    rMtx.mult33Inline(mOppositeSideVecOrtho, mWorldOppositeSideVecOrtho);
    mAxis = mWorldGravityPoints[1] - mWorldGravityPoints[0];
    MR::separateScalarAndDirection(&mAxisLength, &mAxis, mAxis);
}
