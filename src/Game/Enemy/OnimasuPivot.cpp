#include "Game/Enemy/OnimasuPivot.hpp"
#include "Game/Util.hpp"

void OnimasuPivot_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)MR::epsilon();
    (void)2.0f;
}

OnimasuPivot::OnimasuPivot(const char* pName) : Onimasu(pName), mCurNormal(), mNormals() {
    _110.set(0.0f, 0.0f, 0.0f, 1.0f);
    _120.set(0.0f, 0.0f, 0.0f, 1.0f);
}

void OnimasuPivot::initFromRailPoint() {
    mNormals.init(MR::getRailPointNum(this) / 2);
}

void OnimasuPivot::startMoveInner() {
    TVec3f pivot = getPivotPointPos();
    TVec3f toLastPoint = getLastPointPos() - pivot;
    TVec3f toNextPoint = getNextPointPos() - pivot;

    if (MR::isSameDirection(toLastPoint, toNextPoint)) {
        TPos3f mtx;
        mtx.identity();
        mtx.setRotate(_BC, PI * 0.001f);
        mtx.mult(toLastPoint, toLastPoint);
    }

    MR::makeQuatFromVec(&_110, toLastPoint, _BC);
    MR::makeQuatFromVec(&_120, toNextPoint, _BC);
}

const TVec3f OnimasuPivot::getLastPointPos() const {
    TVec3f railPointPos;
    MR::calcRailPointPos(&railPointPos, this, getLastPointNo() * 2);

    return railPointPos;
}

const TVec3f OnimasuPivot::getPivotPointPos() const {
    TVec3f railPointPos;
    MR::calcRailPointPos(&railPointPos, this, (getLastPointNo() * 2) + 1);

    return railPointPos;
}

const TVec3f OnimasuPivot::getNextPointPos() const {
    TVec3f railPointPos;
    MR::calcRailPointPos(&railPointPos, this, mCurNormal * 2);

    return railPointPos;
}

void OnimasuPivot::updatePoseInner() {
    f32 slerpFactor = static_cast< f32 >(getNerveStep()) / static_cast< f32 >(getTimeToNextPoint());

    if (1.0f < slerpFactor) {
        slerpFactor = 1.0f;
    }

    TQuat4f quat;
    quat.set(_110);
    quat.slerp(_120, slerpFactor);

    TVec3f fromPivotPoint(gZeroVec);

    quat.getZDir(fromPivotPoint);

    // This constant is 400.0f * sqrt(2.0f), which is hardcoded
    // for the distance between points in toy time galaxy.
    fromPivotPoint.setLength(565.6854f);

    fromPivotPoint.add(getPivotPointPos());

    mPosition.set(fromPivotPoint);
}

s32 OnimasuPivot::getNextPointNo() const {
    return mCurNormal * 2;
}

s32 OnimasuPivot::getLastPointNo() const {
    s32 n = mCurNormal - 1;

    if (n < 0) {
        return (MR::getRailPointNum(this) / 2) - 1;
    }

    return n;
}

const TVec3f& OnimasuPivot::getLastPointNormal() const {
    return mNormals[getLastPointNo()];
}

const TVec3f& OnimasuPivot::getNextPointNormal() const {
    return mNormals[mCurNormal];
}

void OnimasuPivot::incrementNextPoint() {
    mCurNormal++;

    if (mCurNormal == MR::getRailPointNum(this) / 2) {
        mCurNormal = 0;
    }
}

void OnimasuPivot::collectRailPointInfo() {
    for (s32 i = 0; i < mNormals.size(); i++) {
        TVec3f v5(gZeroVec);
        TVec3f v4(gZeroVec);
        OnimasuFunction::getPolygonOnRailPoint(&v4, &v5, this, i * 2);
        mNormals[i].set(v5);
    }
}
