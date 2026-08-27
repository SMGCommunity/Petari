#include "Game/Player/MarioStep.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioConst.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "revolution/mtx.h"

void Mario::checkStep() {
    if (mMovementStates.jumping) {
        return;
    }

    if (mMovementStates._A) {
        return;
    }

    if (!mMovementStates._1) {
        return;
    }

    if (isStatusActive(MarioStatus_Hang)) {
        return;
    }

    if (isStatusActive(MarioStatus_Wall)) {
        return;
    }

    if (isStatusActive(MarioStatus_Swim)) {
        return;
    }

    if (isStatusActive(MarioStatus_Climb)) {
        return;
    }

    if (isPlayerModeHopper()) {
        return;
    }

    if (isDamaging()) {
        return;
    }

    if (isSwimming()) {
        return;
    }

    if (!MR::isNearZero(_184)) {
        return;
    }

    if (!isStickOn()) {
        return;
    }

    if (_750) {
        return;
    }

    if (mDrawStates._C) {
        return;
    }

    if (isSlipPolygon(_45C)) {
        return;
    }

    if (_4D8->isValid() && !mMovementStates._8 && mMovementStates._1) {
        const f32 frontDot = mFrontVec.dot(*_4D8->getNormal(0));
        TVec3f horizontal;
        MR::vecKillElement(_50C - mPosition, -*getGravityVec(), &horizontal);

        if (horizontal.length() < 20.0f && MR::abs(frontDot) < 0.3926991f) {
            f32 stepHeight = (_50C - mPosition).dot(-*getGravityVec());
            if (stepHeight < mActor->getConst().getTable()->mWalkStepHeight) {
                startStep(_50C);
            }
        } else if (MR::abs(frontDot) < 1.0471976f) {
            Mtx rotMtx;
            PSMTXRotAxisRad(rotMtx, &mSideVec, frontDot);
            PSMTXMultVec(rotMtx, &mVelocity, &mVelocity);
        }
        return;
    }

    if (!mMovementStates._15) {
        return;
    }

    if (!mMovementStates._39) {
        return;
    }

    const TVec3f* worldPadDir = &getWorldPadDir();
    if (!(worldPadDir->dot(*mFrontWallTriangle->getNormal(0)) < -0.5f)) {
        return;
    }

    f32 stepHeight = (_4A4 - mPosition).dot(-*getGravityVec());
    if (stepHeight < mActor->getConst().getTable()->mWalkStepHeight) {
        startStep(_4A4);
    }
}

void Mario::startStep(const TVec3f& rVec) {
    if (getCurrentStatus() == 0x10) {
        return;
    }

    TVec3f gravity(*getGravityVec());
    const f32 gravityScale = 80.0f;
    gravity.x *= gravityScale;
    gravity.y *= gravityScale;
    gravity.z *= gravityScale;

    TVec3f checkPos = rVec - gravity;
    HitInfo hitInfo;

    if (MR::checkStrikePointToMap(checkPos, &hitInfo)) {
        return;
    }

    if (Collision::checkStrikeBallToMap(checkPos, 20.0f, nullptr, nullptr)) {
        return;
    }

    if (Collision::checkStrikeBallToMap(checkPos, 50.0f, nullptr, nullptr)) {
        return;
    }

    TVec3f oldPos(mPosition);
    mPosition = rVec;
    const f32 ceilDist = calcDistToCeil(false);
    mPosition = oldPos;

    if (ceilDist < 200.0f) {
        return;
    }

    if (_4C8->isValid() && MR::isSensorPressObj(_4C8->mSensor)) {
        return;
    }

    if (_4D8->isValid()) {
        if (MR::isSensorPressObj(_4D8->mSensor)) {
            return;
        }

        MtxPtr prevBase = _4D8->getPrevBaseMtx()->toMtxPtr();
        MtxPtr base = _4D8->getBaseMtx()->toMtxPtr();

        if (!MR::isSameMtx(base, prevBase) && _4D8->mSensor != mGroundPolygon->mSensor) {
            return;
        }
    }

    setTrans(rVec, "段差");
    changeStatus(mStep);
    _3D0 = 0;
    const f32 zero = 0.0f;
    mVelocity.z = zero;
    mVelocity.y = zero;
    mVelocity.x = zero;
    mMovementStates._10 = false;
}

MarioStep::MarioStep(MarioActor* pActor) : MarioState(pActor, MarioStatus_Step) {
    _14 = 0.0f;
    _18 = 0.0f;
}

bool MarioStep::start() {
    _18 = getPlayer()->mWalkSpeed;
    MR::clamp(_18, 0.0f, 1.0f);

    s32 v2 = 20 - static_cast< s32 >(16.0f * _18);

    if (v2 < 4) {
        v2 = 4;
    }

    mActor->setBlendMtxTimer(v2);
    _14 = static_cast< u16 >(v2);

    if (isAnimationRun(nullptr) && !mActor->_3E5) {
        stopAnimation(nullptr);
    }

    return true;
}

bool MarioStep::update() {
    if (mActor->isRequestJump()) {
        getPlayer()->tryJump();
        return false;
    } else if (getPlayer()->mMovementStates.jumping) {
        return false;
    }

    getPlayer()->mainMove();
    clearVelocity();

    f32 _278 = getPlayer()->mWalkSpeed;
    if (_278 < 0.1f) {
        _278 = 0.1f;
    }

    getPlayer()->mWalkSpeed = _278;
    getPlayer()->updateWalkSpeed();

    if (_278 < 0.5f) {
        _278 = 0.5f;
    }

    f32 v7 = _14 - _278;
    _14 -= _278;

    if (v7 < 0.0f) {
        return false;
    } else {
        mActor->setBlendMtxTimer(v7);
        return true;
    }
}

bool MarioStep::close() {
    return true;
}

namespace NrvMarioActor {
    INIT_NERVE(MarioActorNrvWait);
    INIT_NERVE(MarioActorNrvGameOver);
    INIT_NERVE(MarioActorNrvGameOverAbyss);
    INIT_NERVE(MarioActorNrvGameOverAbyss2);
    INIT_NERVE(MarioActorNrvGameOverFire);
    INIT_NERVE(MarioActorNrvGameOverBlackHole);
    INIT_NERVE(MarioActorNrvGameOverNonStop);
    INIT_NERVE(MarioActorNrvGameOverSink);
    INIT_NERVE(MarioActorNrvTimeWait);
    INIT_NERVE(MarioActorNrvNoRush);
};  // namespace NrvMarioActor
