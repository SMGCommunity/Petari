#include "Game/Map/RailPart.hpp"

#include "JSystem/JMath/JMath.hpp"

RailPart::RailPart()
    : mRailPartLinear(0), mRailPartBezier(0) {
}

void RailPart::init(const TVec3f &a1, const TVec3f &a2, const TVec3f &a3, const TVec3f &a4) {
    if ((!a1.epsilonEquals(a2, 0.1f)) || (a4.epsilonEquals(a3, 0.1f))) {
        mRailPartLinear = new LinearRailPart();
        mRailPartLinear->set(a1, a4);       
    }
    else {
        initForBezier(a1, a2, a3, a4);
    }
}

void RailPart::initForBezier(const TVec3f &a1, const TVec3f &a2, const TVec3f &a3, const TVec3f &a4) {
    mRailPartBezier = new BezierRailPart();
    mRailPartBezier->set(a1, a2, a3, a4);
}

void RailPart::calcPos(TVec3f *pOut, f32 a2) const {
    if (mRailPartLinear) {
        JMAVECScaleAdd(&mRailPartLinear->_C, &mRailPartLinear->_0, pOut, a2);
    }
    else {
        mRailPartBezier->calcPos(pOut, a2);
    }
}

void RailPart::calcVelocity(TVec3f *pOut, f32 a2) const {
    if (mRailPartLinear) {
        pOut->set<f32>(mRailPartLinear->_C);
    }
    else {
        mRailPartBezier->calcVelocity(pOut, a2);
    }
}

f32 RailPart::getLength(f32 a1, f32 a2, int a3) const {
    if (mRailPartLinear) {
        return (mRailPartLinear->_18 * (a2 - a1));
    }

    return mRailPartBezier->getLength(a1, a2, a3);
}

f32 RailPart::getTotalLength() const {
    if (mRailPartLinear) {
        return mRailPartLinear->_18;
    }

    return mRailPartBezier->mLength;
}

f32 RailPart::getParam(f32 a1) const {
    if (mRailPartLinear) {
        return (a1 / mRailPartLinear->_18);
    }

    return mRailPartBezier->getParam(a1);
}

f32 RailPart::getNearestParam(const TVec3f &rPos, f32 a2) const {
    if (mRailPartLinear) {
        return mRailPartLinear->getNearestParam(rPos, a2);
    }

    return mRailPartBezier->getNearestParam(rPos, a2);
}

void LinearRailPart::set(const TVec3f &a1, const TVec3f &a2) {
    _0 = a1;
    _C = a2;
    _C -= a1;
    _18 = C_VECMag((const Vec *)&_C);
}

/*
f32 LinearRailPart::getNearestParam(const register TVec3f &a1, f32 a2) const {

}
*/
