#include "Game/Gravity.hpp"
#include "Game/Util.hpp"

void PointGravity_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
}

PointGravity::PointGravity() : PlanetGravity(), mOrigPosition(0, 0, 0), mTranslation(0, 0, 0) {
}

bool PointGravity::calcOwnGravityVector(TVec3f* pDest, f32* pScalar, const TVec3f& rPosition) const {
    TVec3f direction = mTranslation - rPosition;
    f32 distance = direction.length();

    // Epsilon-equals zero? If so, direction is the zero vector.
    if (MR::isNearZero(distance)) {
        direction.z = 0.0f;
        direction.y = 0.0f;
        direction.x = 0.0f;
    }
    // Otherwise, it's a proper direction and it should be normalized.
    else {
        f32 scalar = 1.0f / distance;
        direction.x *= scalar;
        direction.y *= scalar;
        direction.z *= scalar;
    }

    // Out of range? If so, don't update result vector & scalar
    if (!isInRangeDistance(distance)) {
        return false;
    }
    // Update result vector & scalar if applicable
    else {
        if (pDest) {
            *pDest = direction;
        }
        if (pScalar) {
            *pScalar = distance;
        }

        return true;
    }
}

void PointGravity::updateMtx(const TPos3f& rMtx) {
    // Converts relative zone-coordinates into worldspace
    // mTranslation = rMtx * mOrigPosition
    rMtx.mult(mOrigPosition, mTranslation);
}
