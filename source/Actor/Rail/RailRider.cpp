#include "Actor/Rail/RailRider.h"
#include "MR/JMap/JMapUtil.h"
#include "MR/MathUtil.h"
#include "MR/SceneUtil.h"
#include "defines.h"

RailRider::RailRider(const JMapInfoIter &iter)
{
    this->mBezierRail = 0;
    this->mTotalLength = 0.0f;
    this->mSpeed = 0.0f;
    this->_C = 1;
    this->mCurrentPos.x = 0.0f;
    this->mCurrentPos.y = 0.0f;
    this->mCurrentPos.z = 0.0f;
    this->mCurrentDirection.x = 1.0f;
    this->mCurrentDirection.y = 0.0f;
    this->mCurrentDirection.z = 0.0f;
    this->mStartPos.x = 0.0f;
    this->mStartPos.y = 0.0f;
    this->mStartPos.z = 0.0f;
    this->mEndPos.x = 0.0f;
    this->mEndPos.y = 0.0f;
    this->mEndPos.z = 0.0f;

    const JMapInfo info(0);
    JMapInfoIter tempIter(0, -1);
    MR::getRailInfo(&tempIter, (const JMapInfo**)&info, iter);
    this->initBezierRail(tempIter, &info);
}

RailRider::RailRider(s32 a1, s32 a2)
{
    this->mBezierRail = 0;
    this->mTotalLength = 0.0f;
    this->mSpeed = 0.0f;
    this->_C = 1;
    this->mCurrentPos.x = 0.0f;
    this->mCurrentPos.y = 0.0f;
    this->mCurrentPos.z = 0.0f;
    this->mCurrentDirection.x = 1.0f;
    this->mCurrentDirection.y = 0.0f;
    this->mCurrentDirection.z = 0.0f;
    this->mStartPos.x = 0.0f;
    this->mStartPos.y = 0.0f;
    this->mStartPos.z = 0.0f;
    this->mEndPos.x = 0.0f;
    this->mEndPos.y = 0.0f;
    this->mEndPos.z = 0.0f;

    JMapInfoIter iter(0, -1);
    const JMapInfo info(0);

    MR::getCameraRailInfo(&iter, (const JMapInfo**)&info, a1, a2);
    this->initBezierRail(iter, &info);
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

void RailRider::moveToNearestPoint(const JGeometry::TVec3<f32> &src)
{
    s32 r31 = 0;
    s32 curPoint = 0;
    
    while (curPoint < this->mBezierRail->mPointNum)
    {
        JGeometry::TVec3<f32> vec;
        this->copyPointPos(&vec, curPoint);
        
        // TODO -- paired single operations here
        curPoint++;
    }

    this->mTotalLength = this->mBezierRail->getRailPosCoord(r31);
    this->syncPosDir();
}

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

bool RailRider::getRailArgWithInit(const char *str, s32 *out) const
{
    s32 temp;
    bool ret = this->mBezierRail->mIter->getValue<s32>(str, &temp);

    if (ret == 0)
        return 0;

    *out = temp;
    return 1;
}

bool RailRider::getRailArgNoInit(const char *str, s32 *out) const
{
    s32 temp;
    bool ret = this->mBezierRail->mIter->getValue<s32>(str, &temp);

    if (ret == 0)
        return 0;

    if (temp == 0)
        return 0;

    *out = temp;
    return 1;
}

f32 RailRider::getNextPointCoord() const
{
    return this->mBezierRail->getRailPosCoord(this->getNextPointNo());
}

f32 RailRider::getCurrentPointCoord() const
{
    return this->mBezierRail->getRailPosCoord(this->mCurPoint);
}

s32 RailRider::getPointNum() const
{
    return this->mBezierRail->mPointNum;
}

void RailRider::copyPointPos(JGeometry::TVec3<f32> *out, s32 pointNum) const
{
    JMapInfoIter iter(0, -1);
    this->mBezierRail->calcRailCtrlPointIter(&iter, pointNum);
    MR::getRailPointPos0(iter, out);
}

f32 RailRider::getPointCoord(s32 idx) const
{
    return this->mBezierRail->getRailPosCoord(idx);
}

void RailRider::initBezierRail(const JMapInfoIter &iter, const JMapInfo *info)
{
    this->mBezierRail = new BezierRail(iter, info);
    this->syncPosDir();
    this->setCoord(this->mBezierRail->getTotalLength());
    this->mEndPos.set(this->mCurrentPos);
    this->setCoord(0.0f);
    this->mStartPos.set(this->mCurrentPos);
}

bool RailRider::getPointArgS32NoInit(const char *str, s32 *out, s32 pointNum) const
{
    s32 outVal;
    JMapInfoIter iter(0, -1);

    this->mBezierRail->calcRailCtrlPointIter(&iter, pointNum);
    outVal = -1;
    iter.getValue<s32>(str, &outVal);

    if (outVal != -1)
    { 
        *out = outVal;
        return 1;
    }
    else
        return 0;
}

bool RailRider::getPointArgS32WithInit(const char *str, s32 *out, s32 pointNum) const
{
    JMapInfoIter iter(0, -1);
    this->mBezierRail->calcRailCtrlPointIter(&iter, pointNum);
    iter.getValue<s32>(str, out);
    return 1;
}

bool RailRider::getCurrentPointArgS32NoInit(const char *str, s32 *out) const
{
    s32 outVal;
    JMapInfoIter iter(0, -1);

    this->mBezierRail->calcRailCtrlPointIter(&iter, this->mCurPoint);
    outVal = -1;
    iter.getValue<s32>(str, &outVal);

    if (outVal != -1)
    { 
        *out = outVal;
        return 1;
    }
    else
        return 0;
}

bool RailRider::getCurrentPointArgS32WithInit(const char *str, s32 *out) const
{
    s32 curPoint = this->mCurPoint;
    JMapInfoIter iter(0, -1);
    this->mBezierRail->calcRailCtrlPointIter(&iter, curPoint);
    iter.getValue<s32>(str, out);
    return 1;
}

bool RailRider::getNextPointArgS32NoInit(const char *str, s32 *out) const
{
    s32 outVal;
    JMapInfoIter iter(0, -1);

    this->mBezierRail->calcRailCtrlPointIter(&iter, this->getNextPointNo());
    outVal = -1;
    iter.getValue<s32>(str, &outVal);

    if (outVal != -1)
    { 
        *out = outVal;
        return 1;
    }
    else
        return 0;
}

bool RailRider::getNextPointArgS32WithInit(const char *str, s32 *out) const
{
    JMapInfoIter iter(0, -1);
    this->mBezierRail->calcRailCtrlPointIter(&iter, this->getNextPointNo());
    iter.getValue<s32>(str, out);
    return 1;
}

s32 RailRider::getNextPointNo() const
{
    s32 var_1;
    s32 var_2 = -1;

    if (this->_C != 0)
        var_2 = 1;

    BezierRail* rail = this->mBezierRail;

    if (rail->mIsLoop != 0)
    {
        s32 pointNum = rail->mPointNum;
        s32 r3 = var_2 + pointNum;
        s32 r0 = pointNum + this->mCurPoint;
        r3 = r3 + r0;
        r0 = r3 / pointNum;
        r0 = r0 * pointNum;
        return r0 - r3;
    }

    var_2 = this->mCurPoint + var_2;
    var_1 = rail->mPointNum - 1;

    if (var_2 < 0)
        return 0;

    if (var_2 > var_1)
        return var_2;

    return var_1;
}

void RailRider::syncPosDir()
{
    if (this->mTotalLength < 0.0f && this->mTotalLength < this->mBezierRail->getTotalLength())
    {
        this->mBezierRail->calcPosDir(&this->mCurrentPos, &this->mCurrentDirection, this->mTotalLength);
    }
    else
    {
        if (this->mTotalLength == 0.0f)
        {
            this->mBezierRail->calcPos(&this->mCurrentPos, this->mTotalLength);
            this->mBezierRail->calcDirection(&this->mCurrentDirection, 0.1f);
        }
        else
        {
            this->mBezierRail->calcPos(&this->mCurrentPos, this->mTotalLength);
            f32 bezierTotalLength = this->mBezierRail->getTotalLength();
            this->mBezierRail->calcDirection(&this->mCurrentDirection, (bezierTotalLength - 0.1f));
        }
    }
    

    if (this->_C == 0)
    {
        f32 f2 = this->mCurrentDirection.x * -1.0f;
        f32 f1 = this->mCurrentDirection.y * -1.0f;
        f32 f0 = this->mCurrentDirection.z * -1.0f;
        this->mCurrentDirection.x = f2;
        this->mCurrentDirection.y = f1;
        this->mCurrentDirection.z = f0;
    }

    JMapInfoIter iter(0, -1);
    this->mBezierRail->calcCurrentRailCtrlPointIter(&iter, this->mTotalLength, this->_C);
    iter.getValue<s32>("id", &this->mCurPoint);
}