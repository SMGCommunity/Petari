#include "Game/Enemy/OnimasuPivot.hpp"
#include "Game/Util/RailUtil.hpp"

OnimasuPivot::OnimasuPivot(const char* pName) : Onimasu(pName), mCurNormal(), mNormals() {
    _110.set< f32 >(0.0f, 0.0f, 0.0f, 1.0f);
    _120.set< f32 >(0.0f, 0.0f, 0.0f, 1.0f);
}

void OnimasuPivot::initFromRailPoint() {
    s32 railPointNum = MR::getRailPointNum(this) / 2;
    mNormals.init(railPointNum);
}

// OnimasuPivot::startMoveInner
// OnimasuPivot::updatePoseInner

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

// this function will (probably) match once the rest of the file is done. TVec3::set<f> inline.
void OnimasuPivot::collectRailPointInfo() {
    for (s32 i = 0; i < mNormals.size(); i++) {
        TVec3f v5(gZeroVec);
        TVec3f v4(gZeroVec);
        OnimasuFunction::getPolygonOnRailPoint(&v4, &v5, this, i * 2);
        mNormals[i].set< f32 >(v5);
    }
}

OnimasuPivot::~OnimasuPivot() {
}
