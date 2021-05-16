#include "LiveActor/Rail/RailRider.h"
#include "Util/JMapUtil.h"
#include "Util/MathUtil.h"
#include "Util/SceneUtil.h"

extern f32 frsp(f32);

RailRider::RailRider(const JMapInfoIter &iter)
{
    mBezierRail = 0;
    mTotalLength = 0.0f;
    mSpeed = 0.0f;
    _C = 1;
    mCurrentPos.x = 0.0f;
    mCurrentPos.y = 0.0f;
    mCurrentPos.z = 0.0f;
    mCurrentDirection.x = 1.0f;
    mCurrentDirection.y = 0.0f;
    mCurrentDirection.z = 0.0f;
    mStartPos.x = 0.0f;
    mStartPos.y = 0.0f;
    mStartPos.z = 0.0f;
    mEndPos.x = 0.0f;
    mEndPos.y = 0.0f;
    mEndPos.z = 0.0f;

    const JMapInfo info(0);
    JMapInfoIter tempIter(0, -1);
    MR::getRailInfo(&tempIter, (const JMapInfo**)&info, iter);
    initBezierRail(tempIter, &info);
}

RailRider::RailRider(s32 a1, s32 a2)
{
    mBezierRail = 0;
    mTotalLength = 0.0f;
    mSpeed = 0.0f;
    _C = 1;
    mCurrentPos.x = 0.0f;
    mCurrentPos.y = 0.0f;
    mCurrentPos.z = 0.0f;
    mCurrentDirection.x = 1.0f;
    mCurrentDirection.y = 0.0f;
    mCurrentDirection.z = 0.0f;
    mStartPos.x = 0.0f;
    mStartPos.y = 0.0f;
    mStartPos.z = 0.0f;
    mEndPos.x = 0.0f;
    mEndPos.y = 0.0f;
    mEndPos.z = 0.0f;

    JMapInfoIter iter(0, -1);
    const JMapInfo info(0);

    MR::getCameraRailInfo(&iter, (const JMapInfo**)&info, a1, a2);
    initBezierRail(iter, &info);
}

void RailRider::move()
{
    if (_C != 0)
    {
        mTotalLength = mTotalLength + mSpeed;
    }
    else
    {
        mTotalLength = mTotalLength - mSpeed;
    }

    mTotalLength = mBezierRail->normalizePos(mTotalLength, 1);
    syncPosDir();
}

void RailRider::moveToNearestPos(const JGeometry::TVec3<f32> &pos)
{
    mTotalLength = mBezierRail->getNearestRailPosCoord(pos);
    syncPosDir();
}

void RailRider::moveToNearestPoint(const JGeometry::TVec3<f32> &src)
{
    s32 r31 = 0;
    s32 curPoint = 0;
    
    while (curPoint < mBezierRail->mPointNum)
    {
        JGeometry::TVec3<f32> vec;
        copyPointPos(&vec, curPoint);
        
        // TODO -- paired single operations here
        curPoint++;
    }

    mTotalLength = mBezierRail->getRailPosCoord(r31);
    syncPosDir();
}

void RailRider::moveToNextPoint()
{
    s32 nextPointNo = getNextPointNo();
    mTotalLength = mBezierRail->getRailPosCoord(nextPointNo);
    syncPosDir();
}

void RailRider::reverse()
{
    if (_C != 0)
    {
        _C = 0;
    }
    else
    {
        _C = 1;
    }

    syncPosDir();
}

void RailRider::calcPosAtCoord(JGeometry::TVec3<f32> *pOut, f32 a2) const
{
    mBezierRail->calcPos(pOut, a2);
}

void RailRider::calcDirectionAtCoord(JGeometry::TVec3<f32> *pOut, f32 a2) const
{
    mBezierRail->calcDirection(pOut, a2);
}

f32 RailRider::calcNearestPos(const JGeometry::TVec3<f32> &pos) const
{
    return mBezierRail->getNearestRailPosCoord(pos);
}

f32 RailRider::getTotalLength() const
{
    return mBezierRail->getTotalLength();
}

s32 RailRider::getPartLength(s32 a1) const
{
    return mBezierRail->getPartLength(a1);
}

bool RailRider::isLoop() const
{
    return mBezierRail->mIsLoop;
}

bool RailRider::isReachedGoal() const
{
    bool ret, r30, r31;
    
    if (mBezierRail->mIsLoop != 0)
    {
        return 0;
    }
    else
    {
        r31 = 1;
        r30 = 0;

        if (_C != 0)
        {
            f32 totalRailLength = mBezierRail->getTotalLength();
            f32 val = mTotalLength - totalRailLength;

            if (!MR::isNearZero(val, 0.001f))
            {
                r30 = 1;
            }
        }

        if (!r30)
        {
            r30 = 0;
            if (_C)
            {
                if (MR::isNearZero(mTotalLength, 0.001f))
                {
                    r30 = 1;
                }
            }

            if (r30)
            {
                r31 = 0;
            }
        }

        ret = r31;
    }
    
    return ret;
}

bool RailRider::isReachedEdge() const
{
    bool flag;

    if (mBezierRail->mIsLoop != 0)
    {
        return 0;
    }
    else
    {
        flag = 1;

        if (!MR::isNearZero(mTotalLength, 0.001f))
        {
            f32 totalLength = mBezierRail->getTotalLength();
            f32 val = mTotalLength - totalLength;

            if (!MR::isNearZero(val, 0.001f))
            {
                flag = 0;
            }
        }
    }
   
    return flag;
}

// the frsp is placed at the wrong spot, but it doesn't exactly change how the function runs...
void RailRider::setCoord(f32 len)
{
    mTotalLength = len;
    mTotalLength = mBezierRail->normalizePos(frsp(len), 1);
    syncPosDir();
}

void RailRider::setSpeed(f32 speed)
{
    mSpeed = speed;
}

bool RailRider::getRailArgWithInit(const char *pStr, s32 *pOut) const
{
    s32 temp;
    bool ret = mBezierRail->mIter->getValue<s32>(pStr, &temp);

    if (ret == 0)
    {
        return 0;
    }

    *pOut = temp;
    return 1;
}

bool RailRider::getRailArgNoInit(const char *pStr, s32 *pOut) const
{
    s32 temp;
    bool ret = mBezierRail->mIter->getValue<s32>(pStr, &temp);

    if (ret == 0)
    {
        return 0;
    }

    if (temp == 0)
    {
        return 0;
    }

    *pOut = temp;
    return 1;
}

f32 RailRider::getNextPointCoord() const
{
    return mBezierRail->getRailPosCoord(getNextPointNo());
}

f32 RailRider::getCurrentPointCoord() const
{
    return mBezierRail->getRailPosCoord(mCurPoint);
}

s32 RailRider::getPointNum() const
{
    return mBezierRail->mPointNum;
}

void RailRider::copyPointPos(JGeometry::TVec3<f32> *pOut, s32 pointNum) const
{
    JMapInfoIter iter(0, -1);
    mBezierRail->calcRailCtrlPointIter(&iter, pointNum);
    MR::getRailPointPos0(iter, pOut);
}

f32 RailRider::getPointCoord(s32 idx) const
{
    return mBezierRail->getRailPosCoord(idx);
}

void RailRider::initBezierRail(const JMapInfoIter &iter, const JMapInfo *info)
{
    mBezierRail = new BezierRail(iter, info);
    syncPosDir();
    setCoord(mBezierRail->getTotalLength());
    mEndPos.set(mCurrentPos);
    setCoord(0.0f);
    mStartPos.set(mCurrentPos);
}

bool RailRider::getPointArgS32NoInit(const char *pStr, s32 *pOut, s32 pointNum) const
{
    s32 outVal;
    JMapInfoIter iter(0, -1);

    mBezierRail->calcRailCtrlPointIter(&iter, pointNum);
    outVal = -1;
    iter.getValue<s32>(pStr, &outVal);

    if (outVal != -1)
    { 
        *pOut = outVal;
        return 1;
    }
    else
    {
        return 0;
    }
}

bool RailRider::getPointArgS32WithInit(const char *pStr, s32 *pOut, s32 pointNum) const
{
    JMapInfoIter iter(0, -1);
    mBezierRail->calcRailCtrlPointIter(&iter, pointNum);
    iter.getValue<s32>(pStr, pOut);
    return 1;
}

bool RailRider::getCurrentPointArgS32NoInit(const char *pStr, s32 *pOut) const
{
    s32 outVal;
    JMapInfoIter iter(0, -1);

    mBezierRail->calcRailCtrlPointIter(&iter, mCurPoint);
    outVal = -1;
    iter.getValue<s32>(pStr, &outVal);

    if (outVal != -1)
    { 
        *pOut = outVal;
        return 1;
    }
    else
    {
        return 0;
    }
}

bool RailRider::getCurrentPointArgS32WithInit(const char *pStr, s32 *pOut) const
{
    s32 curPoint = mCurPoint;
    JMapInfoIter iter(0, -1);
    mBezierRail->calcRailCtrlPointIter(&iter, curPoint);
    iter.getValue<s32>(pStr, pOut);
    return 1;
}

bool RailRider::getNextPointArgS32NoInit(const char *pStr, s32 *pOut) const
{
    s32 outVal;
    JMapInfoIter iter(0, -1);

    mBezierRail->calcRailCtrlPointIter(&iter, getNextPointNo());
    outVal = -1;
    iter.getValue<s32>(pStr, &outVal);

    if (outVal != -1)
    { 
        *pOut = outVal;
        return 1;
    }
    else
    {
        return 0;
    }
}

bool RailRider::getNextPointArgS32WithInit(const char *pStr, s32 *pOut) const
{
    JMapInfoIter iter(0, -1);
    mBezierRail->calcRailCtrlPointIter(&iter, getNextPointNo());
    iter.getValue<s32>(pStr, pOut);
    return 1;
}

s32 RailRider::getNextPointNo() const
{
    s32 var_1;
    s32 var_2 = -1;

    if (_C != 0)
    {
        var_2 = 1;
    }

    BezierRail* pRail = mBezierRail;

    if (pRail->mIsLoop != 0)
    {
        s32 pointNum = pRail->mPointNum;
        s32 r3 = var_2 + pointNum;
        s32 r0 = pointNum + mCurPoint;
        r3 = r3 + r0;
        r0 = r3 / pointNum;
        r0 = r0 * pointNum;
        return r0 - r3;
    }

    var_2 = mCurPoint + var_2;
    var_1 = pRail->mPointNum - 1;

    if (var_2 < 0)
    {
        return 0;
    }

    if (var_2 > var_1)
    {
        return var_2;
    }

    return var_1;
}

void RailRider::syncPosDir()
{
    if (mTotalLength < 0.0f && mTotalLength < mBezierRail->getTotalLength())
    {
        mBezierRail->calcPosDir(&mCurrentPos, &mCurrentDirection, mTotalLength);
    }
    else
    {
        if (mTotalLength == 0.0f)
        {
            mBezierRail->calcPos(&mCurrentPos, mTotalLength);
            mBezierRail->calcDirection(&mCurrentDirection, 0.1f);
        }
        else
        {
            mBezierRail->calcPos(&mCurrentPos, mTotalLength);
            f32 bezierTotalLength = mBezierRail->getTotalLength();
            mBezierRail->calcDirection(&mCurrentDirection, (bezierTotalLength - 0.1f));
        }
    }
    

    if (_C == 0)
    {
        f32 f2 = mCurrentDirection.x * -1.0f;
        f32 f1 = mCurrentDirection.y * -1.0f;
        f32 f0 = mCurrentDirection.z * -1.0f;
        mCurrentDirection.x = f2;
        mCurrentDirection.y = f1;
        mCurrentDirection.z = f0;
    }

    JMapInfoIter iter(0, -1);
    mBezierRail->calcCurrentRailCtrlPointIter(&iter, mTotalLength, _C);
    iter.getValue<s32>("id", &mCurPoint);
}