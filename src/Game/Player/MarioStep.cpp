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

    if (isStatusActive(5)) {
        return;
    }

    if (isStatusActive(1)) {
        return;
    }

    if (isStatusActive(6)) {
        return;
    }

    if (isStatusActive(29)) {
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

    if (mDrawStates.mIsUnderwater) {
        return;
    }

    if (isSlipPolygon(_45C)) {
        return;
    }

    if (_4D8->isValid()) {
        if (!mMovementStates._8 && mMovementStates._1) {
            const f32 frontDot = mFrontVec.dot(*_4D8->getNormal(0));
            TVec3f negGravity = -*getGravityVec();
            TVec3f stepOffset = _50C - mPosition;
            TVec3f horizontal;
            MR::vecKillElement(stepOffset, negGravity, &horizontal);

            if (horizontal.length() < 20.0f) {
                if (__fabsf(frontDot) < 0.3926991f) {
                    TVec3f negGravity2 = -*getGravityVec();
                    TVec3f stepOffset2 = _50C - mPosition;
                    f32 stepHeight = stepOffset2.dot(negGravity2);
                    MarioConst* pConst = mActor->mConst;
                    MarioConstTable* pTable = pConst->mTable[pConst->mCurrentTable];
                    if (stepHeight < pTable->mWalkStepHeight) {
                        startStep(_50C);
                    }
                }
            } else if (__fabsf(frontDot) < 1.0471976f) {
                Mtx rotMtx;
                PSMTXRotAxisRad(rotMtx, &mSideVec, frontDot);
                PSMTXMultVec(rotMtx, &mVelocity, &mVelocity);
            }
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
    if (worldPadDir->dot(*mFrontWallTriangle->getNormal(0)) >= -0.5f) {
        return;
    }

    TVec3f negGravity = -*getGravityVec();
    TVec3f stepOffset = _4A4 - mPosition;
    f32 stepHeight = stepOffset.dot(negGravity);
    MarioConst* pConst = mActor->mConst;
    MarioConstTable* pTable = pConst->mTable[pConst->mCurrentTable];
    if (stepHeight < pTable->mWalkStepHeight) {
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

MarioStep::MarioStep(MarioActor* pActor) : MarioState(pActor, 0x10) {
    _14 = 0.0f;
    _18 = 0.0f;
}

bool MarioStep::start() {
    _18 = getPlayer()->_278;
    f32 v2 = (16.0f * _18);
    u16 v3 = 20 - v2;

    if (v3 < 4) {
        v3 = 4;
    }

    mActor->setBlendMtxTimer(v3);
    _14 = v3;

    if (isAnimationRun(nullptr) && !mActor->_3E5) {
        stopAnimation(nullptr, static_cast<const char*>(nullptr));
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

    f32 _278 = getPlayer()->_278;
    if (_278 < 0.1f) {
        _278 = 0.1f;
    }

    getPlayer()->_278 = _278;
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
