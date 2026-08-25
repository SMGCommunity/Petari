#include "Game/Map/SphereSelectorHandle.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/SphereSelector.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util.hpp"

namespace {
    const s32 cHoldReduceOutScreenFrame = 5;
    //  const s32 cRotateSpeedChangeMax
    //  const f32 cHoldReduceRate
    const f32 cSpinReduceRate = 0.95f;
    //  const s32 cRotateSpeedMaxY
    //  const f32 cHoldRotateRateY
    const s32 cDemoRotateStartFrame = 60;
    const f32 cDemoRotateSpeedY = 0.03f;
    //  const s32 cRotateSpeedMaxX
    //  const f32 cHoldRotateRateX
    const f32 cRotateAppearX = 40.0f;
    const f32 cRotateMinX = -15.0f;
    const f32 cRotateMaxX = 80.0f;
    const s32 cBgmAppearState = 2;
    const s32 cBgmAppearFrames = 60;
    const s32 cBgmDisappearState = 1;
    const s32 cBgmDisappearFrames = 90;
    const s32 cBgmRotateState = 4;
    const s32 cBgmRotateFrames = 60;
    const s32 cBgmNotRotateState = 3;
    const s32 cBgmNotRotateFrames = 30;
    const s32 cBgmConfirmState = 6;
    const s32 cBgmConfirmFrames = 60;
    const s32 cBgmNotConfirmState = 5;
    const s32 cBgmNotConfirmFrames = 60;
};  // namespace

namespace NrvSphereSelectorHandle {
    NEW_NERVE(SphereSelectorHandleNrvWait, SphereSelectorHandle, Wait);
    NEW_NERVE(SphereSelectorHandleNrvHold, SphereSelectorHandle, Hold);
    NEW_NERVE(SphereSelectorHandleNrvSpin, SphereSelectorHandle, Spin);
    NEW_NERVE(SphereSelectorHandleNrvDemoRotate, SphereSelectorHandle, DemoRotate);
    NEW_NERVE(SphereSelectorHandleNrvDisappear, SphereSelectorHandle, Disappear);
    NEW_NERVE(SphereSelectorHandleNrvInvalidRotate, SphereSelectorHandle, InvalidRotate);
    NEW_NERVE(SphereSelectorHandleNrvGalaxyConfirmStart, SphereSelectorHandle, GalaxyConfirmStart);
    NEW_NERVE(SphereSelectorHandleNrvGalaxyConfirmWait, SphereSelectorHandle, GalaxyConfirmWait);
    NEW_NERVE(SphereSelectorHandleNrvGalaxyConfirmCancel, SphereSelectorHandle, GalaxyConfirmCancel);
    NEW_NERVE(SphereSelectorHandleNrvIdleForFileSelect, SphereSelectorHandle, IdleForFileSelect);
    NEW_NERVE(SphereSelectorHandleNrvIdleEndForFileSelect, SphereSelectorHandle, IdleEndForFileSelect);
};  // namespace NrvSphereSelectorHandle

SphereSelectorHandle::SphereSelectorHandle(const char* pName)
    : LiveActor(pName), mIsFileSelectMode(), _C0(0.0f, 0.0f, -1.0f), mRotateSpeed(), mTargetRotateSpeed(), _D4(), _D8(), _DC(1.0f, 0.0f, 0.0f),
      _E8(0.0f, 1.0f, 0.0f), _F4(), _10C(), _110(gZeroVec), _11C(0.0f, 1.0f, 0.0f), _128() {
    clearPointerVelocity();
    _90.identity();
}

void SphereSelectorHandle::init(const JMapInfoIter& rIter) {
    MR::connectToScene(this, MR::MovementType_Environment, MR::CalcAnimType_MapObj, -1, -1);
    MR::getJMapInfoArg0NoInit(rIter, &mIsFileSelectMode);
    MR::invalidateClipping(this);
    initNerve(&NrvSphereSelectorHandle::SphereSelectorHandleNrvWait::sInstance);
    MR::tryRegisterDemoCast(this, rIter);
    MR::registerDemoSimpleCastAll(this);
    SphereSelectorFunction::registerTarget(this);
    SphereSelectorFunction::setHandle(this);
    makeActorDead();
}

void SphereSelectorHandle::appear() {
    LiveActor::appear();
    _C0.negate(MR::getCamZdir());
    _C0.y = 0.0f;
    MR::normalize(&_C0);
    resetRotateParam();
    _10C = 0;
    clearPointerVelocity();
    mRotation.zero();
    _DC.cross(TVec3f(0.0f, 1.0f, 0.0f), _C0);
    MR::rotateVecDegree(&_E8, TVec3f(0.0f, 1.0f, 0.0f), _DC, ::cRotateAppearX);
    MR::normalize(&_E8);
    MR::setStageBGMState(::cBgmAppearState, ::cBgmAppearFrames);
    setNerve(&NrvSphereSelectorHandle::SphereSelectorHandleNrvWait::sInstance);
}

bool SphereSelectorHandle::isPointing() const {
    return MR::isStarPointerInScreen(0);
}

bool SphereSelectorHandle::isHolding() const {
    return isNerve(&NrvSphereSelectorHandle::SphereSelectorHandleNrvHold::sInstance);
}

void SphereSelectorHandle::validateRotate() {
    setNerve(&NrvSphereSelectorHandle::SphereSelectorHandleNrvWait::sInstance);
}

void SphereSelectorHandle::invalidateRotate() {
    setNerve(&NrvSphereSelectorHandle::SphereSelectorHandleNrvInvalidRotate::sInstance);
}

void SphereSelectorHandle::control() {
    rotateAxisY();
    rotateAxisX();
    updateBaseMtx();
    changeBgmRotateState();
    playRotateSE();
    if (isPointing() || MR::isDemoActive()) {
        _10C = 0;
    } else {
        _10C++;
    }
}

bool SphereSelectorHandle::isWaitOrSpinOrDemoRotate() {
    return (isNerve(&NrvSphereSelectorHandle::SphereSelectorHandleNrvWait::sInstance) ||
            isNerve(&NrvSphereSelectorHandle::SphereSelectorHandleNrvSpin::sInstance) ||
            isNerve(&NrvSphereSelectorHandle::SphereSelectorHandleNrvDemoRotate::sInstance));
}

bool SphereSelectorHandle::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (SphereSelectorFunction::isMsgSelectStart(msg)) {
        appear();
        return true;
    }
    if (SphereSelectorFunction::isMsgSelectEnd(msg)) {
        setNerve(&NrvSphereSelectorHandle::SphereSelectorHandleNrvDisappear::sInstance);
        return true;
    }
    if (SphereSelectorFunction::isMsgConfirmStart(msg)) {
        if (mIsFileSelectMode) {
            setNerve(&NrvSphereSelectorHandle::SphereSelectorHandleNrvIdleForFileSelect::sInstance);
        } else {
            setNerve(&NrvSphereSelectorHandle::SphereSelectorHandleNrvGalaxyConfirmStart::sInstance);
        }
        return true;
    }
    if (SphereSelectorFunction::isMsgConfirmCancel(msg)) {
        if (mIsFileSelectMode) {
            setNerve(&NrvSphereSelectorHandle::SphereSelectorHandleNrvIdleEndForFileSelect::sInstance);
        } else {
            setNerve(&NrvSphereSelectorHandle::SphereSelectorHandleNrvGalaxyConfirmCancel::sInstance);
        }
        return true;
    }
    if (SphereSelectorFunction::isMsgTargetSelected(msg)) {
        if (isWaitOrSpinOrDemoRotate()) {
            setNerve(&NrvSphereSelectorHandle::SphereSelectorHandleNrvHold::sInstance);
            return true;
        }
    }
    return false;
}

bool SphereSelectorHandle::tryRelease() {
    if (!SphereSelectorFunction::isPadButton()) {
        setNerve(&NrvSphereSelectorHandle::SphereSelectorHandleNrvSpin::sInstance);
        return true;
    }
    return false;
}

void SphereSelectorHandle::clearPointerVelocity() {
    for (int i = 0; i < ARRAY_SIZE(_F4); i++) {
        _F4[i].zero();
    }
}

void SphereSelectorHandle::stackPointerVelocity() {
    for (u32 i = 1; i < ARRAY_SIZE(_F4); i++) {
        _F4[i].set(_F4[i - 1]);
    }
    _F4[0].set(*MR::getStarPointerScreenVelocity(0));
}

TVec2f* SphereSelectorHandle::getPointerVelocity() {
    u32 max = 0;
    for (u32 i = 1; i < ARRAY_SIZE(_F4); i++) {
        if (_F4[max].length() < _F4[i].length()) {
            max = i;
        }
    }
    return &_F4[max];
}

void SphereSelectorHandle::resetRotateParam() {
    mRotateSpeed = 0.0f;
    mTargetRotateSpeed = 0.0f;
    _D4 = 0.0f;
    _D8 = 0.0f;
}

void SphereSelectorHandle::rotateAxisY() {
    MR::clampBoth(&mRotateSpeed, mTargetRotateSpeed - 0.2f, mTargetRotateSpeed + 0.2f);
    MR::clampBoth(&mRotateSpeed, -5.0f, 5.0f);
    mTargetRotateSpeed = mRotateSpeed;
    mRotation.y = MR::repeatDegree(mRotation.y + mRotateSpeed);
}

void SphereSelectorHandle::rotateAxisX() {
    MR::clampBoth(&_D4, _D8 - 0.2f, _D8 + 0.2f);
    MR::clampBoth(&_D4, -5.0f, -5.0f);
    _D8 = _D4;
    MR::rotateVecDegree(&_E8, _DC, _D4);
    MR::normalize(&_E8);
    TVec3f rotateMinUp;
    MR::rotateVecDegree(&rotateMinUp, TVec3f(0.0f, 1.0f, 0.0f), _DC, ::cRotateMinX);
    TVec3f rotateMaxUp;
    MR::rotateVecDegree(&rotateMaxUp, TVec3f(0.0f, 1.0f, 0.0f), _DC, ::cRotateMaxX);
    if (_E8.dot(_C0) < rotateMinUp.dot(_C0)) {
        _E8.set(rotateMinUp);
    } else if (rotateMaxUp.dot(_C0) < _E8.dot(_C0)) {
        _E8.set(rotateMaxUp);
    }
}

void SphereSelectorHandle::updateBaseMtx() {
    TPos3f mtx;
    MR::makeMtxUpFront(&mtx, _E8, _C0);
    mtx.zeroTrans();
    TPos3f mtx2;
    mtx2.makeRotate(TVec3f(0.0f, 1.0f, 0.0f), MR::toRadian(mRotation.y));
    _90.concat(mtx, mtx2);
    _90.setTrans(mPosition);
}

void SphereSelectorHandle::changeBgmRotateState() {
    if (MR::abs(_D4) > ::cDemoRotateSpeedY || MR::abs(mRotateSpeed) > ::cDemoRotateSpeedY) {
        if (!_128) {
            MR::setStageBGMState(::cBgmRotateState, ::cBgmRotateFrames);
        }
        _128 = true;
        return;
    }
    if (_128) {
        MR::setStageBGMState(::cBgmNotRotateState, ::cBgmNotRotateFrames);
    }
    _128 = false;
}

void SphereSelectorHandle::playRotateSE() {
    if (!(MR::abs(_D4) > ::cDemoRotateSpeedY) && !(MR::abs(mRotateSpeed) > ::cDemoRotateSpeedY)) {
        return;
    }
    MR::startAtmosphereLevelSE("SE_AT_LV_ASTRO_DOME_WIND_1", static_cast< s32 >(100.0f * MR::clamp01(MR::abs(_D4) * (MR::abs(mRotateSpeed) / 5.0f))));
    if (!(MR::abs(mRotateSpeed) >= 4.0f) && !(MR::abs(_D4) >= 1.6f)) {
        return;
    }
    MR::startAtmosphereLevelSE("SE_AT_LV_ASTRO_DOME_WIND_2");
}

void SphereSelectorHandle::setStateConfirmStartAtFirstStep() {
    if (MR::isFirstStep(this)) {
        resetRotateParam();
        MR::setStageBGMState(cBgmConfirmState, cBgmConfirmFrames);
    }
}

void SphereSelectorHandle::exeWait() {
    if (MR::isFirstStep(this)) {
        resetRotateParam();
    }
    if (isPointing()) {
        SphereSelectorFunction::registerPointingTarget(this, HandlePointingPriority(1));
    }
    if (_10C > ::cDemoRotateStartFrame) {
        setNerve(&NrvSphereSelectorHandle::SphereSelectorHandleNrvDemoRotate::sInstance);
    }
}

void SphereSelectorHandle::exeHold() {
    if (MR::isFirstStep(this)) {
        resetRotateParam();
        clearPointerVelocity();
        MR::startSystemSE("SE_DM_ASTRO_HANDLE_GRAB");
    }
    if (isPointing()) {
        stackPointerVelocity();
        mRotateSpeed = 0.2f * getPointerVelocity()->x;
        if (mRotateSpeed * mTargetRotateSpeed < 0.0f || MR::abs(mTargetRotateSpeed) < MR::abs(mRotateSpeed)) {
            mRotateSpeed = MR::getLinerValue(0.9f, mRotateSpeed, mTargetRotateSpeed, 0.0f);
        }
        _D4 = 0.75f * getPointerVelocity()->y;
        if (_D4 * _D8 < 0.0f || MR::abs(_D8) > MR::abs(_D4)) {
            _D4 = MR::getLinerValue(0.9f, _D4, _D8, 1.0f);
        }
    } else if (_10C > ::cHoldReduceOutScreenFrame) {
        mRotateSpeed *= ::cSpinReduceRate;
        _D4 *= ::cSpinReduceRate;
    }
    if (tryRelease()) {
        return;
    }
}

void SphereSelectorHandle::exeSpin() {
    mRotateSpeed *= ::cSpinReduceRate;
    _D4 *= ::cSpinReduceRate;
    if (MR::isStarPointerInScreen(0)) {
        SphereSelectorFunction::registerPointingTarget(this, HandlePointingPriority(1));
    }
    if (MR::isNearZero(mRotateSpeed) && MR::isNearZero(_D4)) {
        setNerve(&NrvSphereSelectorHandle::SphereSelectorHandleNrvWait::sInstance);
    }
}

void SphereSelectorHandle::exeDemoRotate() {
    mRotateSpeed = ::cDemoRotateSpeedY;
    if (isPointing()) {
        SphereSelectorFunction::registerPointingTarget(this, HandlePointingPriority(1));
    }
    if (_10C == 0) {
        setNerve(&NrvSphereSelectorHandle::SphereSelectorHandleNrvWait::sInstance);
    }
}

void SphereSelectorHandle::exeDisappear() {
    if (MR::isFirstStep(this)) {
        resetRotateParam();
        MR::setStageBGMState(::cBgmAppearState, ::cBgmAppearFrames);
    }
    if (MR::isStep(this, ::cBgmDisappearFrames)) {
        kill();
    }
}

void SphereSelectorHandle::exeGalaxyConfirmStart() {
    s32 confirmStartCancelFrame = SphereSelectorFunction::getConfirmStartCancelFrame();
    if (MR::isFirstStep(this)) {
        setStateConfirmStartAtFirstStep();
        _110.zero();
    }
    MR::setNerveAtStep(this, &NrvSphereSelectorHandle::SphereSelectorHandleNrvGalaxyConfirmWait::sInstance, confirmStartCancelFrame);
}

void SphereSelectorHandle::exeGalaxyConfirmCancel() {
    s32 confirmStartCancelFrame = SphereSelectorFunction::getConfirmStartCancelFrame();
    if (MR::isFirstStep(this)) {
        MR::setStageBGMState(cBgmNotConfirmState, cBgmNotConfirmFrames);
    }
    mPosition.lerp(_110, TVec3f(0.0f, 0.0f, 0.0f), MR::calcNerveEaseInRate(this, confirmStartCancelFrame));
    MR::setNerveAtStep(this, &NrvSphereSelectorHandle::SphereSelectorHandleNrvWait::sInstance, confirmStartCancelFrame);
}

void SphereSelectorHandle::exeIdleEndForFileSelect() {
    if (MR::isFirstStep(this)) {
        MR::setStageBGMState(cBgmNotConfirmState, cBgmNotConfirmFrames);
    }
    MR::setNerveAtStep(this, &NrvSphereSelectorHandle::SphereSelectorHandleNrvWait::sInstance, SphereSelectorFunction::getConfirmStartCancelFrame());
}

void SphereSelectorHandle::exeIdleForFileSelect() {
    setStateConfirmStartAtFirstStep();
}

void SphereSelectorHandle::exeInvalidRotate() {
    if (MR::isFirstStep(this)) {
        resetRotateParam();
    }
}

void SphereSelectorHandle::exeGalaxyConfirmWait() {
    if (MR::isFirstStep(this)) {
        mPosition.set(_110);
    }
}
