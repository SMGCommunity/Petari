#include "Game/Map/BezierRail.hpp"
#include "Game/Map/RailPart.hpp"
#include "Game/Util/MathUtil.hpp"
#include <revolution/mtx.h>

#include "JSystem/JGeometry/TMatrix.hpp"

namespace {
    void calcRailDirection(TVec3f *pOut, const RailPart *pRailPart, f32 a3) {
        pRailPart->calcVelocity(pOut, a3);

        f32 v8;

        if (MR::isNearZero(*pOut, 0.001f)) {
            if ((a3 < 0.5f)) {
                v8 = 0.0099999998f + a3;
            }
            else {
                v8 = a3;
                a3 = a3 - 0.0099999998f;
            }

            TVec3f pos;
            pRailPart->calcPos(&pos, a3);
            pRailPart->calcPos(pOut, v8);

            pOut->sub(pos);
        }

        MR::normalize(pOut);
    }
};

void BezierRailPart::set(const TVec3f &a1, const TVec3f &a2, const TVec3f &a3, const TVec3f &a4) {
    TVec3f v21, v20, v19, v18, v17, v16;

    TVec3f v15(a2);
    v15.sub(a2);

    v21 = v15;
    TVec3f v14(a3);
    v14.sub(a2);

    v20 = v14;
    TVec3f v13(a4);
    v13.sub(a3);

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
    _0 = a1;
    _C = v12;
    _18 = v18;
    _24 = v16;

    mLength = getLength(0.0f, 1.0f, 0xA);
}

void BezierRailPart::calcPos(TVec3f *pOut, f32 a2) const {
    f32 v7 = 3.0f * a2;
    f32 v10 = ((3.0f * a2) * a2);
    f32 v9 = (a2 * (a2 * a2));

    TVec3f v16(_C);
    TVec3f v15(_18);
    TVec3f v14(_24);

    v16.scale(v7);
    v15.scale(v10);
    v14.scale(v9);

    pOut->set(_0);
    pOut->add(v16);
    pOut->add(v15);
    pOut->add(v14);
}

void BezierRailPart::calcVelocity(TVec3f *pOut, f32 a2) const {
    f32 v5 = (a2 * a2);
    f32 v7 = (2.0f * a2);

    TVec3f v12(_24);
    TVec3f v11(_18);
    v12.scale(v5);
    v11.scale(v7);

    pOut->set(_C);
    pOut->add(v11);
    pOut->add(v12);
    pOut->scale(3.0f);
}

// BezierRailPart::getLength
// BezierRailPart::getParam
// BezierRailPart::getNearestParam
// BezierRail::normalizePos

f32 BezierRail::getTotalLength() const {
    return _10[_8 - 1];
}

f32 BezierRail::getPartLength(int idx) const {
    return mRailParts[idx].getTotalLength();
}

void BezierRail::calcPos(TVec3f *pOut, f32 a2) const {
    const RailPart* part;
    f32 param;

    getIncludedSection(&part, &param, a2, 1);
    part->calcPos(pOut, part->getParam(param));
}

void BezierRail::calcDirection(TVec3f *pOut, f32 a2) const {
    const RailPart* part;
    f32 param;

    getIncludedSection(&part, &param, a2, 1);
    calcRailDirection(pOut, part, part->getParam(param));
}

void BezierRail::calcPosDir(TVec3f *pPos, TVec3f *pDir, f32 a3) const {
    const RailPart* part;
    f32 param;

    getIncludedSection(&part, &param, a3, 1);
    f32 val = part->getParam(param);
    part->calcPos(pPos, val);
    calcRailDirection(pDir, part, val);
}

#ifdef NON_MATCHING
// regalloc issues
f32 BezierRail::getNearestRailPosCoord(const TVec3f &a1) const {
    RailPart* part = mRailParts;
    int idx = 0;
    
    f32 length = part->getTotalLength();
    f32 nearestParam = part->getNearestParam(a1, 100.0f / length);

    TVec3f pos;
    part->calcPos(&pos, nearestParam);
    pos.sub(a1);

    f32 sqrt = pos.squared();

    for (int i = 1; i < _8; i++) {
        RailPart* curPart = &mRailParts[i];

        f32 curPartLength = curPart->getTotalLength();
        f32 curNearParam = curPart->getNearestParam(a1, 100.0f / curPartLength);
        curPart->calcPos(&pos, curNearParam);

        pos.sub(a1);
        f32 curSqrt = pos.squared();

        if (curSqrt < sqrt) {
            sqrt = curSqrt;
            idx = i;
            nearestParam = curNearParam;
        }
    }

    f32 val = !idx ? 0.0f : _10[idx - 1];
    return (val + mRailParts[idx].getLength(0.0f, nearestParam, 10));
}
#endif

f32 BezierRail::getRailPosCoord(int idx) const {
    if (!idx) {
        return 0.0f;
    }
    else {
        if (mIsClosed || mPointNum - 1 != idx) {
            return _10[idx - 1];
        }
    }

    return getTotalLength();
}

void BezierRail::calcCurrentRailCtrlPointIter(JMapInfoIter *pIter, f32 a2, bool a3) const {
    int idx = getCurrentCtrlPointIndex(a2, a3);
    calcRailCtrlPointIter(pIter, idx);
}

void BezierRail::calcRailCtrlPointIter(JMapInfoIter *pIter, int idx) const {
    pIter->_4 = idx;
    pIter->mInfo = _18;
}

// BezierRail::getIncludedSection
// BezierRail::getCurrentCtrlPointIndex
// BezierRail::BezierRail
