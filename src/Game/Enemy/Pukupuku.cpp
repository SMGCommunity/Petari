#include "Game/Enemy/Pukupuku.hpp"
#include "Game/Util/ParabolicPath.hpp"
#include "Game/Util/ValueControl.hpp"
#include "revolution/os.h"

namespace {
    NEW_NERVE(PukupukuStateLandingLandingMoveLand, PukupukuStateLanding, LandingMoveLand);
    NEW_NERVE(PukupukuStateLandingLandingJumpFromWater, PukupukuStateLanding, LandingJumpFromWater);
    NEW_NERVE(PukupukuStateLandingLandingJumpFromLand, PukupukuStateLanding, LandingJumpFromLand);
};  // namespace

PukupukuStateLanding::PukupukuStateLanding(Pukupuku* pParent) : ActorStateBase< Pukupuku >("プクプク地上") {
    mParent = pParent;
    mPath = nullptr;
    mValueCtrl = nullptr;
    _18 = 0;
    mPath = new ParabolicPath();
    mValueCtrl = new ValueControl(30);
    initNerve(&PukupukuStateLandingLandingMoveLand::sInstance);
    kill();
}

/* https://decomp.me/scratch/JXtwR */
void PukupukuStateLanding::exeLandingMoveLand() {
    if (MR::isFirstStep(this)) {
        setupJumping(300.0f, 15.0f);
        mParent->startAnim("Bound", "CloseEye");
        emitGroundHitEffect();
    }

    mValueCtrl->update();
    updateJumping();
    mParent->updatePoseByRailIgnoreUpScale();

    bool move = false;

    if (mValueCtrl->isFrameMaxFrame() || MR::isBinded(mParent) && 0.5f < mValueCtrl->getValue()) {
        move = true;
    }

    if (move) {
        Pukupuku* parent = mParent;
        MR::moveCoordToRailPoint(parent, MR::getNextRailPointNo(parent));

        if (MR::isBinded(mParent)) {
            mParent->mVelocity.zero();
        }

        setNerveAfterJumpAccordingToNextPoint();
    }
}

void PukupukuStateLanding::exeLandingJumpFromWater() {
    if (MR::isFirstStep(this)) {
        _18 = 0;
        setupJumping(500.0f, 15.0f);
        mParent->rotatePoseByLocalZ();
        mParent->startAnim("FlyStart", "OpenEye");
        MR::startSound(mParent, "SE_EM_PUKUPUKU_WATER_OUT", -1, -1);
    }

    mValueCtrl->update();
    updateJumping();
    emitWaterColumIfNeed(true, false);
    updatePoseByJumpPath(0.15f);

    bool move = false;

    if (mValueCtrl->isFrameMaxFrame() || MR::isBinded(mParent) && 0.5f < mValueCtrl->getValue()) {
        move = true;
    }

    if (move) {
        Pukupuku* parent = mParent;
        MR::moveCoordToRailPoint(parent, MR::getNextRailPointNo(parent));

        if (MR::isBinded(mParent)) {
            mParent->mVelocity.zero();
        }

        setNerveAfterJumpAccordingToNextPoint();
    }
}

void PukupukuStateLanding::exeLandingJumpFromLand() {
    if (MR::isFirstStep(this)) {
        _18 = 0;
        setupJumping(500.0f, 15.0f);
        mParent->startAnim("FlyEnd", "OpenEye");
        MR::startSound(mParent, "SE_EM_PUKUPUKU_WING", -1, -1);
        emitGroundHitEffect();
    }

    mValueCtrl->update();
    updateJumping();
    emitWaterColumIfNeed(false, false);
    updatePoseByJumpPath(0.15f);

    if (mValueCtrl->isFrameMaxFrame()) {
        MR::moveCoordAndTransToNextPoint(mParent);
        emitWaterColumIfNeed(false, true);
        kill();
    }
}

void PukupukuStateLanding::updatePoseByJumpPath(f32 a1) {
    TVec3f direction;
    mPath->calcDirection(&direction, mValueCtrl->getValue(), 0.009f);
    MR::normalize(&direction);
    TVec3f v7 = -mParent->mVelocity;
    Pukupuku* parent = mParent;
    if (!MR::isSameDirection(direction, v7, 0.009f)) {
        TQuat4f quat;
        MR::makeQuatFromVec(&quat, direction, v7);
        parent->_A8.slerp(quat, a1);
    }
}

void PukupukuStateLanding::setNerveAfterJumpAccordingToNextPoint() {
    if (mParent->isReadyToJumpFromLand()) {
        setNerve(&PukupukuStateLandingLandingJumpFromLand::sInstance);
    } else {
        setNerve(&PukupukuStateLandingLandingMoveLand::sInstance);
    }
}

// PukupukuStateLanding::setupJumping

void PukupukuStateLanding::updateJumping() {
    TVec3f pos;
    mPath->calcPosition(&pos, mValueCtrl->getValue());
    pos.sub(mParent->mPosition);
    mParent->mVelocity.set< f32 >(pos);
}

void PukupukuStateLanding::emitWaterColumIfNeed(bool a1, bool a2) {
    if (!_18) {
        TVec3f centerJointPos;
        MR::copyJointPos(mParent, "Center", &centerJointPos);

        if (a2 || a1 != MR::isInWater(centerJointPos)) {
            TPos3f hitMtx;
            mParent->calcGroundHitMtx(&hitMtx);
            f32 num = 1.5f;
            hitMtx.mMtx[0][3] = centerJointPos.x;
            hitMtx.mMtx[1][3] = centerJointPos.y;
            hitMtx.mMtx[2][3] = centerJointPos.z;
            hitMtx.mMtx[0][0] = hitMtx.mMtx[0][0] * num;
            hitMtx.mMtx[0][1] = hitMtx.mMtx[0][1] * num;
            hitMtx.mMtx[0][2] = hitMtx.mMtx[0][2] * num;
            hitMtx.mMtx[1][0] = hitMtx.mMtx[1][0] * num;
            hitMtx.mMtx[1][1] = hitMtx.mMtx[1][1] * num;
            hitMtx.mMtx[1][2] = hitMtx.mMtx[1][2] * num;
            hitMtx.mMtx[2][0] = hitMtx.mMtx[2][0] * num;
            hitMtx.mMtx[2][1] = hitMtx.mMtx[2][1] * num;
            hitMtx.mMtx[2][2] = hitMtx.mMtx[2][2] * num;
            MR::emitEffectHit(mParent, hitMtx, "WaterColumn");
            _18 = 1;
        }
    }
}
