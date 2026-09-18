#include "Game/Enemy/OnimasuJump.hpp"
#include "Game/Util.hpp"

void OnimasuJump_FORCE_MATCH_SDATA2() {
    (void) 0.0f;
}

OnimasuJump::OnimasuJump(const char* pName) : Onimasu(pName), mCurNormal(), mNormals() {
}

void OnimasuJump::calcJumpVelocity() {
    TVec3f railPointPos;
    MR::calcRailPointPos(&railPointPos, this, mCurNormal);
    MR::onCalcGravity(this);
    f32 distToMoveThisFrame = mPosition.distance(railPointPos) / getTimeToNextPoint();
    f32 jumpStrength = 0.5f * (getGravityScalar() * getTimeToNextPoint());

    TVec3f dirToRailPointPos(railPointPos - mPosition);
    
    if(MR::isNearZero(dirToRailPointPos)) {
        dirToRailPointPos.zero();
    } else {
        MR::normalize(&dirToRailPointPos);
    }

    TVec3f gravityDir;
    calcGravityDir(&gravityDir);

    TVec3f moveToRailPos(dirToRailPointPos * distToMoveThisFrame);

    TVec3f moveJump(-gravityDir * jumpStrength);

    mVelocity.set(moveToRailPos + moveJump);
}

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

    mVelocity.add(gravityDir * getGravityScalar());
}

s32 OnimasuJump::getNextPointNo() const {
    return mCurNormal;
}
