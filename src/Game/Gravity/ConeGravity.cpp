#include "Game/Gravity.hpp"

ConeGravity::ConeGravity() : PlanetGravity() {
	mValidDegree = 360.0f;
	mValidCos = -1.0f;
	mWorldRadius = 0.0f;
	mEnableBottom = true;
	mTopCutRate = 0.0f;
	mLocalMtx.identity();
	mWorldMtx.identity();
}

void ConeGravity::setLocalMatrix(const TPos3f &rMtx) {
	mLocalMtx.setInline(rMtx);
	updateIdentityMtx();
}

void ConeGravity::setEnableBottom(bool val) {
	mEnableBottom = val;
}

void ConeGravity::setTopCutRate(f32 val) {
	mTopCutRate = val < 0.0f ? 0.0f : val > 1.0f ? 1.0f : val;
}

inline f32 absfInline(f32 &orig, f32 v) {
    orig = v;
    return __fabsf(v);
}

/*
bool ConeGravity::calcOwnGravityVector(TVec3f *pDest, f32 *pScalar, const TVec3f &rPos) const {
    
    TVec3f worldBaseCenter, worldCentralAxis;

    mWorldMtx.getYDir(worldCentralAxis);
    mWorldMtx.getTransInline(worldBaseCenter);
    
    TVec3f unitWorldCentralAxis;
    f32 centralAxisLength;
    MR::separateScalarAndDirection(&centralAxisLength, &unitWorldCentralAxis, worldCentralAxis);

    TVec3f relativePosition = rPos - worldBaseCenter;
    TVec3f positionOnBasePlane;
    positionOnBasePlane.rejection(relativePosition, unitWorldCentralAxis);

    if(MR::isNearZero(positionOnBasePlane, 0.00100000005f)) {
        
        f32 positionOnCentralAxis;
        f32 distance = absfInline(positionOnCentralAxis, relativePosition.dot(unitWorldCentralAxis));

        if(positionOnCentralAxis > 0.0f) {

            f32 height = centralAxisLength * (1.0f - mTopCutRate);

            distance -= height;
            if(distance < 0.0f) {
                distance = 0.0f;
            }
            
        }
        
        if(!isInRangeDistance(distance)) {
            return false;
        }

        if(positionOnCentralAxis > 0.0f) {
            pDest->set(-unitWorldCentralAxis);
        }
        else {
            pDest->set(unitWorldCentralAxis);
        }

        *pScalar = distance;
        return true;
    }
    
    f32 distanceToCentralAxis = PSVECMag(positionOnBasePlane);
    f32 centralAxisY = unitWorldCentralAxis.dot(relativePosition);

    bool isInsideCone = false;
    
    if(MR::isNearZero(centralAxisLength, 0.00100000005f) || MR::isNearZero(mWorldRadius, 0.00100000005f)) {
        // The cone is too small to have an inner region
        isInsideCone = false;
    }
    else if(distanceToCentralAxis < mWorldRadius - centralAxisY * (mWorldRadius / centralAxisLength)) {
        isInsideCone = true;
    }

    TVec3f pointOfAttraction;
    
    TVec3f dirOnDirectrix = worldBaseCenter + positionOnBasePlane * (mWorldRadius / distanceToCentralAxis);

    TVec3f apex = worldBaseCenter + worldCentralAxis;

    if(relativePosition.dot(worldCentralAxis) < 0.0f) {

        if(!mEnableBottom) {
            return false;
        }

        MR::calcPerpendicFootToLineInside(&pointOfAttraction, rPos, worldBaseCenter, dirOnDirectrix);

        if(MR::isNearZero(pointOfAttraction - rPos, 0.00100000005f)) {

            *pDest = -unitWorldCentralAxis;
            *pScalar = 0.0f;

            return true;
        }

        else {
            return calcGravityFromMassPosition(pDest, pScalar, rPos, pointOfAttraction);
        }

    }


    if(mTopCutRate < 0.00999999978f) {
        MR::calcPerpendicFootToLineInside(&pointOfAttraction, rPos, dirOnDirectrix, apex);
    }
    else {

        TVec3f generatrixTermination;
        generatrixTermination.set(apex * (1.0f - mTopCutRate) + dirOnDirectrix * mTopCutRate);

        TVec3f frustumBaseCenter;
        frustumBaseCenter.set(worldBaseCenter + worldCentralAxis * (1.0f - mTopCutRate));

        if((rPos - generatrixTermination).dot(generatrixTermination - frustumBaseCenter) <= 0.0f) {

            // Attracted to the frustum
            f32 distanceToCentralAxis = unitWorldCentralAxis.dot(rPos - frustumBaseCenter);

            if(distanceToCentralAxis < 0.0f) {
                distanceToCentralAxis = 0.0f;
            }

            if(isInRangeDistance(distanceToCentralAxis)) {
                *pDest = -unitWorldCentralAxis;
                *pScalar = distanceToCentralAxis;
                return true;
            }
            else {
                return false;
            }
        }

        MR::calcPerpendicFootToLineInside(&pointOfAttraction, rPos, dirOnDirectrix, generatrixTermination);
    }


    if(MR::isNearZero(pointOfAttraction - rPos, 0.00100000005f)) {

        TVec3f generatrixDirection = apex - dirOnDirectrix;
        MR::normalizeOrZero(&generatrixDirection);

        TVec3f gravity;
        gravity.rejection(-positionOnBasePlane, generatrixDirection);

        if(MR::isNearZero(gravity, 0.00100000005f)) {
            *pDest = -unitWorldCentralAxis;
        }
        else {
            MR::normalize(gravity, pDest);
        }
        *pScalar = 0.0f;

        return true;
    }

    if(isInsideCone) {
        MR::normalize(rPos - pointOfAttraction, pDest);
        *pScalar = 0.0f;
        return true;
    }
    
    return calcGravityFromMassPosition(pDest, pScalar, rPos, pointOfAttraction);
}
*/

void ConeGravity::updateMtx(const TPos3f &rMtx) {
	mWorldMtx.concat(rMtx, mLocalMtx);

	TVec3f sideVec;
	mWorldMtx.getXDirInline(sideVec);
	mWorldRadius = PSVECMag(&sideVec);

	TVec3f axis; // unused
	mWorldMtx.getYDir(axis);
    // The developers could have left this in because there originally was a height member
    // that they would set to ||axis|| * (1.0f - mTopCutRate)
}
