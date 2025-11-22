#include "Game/Boss/TripodBossMovableArea.hpp"
#include "Game/Util/MathUtil.hpp"
#include "JSystem/JGeometry/TVec.hpp"

namespace {
    // unused
    static TVec3f sTestHitPosition(0, 0, 0);
    static TVec3f sTestHitVerocity(0, 0, 0);
    static TVec3f sTestLandPosition(0, 0, 0);
}; // namespace

HitResult::HitResult() {
    _0.set< int >(0, 0, 0);
    _C.x = 0.0f;
    _C.y = 0.0f;
    _C.z = 0.0f;
    _18.x = 0.0f;
    _18.y = 1.0f;
    _18.z = 0.0f;
}

TripodBossMovableArea::TripodBossMovableArea() {
    mCenter.x = 0.0f;
    mCenter.y = 0.0f;
    mCenter.z = 0.0f;
    mBaseAxis.x = 0.0f;
    mBaseAxis.y = 1.0f;
    mBaseAxis.z = 0.0f;
    mFront.x = 0.0f;
    mFront.y = 0.0f;
    mFront.z = 1.0f;
    mRadius = 3000.0f;
    _2C = -1.0f;
    _30 = 1.0f;
}

void TripodBossMovableArea::setCenter(const TVec3f& rCenter) {
    mCenter = rCenter;
}

void TripodBossMovableArea::setBaseAxis(const TVec3f& rAxis) {
    mBaseAxis = rAxis;
    MR::normalizeOrZero(&mBaseAxis);
}

void TripodBossMovableArea::setFrontVector(const TVec3f& rFront) {
    mFront = rFront;
    MR::normalizeOrZero(&mBaseAxis);
}

void TripodBossMovableArea::setRadius(f32 radius) {
    mRadius = radius;
}

bool TripodBossMovableArea::collideSphere(HitResult* pResult, const TVec3f& a2, f32 a3, const TVec3f& a4) const {
    pResult->_C = a2 + a4;
    f32    v10 = mRadius + a3;
    TVec3f v29(a2);
    v29 -= mCenter;
    TVec3f direction(a4);
    f32    scalar;
    MR::separateScalarAndDirection(&scalar, &direction, direction);
    f32 v11 = v29.dot(direction);
    f32 v12 = v29.dot(v29);

    f32 val = v12 - (v10 * v10);

    if (v11 > 0.0f && val > 0.0f) {
        return false;
    }

    f32 v14 = ((v11 * v11) - val);
    if (v14 < 0.0f) {
        return false;
    }

    f32 v15 = (-v11 - MR::sqrt< f32 >(v14));
    if (v15 > scalar) {
        return false;
    }

    if (v15 < 0.0f) {
        v15 = 0.0f;
    }

    pResult->_0 = a2 + (direction * v15);
    TVec3f v27(pResult->_0);
    v27 -= mCenter;
    f32 v17;
    MR::separateScalarAndDirection(&v17, &v27, v27);
    if (v17 < v10) {
        if (MR::isNearZero(v17, 0.001f)) {
            v27.set< int >(0, 1, 0);
        }

        pResult->_0 = mCenter + (v27 * v10);
    }

    pResult->_18 = v27;
    TVec3f v26(pResult->_C);
    v26 -= pResult->_0;
    f32 v16 = v26.dot(pResult->_18);
    v26 -= pResult->_18 * v16;
    pResult->_C = pResult->_0 + v26;
    return true;
}

void TripodBossMovableArea::calcNearLandingPosition(TVec3f* pPos, const TVec3f& a2) const {
    f32    v6, v8, x, v7;
    TVec3f v16(a2);
    v16 -= mCenter;
    MR::separateScalarAndDirection(&v8, &v16, v16);

    if (MR::isNearZero(v8, 0.001f)) {
        v16 = mBaseAxis;
    }

    x = mBaseAxis.dot(v16);
    if (MR::isInRange(x, _2C, _30)) {
        pPos->set< f32 >(mCenter + (v16 * mRadius));
    } else {
        if (x < _2C) {
            x = _2C;
        } else {
            if (x > _30) {
                x = _30;
            }
        }

        TVec3f v15(v16);
        v6 = mBaseAxis.dot(v16);
        JMAVECScaleAdd(&mBaseAxis, &v16, &v15, -v6);
        if (MR::isNearZero(v15, 0.001f)) {
            v15.zero();
            int idx = MR::getMinAbsElementIndex(mBaseAxis);
            (&v15.x)[idx] = 1.0f;
        }

        MR::normalizeOrZero(&v15);
        v7 = MR::sqrt< f32 >((1.0f - (x * x)));
        TVec3f sp48((mBaseAxis * x) + (v15 * v7));
        pPos->set< f32 >(sp48 * mRadius);
    }
}

void TripodBossMovableArea::calcLandingNormal(TVec3f* pNorm, const TVec3f& a2) const {
    TVec3f norm(a2);
    norm -= mCenter;
    MR::normalizeOrZero(&norm);
    if (MR::isNearZero(norm, 0.001f)) {
        norm = mBaseAxis;
    }

    pNorm->set< f32 >(norm);
}

void TripodBossMovableArea::calcLandingFront(TVec3f* pFront, const TVec3f& a2) const {
    TVec3f v11(a2);
    v11 -= mCenter;
    MR::normalizeOrZero(&v11);
    TVec3f v10;
    TVec3f v9;
    JMathInlineVEC::PSVECNegate(&mBaseAxis, &v9);
    f32 v7 = v11.dot(v9);
    JMAVECScaleAdd(&v11, &v9, &v10, -v7);
    MR::normalizeOrZero(&v10);
    if (MR::isNearZero(v10, 0.001f)) {
        v10.zero();
        int idx = MR::getMinAbsElementIndex(mBaseAxis);
        (&v10.x)[idx] = 1.0f;
        f32 v8 = mBaseAxis.dot(v10);
        JMAVECScaleAdd(&mBaseAxis, &v10, &v10, -v8);
        MR::normalizeOrZero(&v10);
    }

    pFront->set< f32 >(v10);
}
