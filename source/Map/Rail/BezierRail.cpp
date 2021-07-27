#include "Map/Rail/BezierRail.h"
#include "JSystem/JGeometry/TUtil.h"
#include "Util/JMapUtil.h"
#include "Util/MathUtil.h"
#include "Util/StringUtil.h"

BezierRail::BezierRail(const JMapInfoIter &iter, const JMapInfo *pInfo)
{
    f32 f0, f29;

    mMapInfo = pInfo;
    mIter = new JMapInfoIter(iter.mInfo, iter.mPos);

    const char *pUnk = unk_str;
    const JMapInfo *pTemp_info = iter.mInfo;
    s32 position = iter.mPos;

    s32 res = pTemp_info->searchItemInfo("closed");

    if (res >= 0)
    {
        pTemp_info->getValueFast(position, res, &pUnk);
    }

    mIsLoop = MR::isEqualString(pUnk, "CLOSE");
    s32 inf = 0;

    /*
    if (pInfo->mData != 0)
    {
        inf = pInfo->mData;
    }
    else
    {
        inf = 0;
    }

*/
    s32 val = inf - 1;
    mPointNum = inf;    
    mPointNumLoop = val;

    if (mIsLoop != 0)
    {
        mPointNumLoop = ++val;
    }

    f32 f31 = 0.0f;
    f32 f30 = f31;

    mRailParts = new RailPart[mPointNumLoop];
    mSegLengths = new f32[mPointNumLoop];

    s32 curPoint = 0;

    while(curPoint < mPointNumLoop)
    {
        JMapInfoIter tempIter_0 = iter;
        s32 r6 = curPoint + 1;
        tempIter_0.mPos = curPoint;
        s32 r0 = (r6 / mPointNum);
        JMapInfoIter tempIter_1 = iter;
        r0 = r0 * mPointNum;
        r0 = r0 - r6;
        tempIter_1.mPos = r0;

        JGeometry::TVec3f vec_0;
        JGeometry::TVec3f vec_1;
        JGeometry::TVec3f vec_2;
        JGeometry::TVec3f vec_3;

        MR::getRailPointPos0(tempIter_0, &vec_0);
        MR::getRailPointPos2(tempIter_0, &vec_1);
        MR::getRailPointPos1(tempIter_1, &vec_2);
        MR::getRailPointPos0(tempIter_1, &vec_3);

        mRailParts[curPoint].init(vec_0, vec_1, vec_2, vec_3);
        f29 = f31;
        f32 partTotalLength = mRailParts[curPoint].getTotalLength();
        f30 = f30 + partTotalLength;
        f31 = f31 + f30;
        f0 = f31 - f29;
        mSegLengths[curPoint] = f31;
        f30 = f30 - f0;
        curPoint++;
    }
}

f32 BezierRail::normalizePos(f32 a1 ,s32 a2) const
{
    f32 val;

    if (mIsLoop)
    {
        val = MR::mod(a1, getTotalLength());

        if (a2 < 0)
        {
            if (MR::isNearZero(val, 0.001f))
            {
                val = getTotalLength();
            }
        }

        if (val < 0.0f)
        {
            val += getTotalLength();
        }

        return val;
    }
    else
    {
        val = getTotalLength();
        return JGeometry::TUtil<f32>::clamp(a1, 0.0f, val);
    }
}

f32 BezierRail::getTotalLength() const
{
    return mSegLengths[mPointNumLoop - 1];
}

f32 BezierRail::getPartLength(s32 idx) const
{
    return mRailParts[idx].getTotalLength();
}

void BezierRail::calcPos(JGeometry::TVec3f *pOut, f32 a2) const
{
    const RailPart *pPart;
    f32 param_2;
    getIncludedSection(&pPart, &param_2, a2, 1);
    f32 partParam = pPart->getParam(param_2);
    pPart->calcPos(pOut, partParam);
}

void BezierRail::calcDirection(JGeometry::TVec3f *pOut, f32 a2) const
{
    const RailPart *pPart;
    f32 param_2;
    getIncludedSection(&pPart, &param_2, a2, 1);
    f32 partParam = pPart->getParam(param_2);
    BezierRail::calcRailDirection(pOut, pPart, partParam);   
}

void BezierRail::calcPosDir(JGeometry::TVec3f *pPos, JGeometry::TVec3f *pDir, f32 a3) const
{
    const RailPart *pPart;
    f32 param_2;
    getIncludedSection(&pPart, &param_2, a3, 1);
    f32 partParam = pPart->getParam(param_2);
    pPart->calcPos(pPos, partParam); 
    BezierRail::calcRailDirection(pDir, pPart, partParam); 
}

// doesn't match very well, yet. fix me
f32 BezierRail::getNearestRailPosCoord(const JGeometry::TVec3f &coord) const
{
    RailPart *pRootPart = mRailParts;
    s32 curLargest = 0;
    f32 railPartTotalLen = pRootPart->getTotalLength();
    f32 railPartNearParam = pRootPart->getNearestParam(coord, (100.0f / railPartTotalLen));

    JGeometry::TVec3f railPartPos;
    pRootPart->calcPos(&railPartPos, railPartNearParam);
    railPartPos.sub(coord);
    f32 rootRailPartSqr = railPartPos.squared();
    f32 baseSize = 100.0f;

    s32 anotherIdx = 1;
    f32 f30;

    while (anotherIdx < mPointNumLoop)
    {
        RailPart *pCurPart = &mRailParts[anotherIdx];
        f32 curRailTotalLen = pCurPart->getTotalLength();
        f32 curParam = pCurPart->getNearestParam(coord, (baseSize / curRailTotalLen));

        JGeometry::TVec3f curRailPos;
        pCurPart->calcPos(&curRailPos, curParam);
        curRailPos.sub(coord);
        f32 railPartSqr = curRailPos.squared();

        if (railPartSqr < rootRailPartSqr)
        {
            rootRailPartSqr = railPartSqr;
            curLargest = anotherIdx;
            f30 = curParam;
        }
    }

    f32 len;

    if (curLargest != 0)
    {
        len = mSegLengths[curLargest - 1];
    }
    else
    {
        len = 0.0f;
    }

    f32 railPartLen = mRailParts[curLargest].getLength(0.0f, f30, 0xA);
    return len + railPartLen;
}

f32 BezierRail::getRailPosCoord(s32 idx) const
{
    if (idx == 0)
    {
        return 0.0f;
    }

    if (!(mIsLoop == 0) && !(idx == mPointNum - 1))
    {
        idx--;
        return mSegLengths[idx];
    }
        
    return getTotalLength();
}

void BezierRail::calcCurrentRailCtrlPointIter(JMapInfoIter *pIter, f32 a2, bool a3) const
{
    s32 idx = getCurrentCtrlPointIndex(a2, a3);
    calcRailCtrlPointIter(pIter, idx);
}

// this uses one more ldw than it needs to...why?
void BezierRail::calcRailCtrlPointIter(JMapInfoIter *pIter, s32 idx) const
{
    const JMapInfo *pInfo = mIter->mInfo;
    pIter->mPos = idx;
    pIter->mInfo = pInfo;
}