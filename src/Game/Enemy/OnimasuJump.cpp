#include "Game/Enemy/OnimasuJump.hpp"
#include "Game/Util.hpp"

OnimasuJump::OnimasuJump(const char* pName) : Onimasu(pName), mCurNormal(), mNormals() {
}

// OnimasuJump::calcJumpVelocity

s32 OnimasuJump::getLastPointNo() const {
    s32 n = mCurNormal - 1;

    if (n < 0) {
        return MR::getRailPointNum(this) - 1;
    }

    return n;
}

const TVec3f& OnimasuJump::getLastPointNormal() const {
    return mNormals[getLastPointNo()];
}

const TVec3f& OnimasuJump::getNextPointNormal() const {
    return mNormals[mCurNormal];
}

void OnimasuJump::initFromRailPoint() {
    mNormals.init(MR::getRailPointNum(this));
}

void OnimasuJump::incrementNextPoint() {
    mCurNormal++;

    if (mCurNormal == MR::getRailPointNum(this)) {
        mCurNormal = 0;
    }
}

void OnimasuJump::collectRailPointInfo() {
    for (s32 i = 0; i < mNormals.size(); i++) {
        TVec3f v5(gZeroVec);
        TVec3f v4(gZeroVec);
        OnimasuFunction::getPolygonOnRailPoint(&v4, &v5, this, i);
        mNormals[i].set(v5);
    }
}

void OnimasuJump::startMoveInner() {
    calcJumpVelocity();
}

void OnimasuJump::updatePoseInner() {
    TVec3f gravityDir;
    calcGravityDir(&gravityDir);

    mVelocity += gravityDir * getGravityScalar();
}

s32 OnimasuJump::getNextPointNo() const {
    return mCurNormal;
}
