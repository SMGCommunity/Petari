#include "Game/Player/Mario.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioConst.hpp"
#include "Game/Map/HitInfo.hpp"

void Mario::checkTornado() {
    if (mMovementStates._1) {
        mMovementStates._2B = 0;
    }
}

void Mario::resetTornado() {
    _530 = 0.0f;
    _534 = 0;
    _538 = 0.0f;
    mMovementStates._F = 0;
    _544 = 0;
    mYAngleOffset = 0.0f;
    _3F0 = 1.0f;
}

// regswap + bad code ordering
void Mario::calcTornadoTilt() {
    bool var = false;
    bool state = mMovementStates._F;
    if (getPlayerMode() == 4 && mMovementStates._0 && mMovementStates._11) {
        var = true;
    }
    var = (var | state) != false;
    if (!isStickOn() || !var) {
        _548 = _548 * mActor->mConst->mTable[mActor->mConst->mCurrentTable]->mTornadoTiltCancel;
        s32 a1;
        if (var) {
            a1 = MR::vecBlendSphere(_54C, mHeadVec, &_54C, mActor->mConst->mTable[mActor->mConst->mCurrentTable]->mTornadoTiltSpeed);
        }
        else {
            a1 = MR::vecBlendSphere(_54C, mHeadVec, &_54C, mActor->mConst->mTable[mActor->mConst->mCurrentTable]->mTornadoTiltOffSpeed);
        }
        if (a1 == 0) {
            _54C = mHeadVec;
        }
    }
    else {
        TVec3f temp = getWorldPadDir() * mActor->mConst->mTable[mActor->mConst->mCurrentTable]->mTornadoTiltAngle + mHeadVec * (1.0f - mActor->mConst->mTable[mActor->mConst->mCurrentTable]->mTornadoTiltAngle); 
        MR::normalize(&temp);
        s32 a2 = MR::vecBlendSphere(_54C, temp, &_54C, mActor->mConst->mTable[mActor->mConst->mCurrentTable]->mTornadoTiltSpeed);
        MR::normalize(&temp);
        _548 = (1.0f - mActor->mConst->mTable[mActor->mConst->mCurrentTable]->mTornadoTiltNear) * __fabsf(getWorldPadDir().dot(mFrontVec)) + _548 * mActor->mConst->mTable[mActor->mConst->mCurrentTable]->mTornadoTiltNear;
        if (a2 == 0) {
            _54C = temp;
        }
    }
}

void Mario::reflectWallOnSpinning(const TVec3f &rVec, u16 a2) {
    setFrontVecKeepUp(rVec);
    _3F8 = a2;
    _328 = mFrontVec;
    doSpinWallEffect();
}

void Mario::forceStopTornado() {
    if (mMovementStates._F) {
        _40A = mActor->mConst->mTable[mActor->mConst->mCurrentTable]->mTornadoRestartTime;
    }
    resetTornado();
    if (mMovementStates._0) {
        cancelTornadoJump();
    }
    mDrawStates._8 = 1;
}

void Mario::doSpinWallEffect() {
    if (!mMovementStates._8 || !mFrontWallTriangle->mSensor->isType(0x55)) {
        if (!mMovementStates._19 || !mBackWallTriangle->mSensor->isType(0x55)) {
            if (!mMovementStates._1A || !mSideWallTriangle->mSensor->isType(0x55)) {
                playSound("壁反射", -1);
                playSound("声スピンキャンセル", -1);
                playEffect("壁スパーク");
            }
        }
    }
}
