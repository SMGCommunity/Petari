#include "Game/Map/BezierRail.hpp"
#include "Game/Map/RailPart.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/StringUtil.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include <revolution/mtx.h>

inline void getCsvDataStr(const char** pOut, const JMapInfo* pMapInfo, const char* pKey, s32 idx) {
    // This SHOULD NOT be here. This is only here to match BezierRail::BezierRail
    // This is an exact copy of MR::getCsvDataStr, but that refuses to inline
    // automatically in BezierRail::BezierRail, so this is used temporarily.
    int v7 = pMapInfo->searchItemInfo(pKey);

    if (v7 >= 0) {
        pMapInfo->getValueFast(idx, v7, pOut);
    }
}

inline s32 getCsvDataElementNum(const JMapInfo* pMapInfo) {
    // This SHOULD NOT be here. This is only here to match BezierRail::BezierRail
    // This is an exact copy of MR::getCsvDataElementNum, but that refuses to inline
    // automatically in BezierRail::BezierRail, so this is used temporarily.
    if (pMapInfo->mData != nullptr) {
        return pMapInfo->mData->mNumEntries;
    }

    return 0;
}

inline int adjustNextIndex(int index, int pointNum) {
    int nextIndex = index + 1;
    int adjust = (nextIndex / pointNum);
    return nextIndex - adjust * pointNum;
}

namespace {
    void calcRailDirection(TVec3f* pOut, const RailPart* pRailPart, f32 t) {
        pRailPart->calcVelocity(pOut, t);

        f32 v8;

        if (MR::isNearZero(*pOut, 0.001f)) {
            if (t < 0.5f) {
                v8 = 0.01f + t;
            } else {
                v8 = t;
                t = t - 0.01f;
            }

            TVec3f pos;
            pRailPart->calcPos(&pos, t);
            pRailPart->calcPos(pOut, v8);

            pOut->sub(pos);
        }

        MR::normalize(pOut);
    }
};  // namespace

void BezierRailPart::set(const TVec3f& rPoint1, const TVec3f& rPoint1Ctrl, const TVec3f& rPoint2Ctrl, const TVec3f& rPoint2) {
    TVec3f v21, v20, v19, v18, v17, v16;

    TVec3f v15(rPoint1Ctrl);
    v15.sub(rPoint1);

    v21 = v15;
    TVec3f v14(rPoint2Ctrl);
    v14.sub(rPoint1Ctrl);

    v20 = v14;
    TVec3f v13(rPoint2);
    v13.sub(rPoint2Ctrl);

    v19 = v13;
    TVec3f v12(v20);
    v12.sub(v21);

    v18 = v12;
    TVec3f v11(v19);
    v11.sub(v20);

    v17 = v11;
    TVec3f v10(v17);
    v10.sub(v18);

    v16 = v10;
    mStart = rPoint1;
    mCtrlDegree1 = v21;
    mCtrlDegree2 = v18;
    mCtrlDegree3 = v16;

    mLength = getLength(0.0f, 1.0f, 10);
}

void BezierRailPart::calcPos(TVec3f* pOut, f32 t) const {
    f32 v7 = 3.0f * t;
    f32 v10 = 3.0f * t * t;
    f32 v9 = t * t * t;

    TVec3f v16(mCtrlDegree1);
    TVec3f v15(mCtrlDegree2);
    TVec3f v14(mCtrlDegree3);

    v16.scale(v7);
    v15.scale(v10);
    v14.scale(v9);

    pOut->set< f32 >(mStart);
    pOut->add(v16);
    pOut->add(v15);
    pOut->add(v14);
}

void BezierRailPart::calcVelocity(TVec3f* pOut, f32 t) const {
    f32 v5 = t * t;
    f32 v7 = 2.0f * t;

    TVec3f v12(mCtrlDegree3);
    TVec3f v11(mCtrlDegree2);
    v12.scale(v5);
    v11.scale(v7);

    pOut->set< f32 >(mCtrlDegree1);
    pOut->add(v11);
    pOut->add(v12);
    pOut->scale(3.0f);
}

f32 BezierRailPart::getLength(f32 t1, f32 t2, int k) const {
    f32 len1, len2;

    f32 m3 = (1.0f / 2.0f) * (getVelocityLength(t1) + getVelocityLength(t2));

    f32 tDiff = (t2 - t1);
    f32 temp = (1.0f / (k * 2.0));
    f32 delta = tDiff * temp;

    len1 = 0.0f;
    len2 = 0.0f;

    for (s32 seg = 1; seg <= k; seg++) {
        s32 idxA = ((seg * 2) - 1);
        s32 idxB = (seg * 2);

        f32 tA = t1 + delta * idxA;
        f32 tB = t1 + delta * idxB;

        len1 += getVelocityLength(tA);

        if (seg != k) {
            len2 += getVelocityLength(tB);
        }
    }

    f32 len = floor(1024.0f * static_cast< f32 >((1.0 / 3.0) * delta * ((len1 * 4.0f) + (m3 + (len2 * 2.0f)))));
    f32 rescale = 1.0f / 1024.0f;
    len *= rescale;
    return len;
}

f32 BezierRailPart::getParam(f32 t) const {
    f32 param = t / mLength;
    f32 railLength = getLength(0.0f, param, 10);

    if (__fabsf(t - railLength) <= 0.01f) {
        return param;
    }

    f32 length = railLength;
    for (s32 count = 0; count < 5; count++) {
        f32 mag = getVelocityLength(param);
        param = JGeometry::TUtil< f32 >::clamp(param + (t - length) / mag, 0.0f, 1.0f);
        length = getLength(0.0f, param, 10);

        f32 limit = 0.01f;
        if (__fabs(t - length) <= limit) {
            return param;
        }
    }

    if (length < 0.0f || 1.0f < param) {
        param = JGeometry::TUtil< f32 >::clamp(param, 0.0f, 1.0f);
    }

    return param;
}

f32 BezierRailPart::getNearestParam(const TVec3f& rPos, f32 delta) const {
    f32 nearestParam = -1.0f;
    f32 nearestMag = FLOAT_MAX;

    for (f32 t = 0.0f; t <= 1.0f; t += delta) {
        TVec3f pos;
        calcPos(&pos, t);
        f32 mag = pos.squared(rPos);
        if (mag < nearestMag) {
            nearestParam = t;
            nearestMag = mag;
        }
    }

    return nearestParam;
}

f32 BezierRail::normalizePos(f32 t, int direction) const {
    if (mIsClosed) {
        f32 pos = MR::mod(t, getTotalLength());
        if (direction < 0 && MR::isNearZero(pos, 0.001f)) {
            pos = getTotalLength();
        }
        if (pos < 0.0f) {
            pos += getTotalLength();
        }
        return pos;
    } else {
        return JGeometry::TUtil< f32 >::clamp(t, 0.0f, getTotalLength());
    }
}

f32 BezierRail::getTotalLength() const {
    return mPointCoords[mNumRailParts - 1];
}

f32 BezierRail::getPartLength(int idx) const {
    return mRailParts[idx].getTotalLength();
}

void BezierRail::calcPos(TVec3f* pOut, f32 t) const {
    const RailPart* part;
    f32 param;

    getIncludedSection(&part, &param, t, 1);
    part->calcPos(pOut, part->getParam(param));
}

void BezierRail::calcDirection(TVec3f* pOut, f32 t) const {
    const RailPart* part;
    f32 param;

    getIncludedSection(&part, &param, t, 1);
    calcRailDirection(pOut, part, part->getParam(param));
}

void BezierRail::calcPosDir(TVec3f* pPos, TVec3f* pDir, f32 t) const {
    const RailPart* part;
    f32 param;

    getIncludedSection(&part, &param, t, 1);
    f32 val = part->getParam(param);
    part->calcPos(pPos, val);
    calcRailDirection(pDir, part, val);
}

f32 BezierRail::getNearestRailPosCoord(const TVec3f& rPos) const {
    RailPart* part = mRailParts;
    int idx = 0;

    f32 length = part->getTotalLength();
    f32 nearestParam = part->getNearestParam(rPos, 100.0f / length);

    TVec3f pos;
    part->calcPos(&pos, nearestParam);
    pos.sub(rPos);

    f32 sqrt = pos.squared();

    for (int i = 1; i < mNumRailParts; i++) {
        RailPart* curPart = getRailPart(i);

        f32 curPartLength = curPart->getTotalLength();
        f32 curNearParam = curPart->getNearestParam(rPos, 100.0f / curPartLength);
        curPart->calcPos(&pos, curNearParam);

        pos.sub(rPos);
        f32 curSqrt = pos.squared();

        if (curSqrt < sqrt) {
            sqrt = curSqrt;
            idx = i;
            nearestParam = curNearParam;
        }
    }

    f32 val = !idx ? 0.0f : mPointCoords[idx - 1];
    return (val + getRailPart(idx)->getLength(0.0f, nearestParam, 10));
}

f32 BezierRail::getRailPosCoord(int idx) const {
    if (!idx) {
        return 0.0f;
    }

    if (mIsClosed || idx != mPointNum - 1) {
        return mPointCoords[idx - 1];
    }

    return getTotalLength();
}

void BezierRail::calcCurrentRailCtrlPointIter(JMapInfoIter* pIter, f32 t, bool forward) const {
    int idx = getCurrentCtrlPointIndex(t, forward);
    calcRailCtrlPointIter(pIter, idx);
}

void BezierRail::calcRailCtrlPointIter(JMapInfoIter* pIter, int idx) const {
    pIter->mIndex = idx;
    pIter->mInfo = mInfo;
}

void BezierRail::getIncludedSection(const RailPart** pRailParts, f32* pPos, f32 t, int direction) const {
    f32 pos = normalizePos(t, direction);
    f32 partPos = 0.0f;
    s32 idx = -1;

    if (direction > 0) {
        for (s32 i = 0; i < mNumRailParts; i++) {
            if (!(mPointCoords[i] <= pos) || i == mNumRailParts - 1) {
                idx = i;

                f32 newPos;
                if (i == 0) {
                    newPos = pos;
                } else {
                    s32 j = i - 1;
                    newPos = pos - mPointCoords[j];
                }
                partPos = newPos;
                break;
            }
        }
    } else {
        for (s32 i = mNumRailParts - 1; i >= 0; i--) {
            if (i == 0 || (mPointCoords[i - 1] < pos && pos <= mPointCoords[i])) {
                idx = i;

                f32 newPos;
                if (i == 0) {
                    newPos = pos;
                } else {
                    s32 j = i - 1;
                    newPos = pos - mPointCoords[j];
                }
                partPos = newPos;
                break;
            }
        }
    }

    RailPart* part = getRailPart(idx);
    pRailParts[0] = part;
    *pPos = JGeometry::TUtil< f32 >::clamp(partPos, 0.0f, part->getTotalLength());
}

int BezierRail::getCurrentCtrlPointIndex(f32 t, bool forward) const {
    f32 pos = normalizePos(t, forward ? 1 : -1);

    if (MR::isNearZero(pos, 0.001f)) {
        return 0;
    }

    f32 totalLength = getTotalLength();
    if (MR::isNearZero(totalLength - pos, 0.001f)) {
        return !mIsClosed ? mPointNum - 1 : 0;
    }

    if (forward) {
        for (int i = 0; i < mNumRailParts; i++) {
            // this matches but is probably not written like this.
            // other "simple" configurations dont seem to match
            f32 pointCoord = mPointCoords[i];
            if (!(pointCoord <= pos)) {
                return i;
            }
        }
    } else {
        for (int i = mNumRailParts - 1; i >= 0; i--) {
            f32 f = i > 0 ? mPointCoords[i - 1] : 0.0f;

            f32 pointCoord = mPointCoords[i];
            if (f < pos && pos <= pointCoord) {
                return adjustNextIndex(i, mPointNum);
            }
        }
    }

    return 0;
}

BezierRail::BezierRail(const JMapInfoIter& rIter, const JMapInfo* pInfo) {
    mInfo = pInfo;
    mIter = new JMapInfoIter(rIter);

    const char* closedValue = "";
    const JMapInfo* info = rIter.mInfo;

    getCsvDataStr(&closedValue, info, "closed", rIter.mIndex);
    mIsClosed = MR::isEqualString(closedValue, "CLOSE");

    mPointNum = getCsvDataElementNum(pInfo);

    mNumRailParts = mPointNum - 1;
    if (mIsClosed) {
        mNumRailParts++;
    }

    f32 pointCoord = 0.0f;
    f32 railStartCoord = 0.0f;

    mRailParts = new RailPart[mNumRailParts];
    mPointCoords = new f32[mNumRailParts];

    for (int i = 0; i < mNumRailParts; i++) {
        JMapInfoIter iterPoint1 = JMapInfoIter(pInfo, i);
        JMapInfoIter iterPoint2 = JMapInfoIter(pInfo, adjustNextIndex(i, mPointNum));

        TVec3f point1;
        TVec3f point2;
        TVec3f point1Ctrl;
        TVec3f point2Ctrl;

        MR::getRailPointPos0(iterPoint1, &point1);
        MR::getRailPointPos2(iterPoint1, &point1Ctrl);
        MR::getRailPointPos1(iterPoint2, &point2Ctrl);
        MR::getRailPointPos0(iterPoint2, &point2);

        mRailParts[i].init(point1, point1Ctrl, point2Ctrl, point2);

        // this is the strangest way to do this calculation I have ever seen
        f32 railEndCoord = railStartCoord;
        f32 prevPointCoord = pointCoord;
        f32 railLength = mRailParts[i].getTotalLength();
        railEndCoord += railLength;
        pointCoord += railEndCoord;
        railEndCoord -= pointCoord - prevPointCoord;
        mPointCoords[i] = pointCoord;
        railStartCoord = railEndCoord;
    }
}
