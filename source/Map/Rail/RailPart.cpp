#include "JSystem/JMath/JMath.h"
#include "Map/Rail/RailPart.h"
#include <revolution/mtx.h>

RailPart::RailPart()
{
    mLinearRailPart = 0;
    mBezierRailPart = 0;
}

void RailPart::init(const JGeometry::TVec3f &a1, const JGeometry::TVec3f &a2, const JGeometry::TVec3f &a3, const JGeometry::TVec3f &a4)
{
    if (a1.epsilonEquals(a2, 0.1f) != 0)
    {
        if (a4.epsilonEquals(a3, 0.1f) != 0)
        {
            mLinearRailPart = new LinearRailPart();
            mLinearRailPart->set(a1, a2);
            return;
        }
    }

    initForBezier(a1, a2, a3, a4);
}

void RailPart::initForBezier(const JGeometry::TVec3f &a1, const JGeometry::TVec3f &a2, const JGeometry::TVec3f &a3, const JGeometry::TVec3f &a4)
{
    // todo -- omit constructor here
    mBezierRailPart = new BezierRailPart();
    mBezierRailPart->set(a1, a2, a3, a4);
}

void RailPart::calcPos(JGeometry::TVec3f *pOut, f32 a2) const
{
    // uses the stack, fix this
    if (mLinearRailPart != 0)
    {
        //JMAVECScaleAdd(&mLinearRailPart->_C, &mLinearRailPart->_0, pOut, a2);
    }

    mBezierRailPart->calcPos(pOut, a2);
}

void RailPart::calcVelocity(JGeometry::TVec3f *pOut, f32 a2) const
{
    if (mLinearRailPart != 0)
    {
        pOut->set(mLinearRailPart->_C);
        return;
    }

    mBezierRailPart->calcVelocity(pOut, a2);
}

f32 RailPart::getLength(f32 a1, f32 a2, s32 a3) const
{
    if (mLinearRailPart != 0)
    {
        return mLinearRailPart->mMagnitude * (a2 - a1);
    }

    return mBezierRailPart->getLength(a1, a2, a3);
}

f32 RailPart::getTotalLength() const
{
    if (mLinearRailPart != 0)
    {
        return mLinearRailPart->mMagnitude;
    }

    return mBezierRailPart->mLength;
}

f32 RailPart::getParam(f32 a1) const
{
    if (mLinearRailPart != 0)
    {
        return (a1 / mLinearRailPart->mMagnitude);
    }

    return mBezierRailPart->getParam(a1);
}

f32 RailPart::getNearestParam(const JGeometry::TVec3f &pos, f32 a2) const
{
    if (mLinearRailPart != 0)
    {
        return mLinearRailPart->getNearestParam(pos, a2);
    }

    return mBezierRailPart->getNearestParam(pos, a2);
}

void LinearRailPart::set(const JGeometry::TVec3f &a1, const JGeometry::TVec3f &a2)
{
    _0 = a1;
    _C = a2;
    _C -= a2;
    //mMagnitude = PSVECMag(&_C);
}