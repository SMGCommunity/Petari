#include "JMath/JMath.h"
#include "Map/Rail/RailPart.h"
#include "OS/vec.h"

void RailPart::initForBezier(const JGeometry::TVec3<f32> &a1, const JGeometry::TVec3<f32> &a2, const JGeometry::TVec3<f32> &a3, const JGeometry::TVec3<f32> &a4)
{
    // todo -- omit constructor here
    this->mBezierRailPart = new BezierRailPart();
    this->mBezierRailPart->set(a1, a2, a3, a4);
}

void RailPart::calcPos(JGeometry::TVec3<f32> *out, f32 a2) const
{
    // uses the stack, fix this
    if (this->mLinearRailPart != 0)
    {
        JMAVECScaleAdd(&this->mLinearRailPart->_C, &this->mLinearRailPart->_0, out, a2);
    }

    this->mBezierRailPart->calcPos(out, a2);
}

void RailPart::calcVelocity(JGeometry::TVec3<f32> *out, f32 a2) const
{
    if (this->mLinearRailPart != 0)
    {
        out->set(this->mLinearRailPart->_C);
        return;
    }

    this->mBezierRailPart->calcVelocity(out, a2);
}

f32 RailPart::getLength(f32 a1, f32 a2, s32 a3) const
{
    if (this->mLinearRailPart != 0)
    {
        return this->mLinearRailPart->mMagnitude * (a2 - a1);
    }

    return this->mBezierRailPart->getLength(a1, a2, a3);
}

f32 RailPart::getTotalLength() const
{
    if (this->mLinearRailPart != 0)
    {
        return this->mLinearRailPart->mMagnitude;
    }

    return this->mBezierRailPart->mLength;
}

f32 RailPart::getParam(f32 a1) const
{
    if (this->mLinearRailPart != 0)
    {
        return (a1 / this->mLinearRailPart->mMagnitude);
    }

    return this->mBezierRailPart->getParam(a1);
}

f32 RailPart::getNearestParam(const JGeometry::TVec3<f32> &pos) const
{
    if (this->mLinearRailPart != 0)
    {
        return this->mLinearRailPart->getNearestParam(pos);
    }

    return this->mBezierRailPart->getNearestParam(pos);
}

void LinearRailPart::set(const JGeometry::TVec3<f32> &a1, const JGeometry::TVec3<f32> &a2)
{
    this->_0 = a1;
    this->_C = a2;
    this->_C -= a2;
    this->mMagnitude = C_VECMag(&this->_C);
}