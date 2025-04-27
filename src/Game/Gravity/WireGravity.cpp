#include "Game/Gravity.hpp"
#include "Game/Util.hpp"

WireGravity::WireGravity() {
    
}

void WireGravity::setPointListSize(u32 numPoints) {
	mPoints.init(numPoints);
	mPoints.clear();
}

void WireGravity::addPoint(const TVec3f &rPoint) {
	mPoints.mArray.mArr[mPoints.mCount++] = rPoint;
}

/*
bool WireGravity::calcOwnGravityVector(TVec3f *pDest, f32 *pScalar, const TVec3f &rPos) const {

    f32 distance = -1.0f;
    TVec3f pointOfAttraction;
    for(s32 i = 0; i < mCount - 1; i++) {

        // unused
        TVec3f wireBegin(mPoints[i]);
        TVec3f wireEnd(mPoints[i + 1]);


        TVec3f positionProjectedOntoWire;
        MR::calcPerpendicFootToLineInside(&positionProjectedOntoWire, rPos, mPoints[i], mPoints[i + 1]);

        f32 squareDistance = rPos.squareDistancePS(positionProjectedOntoWire);
        if(squareDistance < distance || distance < 0.0f) {

            pointOfAttraction = positionProjectedOntoWire;
            distance = squareDistance;

        }
    }

    if(!isInRangeSquare(distance)) {
        return false;
    }

    if(distance >= 0.0f) {
        TVec3f gravity(pointOfAttraction);
        gravity.subInline4(rPos);

        MR::separateScalarAndDirection(pScalar, pDest, gravity);

        return true;
    }

    return false;
}
*/
