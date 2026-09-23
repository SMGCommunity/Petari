#include "Game/Map/SphereSelectorHandle.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/Map/SphereSelector.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Util.hpp"

void SphereSelectorHandle_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
    (void)-1.0f;
    (void)2.0f;
}

namespace {
    const s32 cHoldReduceOutScreenFrame = 5;
    const f32 cRotateSpeedChangeMax = 0.2f;
    const f32 cHoldReduceRate = 0.9f;
    const f32 cSpinReduceRate = 0.95f;
    const f32 cRotateSpeedMaxY = 5.0f;
    const f32 cHoldRotateRateY = 0.2f;
    const s32 cDemoRotateStartFrame = 60;
    const f32 cDemoRotateSpeedY = 0.03f;
    const f32 cRotateSpeedMaxX = 2.0f;
    const f32 cHoldRotateRateX = 0.075f;
    const f32 cRotateAppearX = 40.0f;
    const f32 cRotateMinX = -15.0f;
    const f32 cRotateMaxX = 80.0f;
    s32 cBgmAppearState = 2;
    s32 cBgmAppearFrames = 60;
    s32 cBgmDisappearState = 1;
    s32 cBgmDisappearFrames = 90;
    s32 cBgmRotateState = 4;
    s32 cBgmRotateFrames = 60;
    s32 cBgmNotRotateState = 3;
    s32 cBgmNotRotateFrames = 30;
    s32 cBgmConfirmState = 6;
    s32 cBgmConfirmFrames = 60;
    s32 cBgmNotConfirmState = 5;
    s32 cBgmNotConfirmFrames = 60;
}  // namespace

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
}  // namespace NrvSphereSelectorHandle

SphereSelectorHandle::SphereSelectorHandle(const char* pName)
    : LiveActor(pName), mIsFileSelectMode(), _C0(0.0f, 0.0f, -1.0f), mRotateSpeed(), mTargetRotateSpeed(), _D4(), _D8(), _DC(1.0f, 0.0f, 0.0f),
      _E8(0.0f, 1.0f, 0.0f), _F4(), _10C(), _110(gZeroVec), _11C(0.0f, 1.0f, 0.0f), _128() {
    clearPointerVelocity();
    _90.identity();
}

void SphereSelectorHandle::init(const JMapInfoIter& rIter) {
    MR::connectToScene(this, MR::MovementType_Environment, MR::CalcAnimType_MapObj, MR::DrawBufferType_None, MR::DrawType_None);
    MR::getJMapInfoArg0NoInit(rIter, &mIsFileSelectMode);
    MR::invalidateClipping(this);
    initNerve(GET_NERVE(SphereSelectorHandle, SphereSelectorHandleNrvWait));
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
    setNerve(GET_NERVE(SphereSelectorHandle, SphereSelectorHandleNrvWait));
}

bool SphereSelectorHandle::isPointing() const {
    return MR::isStarPointerInScreen(WPAD_CHAN0);
}

bool SphereSelectorHandle::isHolding() const {
    return isNerve(GET_NERVE(SphereSelectorHandle, SphereSelectorHandleNrvHold));
}

void SphereSelectorHandle::validateRotate() {
    setNerve(GET_NERVE(SphereSelectorHandle, SphereSelectorHandleNrvWait));
}

void SphereSelectorHandle::invalidateRotate() {
    setNerve(GET_NERVE(SphereSelectorHandle, SphereSelectorHandleNrvInvalidRotate));
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
    return (isNerve(GET_NERVE(SphereSelectorHandle, SphereSelectorHandleNrvWait)) ||
            isNerve(GET_NERVE(SphereSelectorHandle, SphereSelectorHandleNrvSpin)) ||
            isNerve(GET_NERVE(SphereSelectorHandle, SphereSelectorHandleNrvDemoRotate)));
}

bool SphereSelectorHandle::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (SphereSelectorFunction::isMsgSelectStart(msg)) {
        appear();
        return true;
    }

    if (SphereSelectorFunction::isMsgSelectEnd(msg)) {
        setNerve(GET_NERVE(SphereSelectorHandle, SphereSelectorHandleNrvDisappear));
        return true;
    }

    if (SphereSelectorFunction::isMsgConfirmStart(msg)) {
        if (mIsFileSelectMode) {
            setNerve(GET_NERVE(SphereSelectorHandle, SphereSelectorHandleNrvIdleForFileSelect));
        } else {
            setNerve(GET_NERVE(SphereSelectorHandle, SphereSelectorHandleNrvGalaxyConfirmStart));
        }

        return true;
    }

    if (SphereSelectorFunction::isMsgConfirmCancel(msg)) {
        if (mIsFileSelectMode) {
            setNerve(GET_NERVE(SphereSelectorHandle, SphereSelectorHandleNrvIdleEndForFileSelect));
        } else {
            setNerve(GET_NERVE(SphereSelectorHandle, SphereSelectorHandleNrvGalaxyConfirmCancel));
        }

        return true;
    }

    if (SphereSelectorFunction::isMsgTargetSelected(msg)) {
        if (isWaitOrSpinOrDemoRotate()) {
            setNerve(GET_NERVE(SphereSelectorHandle, SphereSelectorHandleNrvHold));
            return true;
        }
    }

    return false;
}

bool SphereSelectorHandle::tryRelease() {
    if (!SphereSelectorFunction::isPadButton()) {
        setNerve(GET_NERVE(SphereSelectorHandle, SphereSelectorHandleNrvSpin));
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

    _F4[0].set(*MR::getStarPointerScreenVelocity(WPAD_CHAN0));
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
    MR::clampBoth(&mRotateSpeed, mTargetRotateSpeed - ::cRotateSpeedChangeMax, mTargetRotateSpeed + ::cRotateSpeedChangeMax);
    MR::clampBoth(&mRotateSpeed, -::cRotateSpeedMaxY, ::cRotateSpeedMaxY);
    mTargetRotateSpeed = mRotateSpeed;
    mRotation.y = MR::repeatDegree(mRotation.y + mRotateSpeed);
}

void SphereSelectorHandle::rotateAxisX() {
    MR::clampBoth(&_D4, _D8 - ::cRotateSpeedChangeMax, _D8 + ::cRotateSpeedChangeMax);
    MR::clampBoth(&_D4, -::cRotateSpeedMaxX, ::cRotateSpeedMaxX);
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

    f32 verticalRate = MR::abs(_D4) / 2.0f;
    f32 horizontalRate = MR::abs(mRotateSpeed) / 5.0f;
    f32 rate = verticalRate > horizontalRate ? verticalRate : horizontalRate;

    if (rate > 1.0f) {
        rate = 1.0f;
    }

    MR::startAtmosphereLevelSE("SE_AT_LV_ASTRO_DOME_WIND_1", static_cast< s32 >(100.0f * rate));
    if (!(MR::abs(mRotateSpeed) >= 4.0f) && !(MR::abs(_D4) >= 1.6f)) {
        return;
    }

    MR::startAtmosphereLevelSE("SE_AT_LV_ASTRO_DOME_WIND_2");
}

void SphereSelectorHandle::setStateConfirmStartAtFirstStep() NO_INLINE {
    if (MR::isFirstStep(this)) {
        resetRotateParam();
        MR::setStageBGMState(::cBgmConfirmState, ::cBgmConfirmFrames);
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
        setNerve(GET_NERVE(SphereSelectorHandle, SphereSelectorHandleNrvDemoRotate));
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
        mRotateSpeed = ::cHoldRotateRateY * getPointerVelocity()->x;
        if (mRotateSpeed * mTargetRotateSpeed < 0.0f || MR::abs(mRotateSpeed) < MR::abs(mTargetRotateSpeed)) {
            mRotateSpeed = MR::getLinerValue(::cHoldReduceRate, mRotateSpeed, mTargetRotateSpeed, 1.0f);
        }

        _D4 = ::cHoldRotateRateX * getPointerVelocity()->y;
        if (_D4 * _D8 < 0.0f || MR::abs(_D4) < MR::abs(_D8)) {
            _D4 = MR::getLinerValue(::cHoldReduceRate, _D4, _D8, 1.0f);
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
    if (MR::isStarPointerInScreen(WPAD_CHAN0)) {
        SphereSelectorFunction::registerPointingTarget(this, HandlePointingPriority(1));
    }

    if (MR::isNearZero(mRotateSpeed) && MR::isNearZero(_D4)) {
        setNerve(GET_NERVE(SphereSelectorHandle, SphereSelectorHandleNrvWait));
    }
}

void SphereSelectorHandle::exeDemoRotate() {
    mRotateSpeed = ::cDemoRotateSpeedY;
    if (isPointing()) {
        SphereSelectorFunction::registerPointingTarget(this, HandlePointingPriority(1));
    }

    if (_10C == 0) {
        setNerve(GET_NERVE(SphereSelectorHandle, SphereSelectorHandleNrvWait));
    }
}

void SphereSelectorHandle::exeDisappear() {
    if (MR::isFirstStep(this)) {
        resetRotateParam();
        MR::setStageBGMState(::cBgmDisappearState, ::cBgmDisappearFrames);
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

    MR::setNerveAtStep(this, GET_NERVE(SphereSelectorHandle, SphereSelectorHandleNrvGalaxyConfirmWait), confirmStartCancelFrame);
}

void SphereSelectorHandle::exeGalaxyConfirmCancel() {
    s32 confirmStartCancelFrame = SphereSelectorFunction::getConfirmStartCancelFrame();
    if (MR::isFirstStep(this)) {
        MR::setStageBGMState(::cBgmNotConfirmState, ::cBgmNotConfirmFrames);
    }

    mPosition.lerp(_110, TVec3f(0.0f, 0.0f, 0.0f), MR::calcNerveEaseInRate(this, confirmStartCancelFrame));
    MR::setNerveAtStep(this, GET_NERVE(SphereSelectorHandle, SphereSelectorHandleNrvWait), confirmStartCancelFrame);
}

void SphereSelectorHandle::exeIdleEndForFileSelect() NO_INLINE {
    if (MR::isFirstStep(this)) {
        MR::setStageBGMState(::cBgmNotConfirmState, ::cBgmNotConfirmFrames);
    }

    MR::setNerveAtStep(this, GET_NERVE(SphereSelectorHandle, SphereSelectorHandleNrvWait), SphereSelectorFunction::getConfirmStartCancelFrame());
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
