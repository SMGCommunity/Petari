#include "Game/Gravity.hpp"
#include "Game/Util.hpp"

PointGravity::PointGravity() :
	PlanetGravity(),
	mOrigPosition(0, 0, 0),
	mTranslation(0, 0, 0)
{

}

inline TVec3f sub(const TVec3f &mTranslation, const TVec3f &rPosition) {
TVec3f direction;
    direction.setPS(mTranslation);
    /*direction.x = mTranslation.x;
    direction.y = mTranslation.y;
    direction.z = mTranslation.z;*/
	JMathInlineVEC::PSVECSubtract(&direction, &rPosition, &direction);
    return direction;
}

bool PointGravity::calcOwnGravityVector(TVec3f *pDest, f32 *pScalar, const TVec3f &rPosition) const {
	TVec3f direction = sub(mTranslation, rPosition);
	f32 scalar = PSVECMag(&direction);

	// Epsilon-equals zero? If so, direction is the zero vector.
	if (MR::isNearZero(scalar, 0.01f)) {
		direction.z = 0.0f;
		direction.y = 0.0f;
		direction.x = 0.0f;
	}
	// Otherwise, it's a proper direction and it should be normalized.
	else {
        f32 trueScalar = 1.0f / scalar;
		direction.x *= trueScalar;
		direction.y *= trueScalar;
		direction.z *= trueScalar;
	}

	// Out of range? If so, don't update result vector & scalar
	if (!isInRangeDistance(scalar)) {
		return false;
	}
	// Update result vector & scalar if applicable
	else {
		if (pDest) {
			pDest->setPS2(direction);
		}
		if (pScalar) {
			*pScalar = scalar;
		}

		return true;
	}
}
	

void PointGravity::updateMtx(const TPos3f &rMtx) {
	// Converts relative zone-coordinates into worldspace
	// mTranslation = rMtx * mOrigPosition
	rMtx.mult(mOrigPosition, mTranslation);
}
