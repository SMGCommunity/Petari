#include "Game/Map/RailPart.hpp"
#include "Game/Map/BezierRail.hpp"
#include "JSystem/JMath/JMath.hpp"

RailPart::RailPart() : mRailPartLinear(nullptr), mRailPartBezier(nullptr) {}

void RailPart::init(const TVec3f& rPoint1, const TVec3f& rPoint1Ctrl, const TVec3f& rPoint2Ctrl, const TVec3f& rPoint2) {
    if ((rPoint1.epsilonEquals(rPoint1Ctrl, 0.1f)) && (rPoint2.epsilonEquals(rPoint2Ctrl, 0.1f))) {
        mRailPartLinear = new LinearRailPart();
        mRailPartLinear->set(rPoint1, rPoint2);
    } else {
        initForBezier(rPoint1, rPoint1Ctrl, rPoint2Ctrl, rPoint2);
    }
}

void RailPart::initForBezier(const TVec3f& rPoint1, const TVec3f& rPoint1Ctrl, const TVec3f& rPoint2Ctrl, const TVec3f& rPoint2) {
    mRailPartBezier = new BezierRailPart();
    mRailPartBezier->set(rPoint1, rPoint1Ctrl, rPoint2Ctrl, rPoint2);
}

void RailPart::calcPos(TVec3f* pOut, f32 t) const {
    if (mRailPartLinear) {
        JMAVECScaleAdd(&mRailPartLinear->mCtrlDegree1, &mRailPartLinear->mStart, pOut, t);
    } else {
        mRailPartBezier->calcPos(pOut, t);
    }
}

void RailPart::calcVelocity(TVec3f* pOut, f32 t) const {
    if (mRailPartLinear) {
        pOut->set< f32 >(mRailPartLinear->mCtrlDegree1);
    } else {
        mRailPartBezier->calcVelocity(pOut, t);
    }
}

f32 RailPart::getLength(f32 t1, f32 t2, int k) const {
    if (mRailPartLinear) {
        return (mRailPartLinear->mLength * (t2 - t1));
    }

    return mRailPartBezier->getLength(t1, t2, k);
}

f32 RailPart::getTotalLength() const {
    if (mRailPartLinear) {
        return mRailPartLinear->mLength;
    }

    return mRailPartBezier->mLength;
}

f32 RailPart::getParam(f32 t) const {
    if (mRailPartLinear) {
        return (t / mRailPartLinear->mLength);
    }

    return mRailPartBezier->getParam(t);
}

f32 RailPart::getNearestParam(const TVec3f& rPos, f32 t) const {
    if (mRailPartLinear) {
        return mRailPartLinear->getNearestParam(rPos, t);
    }

    return mRailPartBezier->getNearestParam(rPos, t);
}

void LinearRailPart::set(const TVec3f& rPoint1, const TVec3f& rPoint2) {
    mStart = rPoint1;
    mCtrlDegree1 = rPoint2;
    mCtrlDegree1 -= rPoint1;
    mLength = C_VECMag(&mCtrlDegree1);
}

f32 LinearRailPart::getNearestParam(const register TVec3f& rPos, f32 t) const {
    TVec3f v = rPos;
    v -= mStart;

    f32 proj = v.dot(mCtrlDegree1) / mCtrlDegree1.squareMag();

    if (proj < 0.0f) {
        return 0.0f;
    }

    if (proj > 1.0f) {
        return 1.0f;
    }

    return proj;
}
