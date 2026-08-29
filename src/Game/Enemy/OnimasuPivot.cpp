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

// OnimasuPivot::collectRailPointInfo

OnimasuPivot::~OnimasuPivot() {
}
