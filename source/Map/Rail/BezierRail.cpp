#include "Map/Rail/BezierRail.h"
#include "JGeometry/TUtil.h"
#include "MR/JMap/JMapUtil.h"
#include "MR/MathUtil.h"
#include "MR/StringUtil.h"

BezierRail::BezierRail(const JMapInfoIter &iter, const JMapInfo *info)
{
    f32 f0, f29;

    this->mMapInfo = info;
    this->mIter = new JMapInfoIter(iter.mInfo, iter.mPos);

    const char* unk = unk_str;
    const JMapInfo* temp_info = iter.mInfo;
    s32 position = iter.mPos;

    s32 res = temp_info->searchItemInfo("closed");

    if (res >= 0)
    {
        temp_info->getValueFast(position, res, &unk);
    }

    this->mIsLoop = MR::isEqualString(unk, "CLOSE");
    s32 inf;

    // this check screws up some stuff, adds some useless math
    if (info->_0 != 0)
        inf = info->_0;
    else
        inf = 0;

    s32 val = inf - 1;
    this->mPointNum = inf;    
    this->mPointNumLoop = val;

    if (this->mIsLoop != 0)
        this->mPointNumLoop = ++val;

    f32 f31 = 0.0f;
    f32 f30 = f31;

    this->mRailParts = new RailPart[this->mPointNumLoop];
    this->mSegLengths = new f32[this->mPointNumLoop];

    s32 curPoint = 0;

    while(curPoint < this->mPointNumLoop)
    {
        JMapInfoIter tempIter_0 = iter;
        s32 r6 = curPoint + 1;
        tempIter_0.mPos = curPoint;
        s32 r0 = (r6 / this->mPointNum);
        JMapInfoIter tempIter_1 = iter;
        r0 = r0 * this->mPointNum;
        r0 = r0 - r6;
        tempIter_1.mPos = r0;

        JGeometry::TVec3<f32> vec_0;
        JGeometry::TVec3<f32> vec_1;
        JGeometry::TVec3<f32> vec_2;
        JGeometry::TVec3<f32> vec_3;

        MR::getRailPointPos0(tempIter_0, &vec_0);
        MR::getRailPointPos2(tempIter_0, &vec_1);
        MR::getRailPointPos1(tempIter_1, &vec_2);
        MR::getRailPointPos0(tempIter_1, &vec_3);

        this->mRailParts[curPoint].init(vec_0, vec_1, vec_2, vec_3);
        f29 = f31;
        f32 partTotalLength = this->mRailParts[curPoint].getTotalLength();
        f30 = f30 + partTotalLength;
        f31 = f31 + f30;
        f0 = f31 - f29;
        this->mSegLengths[curPoint] = f31;
        f30 = f30 - f0;
        curPoint++;
    }
}

f32 BezierRail::normalizePos(f32 a1 ,s32 a2) const
{
    f32 val;

    if (this->mIsLoop)
    {
        val = MR::mod(a1, this->getTotalLength());

        if (a2 < 0)
        {
            if (MR::isNearZero(val, 0.001f))
                val = this->getTotalLength();
        }

        if (val < 0.0f)
            val += this->getTotalLength();

        return val;
    }
    else
    {
        val = this->getTotalLength();
        return JGeometry::TUtil<f32>::clamp(a1, 0.0f, val);
    }
}

f32 BezierRail::getTotalLength() const
{
    return this->mSegLengths[this->mPointNumLoop - 1];
}

f32 BezierRail::getPartLength(s32 idx) const
{
    return this->mRailParts[idx].getTotalLength();
}

void BezierRail::calcPos(JGeometry::TVec3<f32> *out, f32 a2) const
{
    const RailPart* part;
    f32 param_2;
    this->getIncludedSection(&part, &param_2, a2, 1);
    f32 partParam = part->getParam(param_2);
    part->calcPos(out, partParam);
}

void BezierRail::calcDirection(JGeometry::TVec3<f32> *out, f32 a2) const
{
    const RailPart* part;
    f32 param_2;
    this->getIncludedSection(&part, &param_2, a2, 1);
    f32 partParam = part->getParam(param_2);
    BezierRail::calcRailDirection(out, part, partParam);   
}

void BezierRail::calcPosDir(JGeometry::TVec3<f32> *pos, JGeometry::TVec3<f32> *dir, f32 a3) const
{
    const RailPart* part;
    f32 param_2;
    this->getIncludedSection(&part, &param_2, a3, 1);
    f32 partParam = part->getParam(param_2);
    part->calcPos(pos, partParam); 
    BezierRail::calcRailDirection(dir, part, partParam); 
}

// doesn't match very well, yet. fix me
f32 BezierRail::getNearestRailPosCoord(const JGeometry::TVec3<f32> &coord) const
{
    RailPart* rootPart = this->mRailParts;
    s32 curLargest = 0;
    f32 railPartTotalLen = rootPart->getTotalLength();
    f32 railPartNearParam = rootPart->getNearestParam(coord, (100.0f / railPartTotalLen));

    JGeometry::TVec3<f32> railPartPos;
    rootPart->calcPos(&railPartPos, railPartNearParam);
    railPartPos.sub(coord);
    f32 rootRailPartSqr = railPartPos.squared();
    f32 baseSize = 100.0f;

    s32 anotherIdx = 1;
    f32 f30;

    while (anotherIdx < this->mPointNumLoop)
    {
        RailPart* curPart = &this->mRailParts[anotherIdx];
        f32 curRailTotalLen = curPart->getTotalLength();
        f32 curParam = curPart->getNearestParam(coord, (baseSize / curRailTotalLen));

        JGeometry::TVec3<f32> curRailPos;
        curPart->calcPos(&curRailPos, curParam);
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
        len = this->mSegLengths[curLargest - 1];
    else
        len = 0.0f;

    f32 railPartLen = this->mRailParts[curLargest].getLength(0.0f, f30, 0xA);
    return len + railPartLen;
}

f32 BezierRail::getRailPosCoord(s32 idx) const
{
    if (idx == 0)
        return 0.0f;

    if (!(this->mIsLoop == 0) && !(idx == this->mPointNum - 1))
    {
        idx--;
        return this->mSegLengths[idx];
    }
        
    return this->getTotalLength();
}

void BezierRail::calcCurrentRailCtrlPointIter(JMapInfoIter *iter, f32 a2, bool a3) const
{
    s32 idx = this->getCurrentCtrlPointIndex(a2, a3);
    this->calcRailCtrlPointIter(iter, idx);
}

// this uses one more ldw than it needs to...why?
void BezierRail::calcRailCtrlPointIter(JMapInfoIter *iter, s32 idx) const
{
    const JMapInfo* info = this->mIter->mInfo;
    iter->mPos = idx;
    iter->mInfo = info;
}