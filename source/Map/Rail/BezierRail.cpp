#include "Map/Rail/BezierRail.h"
#include "MR/JMap/JMapUtil.h"
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