#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Map/HitInfo.hpp"
#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioConst.hpp"
#include "Game/Util/MathUtil.hpp"

void Mario::checkTornado() {
    if (mMovementStates._1) {
        mMovementStates._2B = false;
    }
}

void Mario::resetTornado() {
    _530 = 0.0f;
    _534 = 0;
    _538 = 0.0f;
    mMovementStates._F = false;
    _544 = 0;
    mYAngleOffset = 0.0f;
    _3F0 = 1.0f;
}

void Mario::calcTornadoTilt() {
    bool flying = false;
    bool spinning = mMovementStates._F;
    if (getPlayerMode() == PlayerMode_Bee) {
        if (mMovementStates.jumping && mMovementStates._11) {
            flying = true;
        }
    }
    bool tilted = flying | spinning;
    if (!isStickOn() || !tilted) {
        _548 *= mActor->getConst().getTable()->mTornadoTiltCancel;
        bool blended;
        if (tilted) {
            blended = MR::vecBlendSphere(_54C, mHeadVec, &_54C, mActor->getConst().getTable()->mTornadoTiltSpeed);
        } else {
            blended = MR::vecBlendSphere(_54C, mHeadVec, &_54C, mActor->getConst().getTable()->mTornadoTiltOffSpeed);
        }
        if (!blended) {
            _54C = mHeadVec;
        }
    } else {
        TVec3f tilt(getWorldPadDir() * mActor->getConst().getTable()->mTornadoTiltAngle +
                    mHeadVec * (1.0f - mActor->getConst().getTable()->mTornadoTiltAngle));
        MR::normalize(&tilt);
        bool blended = MR::vecBlendSphere(_54C, tilt, &_54C, mActor->getConst().getTable()->mTornadoTiltSpeed);
        MR::normalize(&_54C);
        f32 alignment = __fabsf(getWorldPadDir().dot(mFrontVec));
        _548 = _548 * mActor->getConst().getTable()->mTornadoTiltNear + alignment * (1.0f - mActor->getConst().getTable()->mTornadoTiltNear);
        if (!blended) {
            _54C = tilt;
        }
    }
}

void Mario::reflectWallOnSpinning(const TVec3f& normal, u16 time) {
    setFrontVecKeepUp(normal);
    _3F8 = time;
    _328 = mFrontVec;
    doSpinWallEffect();
}

void Mario::forceStopTornado() {
    if (mMovementStates._F) {
        _40A = mActor->getConst().getTable()->mTornadoRestartTime;
    }
    resetTornado();
    if (mMovementStates.jumping) {
        cancelTornadoJump();
    }
    mDrawStates._8 = true;
}

void Mario::startRotationTask(u32 flags) {
    pushTask(&Mario::taskOnRotation, flags);
}

void Mario::doSpinWallEffect() {
    if ((!mMovementStates._8 || !mFrontWallTriangle->mSensor->isType(0x55)) && (!mMovementStates._19 || !mBackWallTriangle->mSensor->isType(0x55)) &&
        (!mMovementStates._1A || !mSideWallTriangle->mSensor->isType(0x55))) {
        playSound("壁反射");
        playSound("声スピンキャンセル");
        playEffect("壁スパーク");
    }
}

bool Mario::taskOnRotation(u32 flags) {
    if (flags & 4) {
        if (!isAnimationRun("ヘリコプタージャンプ")) {
            mYAngleOffset = 0.0f;
            return false;
        }
        if (isRising()) {
            mYAngleOffset += mActor->getConst().getTable()->mTrampleBegomaRotRise;
        } else {
            mYAngleOffset += mActor->getConst().getTable()->mTrampleBegomaRotFall;
        }
    }
    return true;
}
