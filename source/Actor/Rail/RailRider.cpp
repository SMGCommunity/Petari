#include "Actor/Rail/RailRider.h"
#include "MR/MathUtil.h"
#include "MR/SceneUtil.h"
#include "defines.h"

RailRider::RailRider(s32 a1, s32 a2)
{
    JMapInfoIter iter;
    JMapInfo* info = 0;

    this->mBezierRail = 0;
    this->mTotalLength = 0.0f;
    this->mSpeed = 0.0f;
    this->_C = 1;
    this->_10.x = 0.0f;
    this->_10.y = 0.0f;
    this->_10.z = 0.0f;
    this->_1C.x = 1.0f;
    this->_1C.y = 0.0f;
    this->_1C.z = 0.0f;
    this->_28.x = 0.0f;
    this->_28.y = 0.0f;
    this->_28.z = 0.0f;
    this->_34.x = 0.0f;
    this->_34.y = 0.0f;
    this->_34.z = 0.0f;
    info->_0 = 0;
    iter._0 = 0;
    iter._4 = -1;

    // this is weird, it wants it to be constant but it modifies it before this call
    MR::getCameraRailInfo(&iter, (const JMapInfo **)info, a1, a2);
    this->initBezierRail(iter, (const JMapInfo*)info);
}

void RailRider::move()
{
    if (this->_C != 0)
        this->mTotalLength = this->mTotalLength + this->mSpeed;
    else
        this->mTotalLength = this->mTotalLength - this->mSpeed;

    this->mTotalLength = this->mBezierRail->normalizePos(this->mTotalLength, 1);
    this->syncPosDir();
}

void RailRider::moveToNearestPos(const JGeometry::TVec3<f32> &pos)
{
    this->mTotalLength = this->mBezierRail->getNearestRailPosCoord(pos);
    this->syncPosDir();
}

// RailRider::moveToNearestPoint()

void RailRider::moveToNextPoint()
{
    s32 nextPointNo = this->getNextPointNo();
    this->mTotalLength = this->mBezierRail->getRailPosCoord(nextPointNo);
    this->syncPosDir();
}

void RailRider::reverse()
{
    if (this->_C != 0)
        this->_C = 0;
    else
        this->_C = 1;

    this->syncPosDir();
}

void RailRider::calcPosAtCoord(JGeometry::TVec3<f32> *out, f32 a2) const
{
    this->mBezierRail->calcPos(out, a2);
}

void RailRider::calcDirectionAtCoord(JGeometry::TVec3<f32> *out, f32 a2) const
{
    this->mBezierRail->calcDirection(out, a2);
}

void RailRider::calcNearestPos(const JGeometry::TVec3<f32> &a1) const
{
    this->mBezierRail->getNearestRailPosCoord(a1);
}

s32 RailRider::getTotalLength() const
{
    return this->mBezierRail->getTotalLength();
}

s32 RailRider::getPartLength(s32 a1) const
{
    return this->mBezierRail->getPartLength(a1);
}

bool RailRider::isLoop() const
{
    return this->mBezierRail->mIsLoop;
}

bool RailRider::isReachedGoal() const
{
    bool ret, r30, r31;
    
    if (this->mBezierRail->mIsLoop != 0)
        return 0;
    else
    {
        r31 = 1;
        r30 = 0;

        if (this->_C != 0)
        {
            f32 totalRailLength = this->mBezierRail->getTotalLength();
            f32 val = this->mTotalLength - totalRailLength;

            if (!MR::isNearZero(val, 0.001f))
                r30 = 1;
        }

        if (!r30)
        {
            r30 = 0;
            if (this->_C)
            {
                if (MR::isNearZero(this->mTotalLength, 0.001f))
                    r30 = 1;             
            }

            if (r30)
                r31 = 0;
        }

        ret = r31;
    }
    
    return ret;
}

bool RailRider::isReachedEdge() const
{
    bool flag;

    if (this->mBezierRail->mIsLoop != 0)
        return 0;
    else
    {
        flag = 1;

        if (!MR::isNearZero(this->mTotalLength, 0.001f))
        {
            f32 totalLength = this->mBezierRail->getTotalLength();
            f32 val = this->mTotalLength - totalLength;

            if (!MR::isNearZero(val, 0.001f))
                flag = 0;
        }
    }
   
    return flag;
}

// the frsp is placed at the wrong spot, but it doesn't exactly change how the function runs...
void RailRider::setCoord(f32 len)
{
    this->mTotalLength = len;
    this->mTotalLength = this->mBezierRail->normalizePos(frsp(len), 1);
    this->syncPosDir();
}

void RailRider::setSpeed(f32 speed)
{
    this->mSpeed = speed;
}

// getRailArgWithInit / getRailArgNoInit

f32 RailRider::getNextPointCoord() const
{
    return this->mBezierRail->getRailPosCoord(this->getNextPointNo());
}

f32 RailRider::getCurrentPointCoord() const
{
    return this->mBezierRail->getRailPosCoord(this->_40);
}

s32 RailRider::getPointNum() const
{
    return this->mBezierRail->mPointNum;
}