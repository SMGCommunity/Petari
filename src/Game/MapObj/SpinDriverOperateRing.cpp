#include "Game/MapObj/SpinDriverOperateRing.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"

void SpinDriverOperateRing_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
    (void)2.0f;
}

namespace {
    // static const f32 sExpandOparateRingSpeed = 0.0f;
    // static const f32 sOparateRange = 0.0f;
    // static const f32 sOperateNearDistance = 0.0f;
    static const f32 sOperateAccel = 0.5f;
    // static const f32 sOperateFreq = 0.0f;
    static const f32 sToPlaneRate = 0.05f;
    // static const f32 sReturnAccel = 0.0f;
    static const f32 sDirectionRate = 0.1f;
};  // namespace

namespace NrvSpinDriverOperateRing {
    NEW_NERVE(SpinDriverOperateRingNrvWait, SpinDriverOperateRing, Wait);
};  // namespace NrvSpinDriverOperateRing

SpinDriverOperateRing::SpinDriverOperateRing(const char* pName)
    : LiveActor(pName), _8C(0, 0, 0), _98(0, 0, 0), _A4(0, 0, 0), mAccelerate(0, 0, 0), mDirection(0, 0, 0), _E0(), _C8(0.0f, 0.0f, 0.0f),
      mRadiusRate(), _D8(), _DC() {
}

void SpinDriverOperateRing::init(const JMapInfoIter& rIter) {
    makeActorAppeared();
    MR::invalidateClipping(this);
}

void SpinDriverOperateRing::control() {
}

void SpinDriverOperateRing::setRadiusRate(f32 rate) {
    mRadiusRate = MR::normalize(rate, 0.0f, 1.0f);
}

void SpinDriverOperateRing::reset() {
    resetVelocityAndTrans();
}

void SpinDriverOperateRing::update(const TVec3f& a1, const TVec3f& a2) {
    _8C.set(a1);
    _98.set(a2);

    if (mRadiusRate <= 0.0f) {
        resetVelocityAndTrans();
        return;
    }

    updateControlPoint(a2);

    addAccelToOperatePlane(a2);

    TVec3f direction;
    addAccelOperate(&direction, a2);

    addAccelToCenter();
    attenuateVelocity();

    updateDirection(direction);
    updatePosition();
}

void SpinDriverOperateRing::updatePosition() {
    _A4 += mAccelerate;
    _D8 = _A4.length();
    _DC = _D8 / (mRadiusRate * 500.0f);

    TVec3f vec24(0.0f, 0.0f, 0.0f);
    if (!MR::isNearZero(_D8)) {
        vec24 = _A4 * (1.0f / _D8);
    }

    if (_DC > 1.0f) {
        f32 dot = mAccelerate.dot(vec24);
        if (dot > 0.0f) {
            mAccelerate -= vec24 * dot;
        }

        _A4 = _A4.scaleInline(1.0f / _DC);
        _DC = 1.0f;
    }
}

void SpinDriverOperateRing::updateControlPoint(const TVec3f& rVec) {
    if (MR::isStarPointerInScreen(WPAD_CHAN0) && MR::calcStarPointerPosOnPlane(&_C8, _8C, rVec, WPAD_CHAN0, false)) {
        _E0 = 1;

        if (MR::testCorePadButtonB(WPAD_CHAN0)) {
            _E0 = 2;
        }
    } else {
        _E0 = 0;
    }
}

void SpinDriverOperateRing::updateDirection(const TVec3f& rNewDir) {
    mDirection = rNewDir * ::sDirectionRate + mDirection * (1.0f - ::sDirectionRate);
}

void SpinDriverOperateRing::resetVelocityAndTrans() {
    mDirection.zero();
    mAccelerate.zero();
    _A4.zero();
    _D8 = 0.0f;
    _DC = 0.0f;
}

void SpinDriverOperateRing::addAccelToOperatePlane(const TVec3f& rVec) {
    f32 dot = -rVec.dot(_A4);
    mAccelerate += rVec.scaleInline(::sToPlaneRate * dot);
}

void SpinDriverOperateRing::addAccelOperate(TVec3f* pVec, const TVec3f& rVec) {
    TVec3f dpdVec;
    calcOperatePowerByDPD(&dpdVec);
    dpdVec.scale(2.0f);

    if (_DC > 0.5f) {
        TVec3f vec44(-_A4 / _D8);
        f32 dot = vec44.dot(dpdVec);
        f32 norm = MR::normalize(_DC, 0.5f, 1.0f);

        if (dot < 0.0f) {
            dpdVec -= vec44 * dot * norm;
        }
    }

    pVec->set(dpdVec);
    mAccelerate += dpdVec;
}

void SpinDriverOperateRing::addAccelToCenter() {
    f32 norm = MR::normalize(_DC, 0.1f, 1.0f);

    if (norm > 0.0001f) {
        f32 scalar = ((::sOperateAccel * norm) / _D8);
        mAccelerate -= _A4 * scalar;
    }
}

void SpinDriverOperateRing::attenuateVelocity() {
    mAccelerate.x *= 0.94f;
    mAccelerate.y *= 0.94f;
    mAccelerate.z *= 0.94f;
}

void SpinDriverOperateRing::calcOperatePowerByDPD(TVec3f* pVec) const {
    if (_E0 == 2) {
        pVec->set(_C8 - _8C - _A4);

        TVec3f vec(_C8 - _8C - _A4);
        TVec3f dir;
        f32 scalar;
        MR::separateScalarAndDirection(&scalar, &vec, vec);

        if (scalar < 50.0f) {
            f32 scale = scalar / 50.0f;
            pVec->set(vec * scale);
        } else {
            pVec->set(vec);
        }
    } else {
        pVec->zero();
    }
}

void SpinDriverOperateRing::exeWait() {
}
