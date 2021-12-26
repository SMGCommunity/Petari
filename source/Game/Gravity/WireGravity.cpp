#include "Game/Gravity/WireGravity.h"
#include "Game/Util.h"

WireGravity::WireGravity() {
	mCount = 0;
}

void WireGravity::setPointListSize(u32 numPoints) {
	mPoints.init(numPoints);
	mCount = 0;
}

void WireGravity::addPoint(const TVec3f &rPoint) {
	mPoints.mArr[mCount++] = rPoint;
}

/*bool WireGravity::calcOwnGravityVector(TVec3f *pDest, f32 *pScalar, const TVec3f &rPosition) const {

}*/
