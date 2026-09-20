#include "Game/MapObj/SpinDriver.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/SpinDriverUtil.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/AreaObjUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/GravityUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/ModelUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/NerveUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include <cmath>

namespace {
    static const f32 sShadowMaxHeight = 0.0f;
    static const f32 sShadowHeightMargin = 0.0f;
    static const f32 sRotateYCanJumpAccel = 0.0f;
    static const f32 sRotateYActiveAccel = 0.0f;
    static const f32 sRotateYSwingSignAccel = 0.0f;
    static const f32 sMaxSwingSignRotateYSpeed = 0.0f;
    static const f32 sRotateYFreq = 0.0f;
    static const f32 sMaxBindStartSpeed = 0.0f;
    static const f32 sCaptureRange = 0.0f;
    static const f32 sCaptureAgainRange = 0.0f;
    static const f32 sCaptureTime = 0.0f;
    static const f32 sCaputureEndDistance = 0.0f;
    static const f32 sCaptureToCenterAccel = 0.0f;
    static const f32 sCaptureSpeedDownRange = 0.0f;
    static const f32 sCaptureFreq = 0.0f;
    static const f32 sRotateYCaptureAccel = 0.0f;
    static const f32 sJumpReadyTime = 0.0f;
    static const f32 sCoolDownTime = 0.0f;
    static const f32 sTakeBackDistance = 0.0f;
    static const f32 sLandRotateStartRate = 0.0f;
    static const f32 sLandRotateEndRate = 0.0f;
    static const f32 sFallMotionStartRate = 0.0f;
    static const f32 sFlyMotionInterFrame = 0.0f;
    static const f32 sPreLandMotionInterFrame = 0.0f;
    static const f32 sFallMotionInterFrame = 0.0f;
    static s32 cSpaceFlyStartFrame = 10;
    static s32 cSpaceFlyEndFrame = 50;
    static const f32 sJumpWaitTime = 0.0f;
};  // namespace

namespace NrvSpinDriver {
    NEW_NERVE(SpinDriverNrvTryDemo, SpinDriver, TryDemo);
    NEW_NERVE(SpinDriverNrvNonActive, SpinDriver, NonActive);
    NEW_NERVE(SpinDriverNrvAppear, SpinDriver, Appear);
    NEW_NERVE(SpinDriverNrvWait, SpinDriver, Wait);
    NEW_NERVE(SpinDriverNrvCapture, SpinDriver, Capture);
    NEW_NERVE(SpinDriverNrvShootStart, SpinDriver, ShootStart);
    NEW_NERVE(SpinDriverNrvShoot, SpinDriver, Shoot);
    NEW_NERVE(SpinDriverNrvCoolDown, SpinDriver, CoolDown);
};  // namespace NrvSpinDriver

SpinDriver::SpinDriver(const char* pName)
    : LiveActor(pName), _8C(), mShootPath(), mSpinDriverCamera(), _98(0, 0, 0, 1), _A8(0, 0, 0, 1), _B8(0, 0, 0), _C4(0, 0, 0), _D0(0, 0, 1),
      _DC(0, 0, 0), _E8(0, 1, 0), _F4(0, 0, 0), _104(), _100(40.0f), _108(), _10C(0, 0, 0), _11C(), _120(-1.0f), _124(300), _128(-1), _12C(240),
      _130(270), _134(), _138(true), _13C(), _140(), _141(true), _142() {
}

void SpinDriver::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("SpinDriver", nullptr, false);

    MR::connectToSceneNoSilhouettedMapObjStrongLight(this);

    initShootPath(rIter);

    MR::offCalcGravity(this);

    TVec3f gravityVector;
    MR::calcGravityVectorOrZero(this, &gravityVector, nullptr, 0);
    if (MR::isNearZero(gravityVector)) {
        gravityVector.set(0.0f, -1.0f, 0.0f);
    }

    mGravity.set(gravityVector);

    initHitSensor(1);
    MR::addHitSensor(this, "body", ATYPE_SPIN_DRIVER_BIND, 16, 300.0f, TVec3f(0.0f, 0.0f, 0.0f));

    initEffectKeeper(0, nullptr, false);

    initSound(6, false);

    initEventCamera(rIter);

    initNerve(GET_NERVE(SpinDriver, SpinDriverNrvWait));

    MR::initShadowVolumeFlatModel(this, "SpinDriverShadow", MR::getJointMtx(this, "SpinDriver"));

    initParamFromJMapInfo(rIter);

    MR::setClippingFar200m(this);

    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);

        makeActorDead();

        mSpinDriverCamera->initAppearCamera(rIter, this);
    } else {
        makeActorAppeared();
    }

    MR::useStageSwitchSleep(this, rIter);
}

void SpinDriver::initAfterPlacement() {
    SpinDriverUtil::setShadowAndClipping(this, &_10C, _120, 300.0f, &_118);
}

void SpinDriver::initParamFromJMapInfo(const JMapInfoIter& rIter) {
    if (!mShootPath) {
        MR::getJMapInfoArg0NoInit(rIter, &_100);
    } else {
        MR::getJMapInfoArg0NoInit(rIter, &_124);
    }

    _120 = -1.0f;
    MR::getJMapInfoArg1NoInit(rIter, &_120);

    s32 arg2 = 0;
    if (MR::getJMapInfoArg2NoInit(rIter, &arg2)) {
        _138 = arg2 != 0;
    }

    s32 arg4 = 0;
    if (MR::getJMapInfoArg4NoInit(rIter, &arg4)) {
        _142 = !(arg4 - 1);
    }
}

void SpinDriver::initShootPath(const JMapInfoIter& rIter) {
    if (MR::isConnectedWithRail(rIter)) {
        mShootPath = new SpinDriverShootPath();

        _134 = 0.0f;
        MR::getJMapInfoArg5NoInit(rIter, &_134);

        mShootPath->initUsingParabolic(rIter, mPosition);
        mShootPath->calcInitPose(&_E8, &_D0, &_DC, _134);
    } else {
        TPos3f rotateMtx;
        MR::makeMtxRotate(rotateMtx, mRotation);
        rotateMtx.getXYZDir(_DC, _E8, _D0);
    }
}

void SpinDriver::initEventCamera(const JMapInfoIter& rIter) {
    mSpinDriverCamera = new SpinDriverCamera();
    mSpinDriverCamera->init(rIter, this);

    if (MR::isConnectedWithRail(rIter)) {
    }
}

void SpinDriver::appear() {
    LiveActor::appear();

    MR::invalidateClipping(this);

    if (mSpinDriverCamera->isUseAppearCamera(this)) {
        MR::requestStartDemo(this, "出現", GET_NERVE(SpinDriver, SpinDriverNrvAppear), GET_NERVE(SpinDriver, SpinDriverNrvTryDemo));
    } else {
        setNerve(GET_NERVE(SpinDriver, SpinDriverNrvAppear));
    }
}

void SpinDriver::makeActorAppeared() {
    LiveActor::makeActorAppeared();
    setNerve(GET_NERVE(SpinDriver, SpinDriverNrvWait));
}

void SpinDriver::makeActorDead() {
    if (_8C) {
        cancelBind();
    }

    LiveActor::makeActorDead();
}

void SpinDriver::control() {
    if (!_141 && mPosition.squared(*MR::getPlayerPos()) > 122500.0f) {
        _141 = true;
    }

    if (_13C > 0) {
        _13C--;
    }

    _140 = _13C > 0;

    _104 += _108;
    _104 = MR::repeat(_104, -MR::pi(), 2 * MR::pi());

    _108 *= 0.95f;
}

void SpinDriver::calcAndSetBaseMtx() {
    TPos3f baseMtx;
    MR::makeMtxUpFrontPos(&baseMtx, _E8, _D0, mPosition);

    TPos3f yMtx;
    yMtx.identity();
    yMtx.setEulerY(_104);

    baseMtx.concat(baseMtx, yMtx);
    MR::setBaseTRMtx(this, baseMtx);
}

bool SpinDriver::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (msg == ACTMES_IS_RUSH_TAKEOVER) {
        _13C = 60;
        return canBind(pSender);
    }

    if (msg == ACTMES_AUTORUSH_BEGIN) {
        _13C = 60;

        if (startBind(pSender)) {
            return true;
        }
    }

    if (msg == ACTMES_RUSH_CANCEL) {
        if (isNerve(GET_NERVE(SpinDriver, SpinDriverNrvShootStart))) {
            return false;
        }

        if (_8C != nullptr) {
            _8C = nullptr;

            cancelCamera();

            return true;
        }
    }

    if (msg == ACTMES_RUSH_FORCE_CANCEL && _8C != nullptr) {
        _8C = nullptr;

        cancelCamera();

        return true;
    }

    if (msg == ACTMES_UPDATE_BASEMTX && _8C != nullptr) {
        calcBindActorMatrix();
        return true;
    }

    return false;
}

bool SpinDriver::tryStartShoot() {
    bool isSwingOr2P = MR::isPadSwing(WPAD_CHAN0) || MR::isPlayerPointedBy2POnTriggerButton();
    if (isSwingOr2P) {
        MR::startSound(_8C, "SE_PM_SPIN_ATTACK");

        if (MR::isInAreaObj("Water", mPosition)) {
            MR::startSound(this, "SE_PMSPIN_DRV_IN_WATER_1");
        }

        setNerve(GET_NERVE(SpinDriver, SpinDriverNrvShootStart));

        return true;
    }

    return false;
}

bool SpinDriver::tryEndCapture() {
    if (MR::isGreaterStep(this, 40) && _B8.distance(mPosition) < 15.0f) {
        cancelBind();

        _141 = 0;

        setNerve(GET_NERVE(SpinDriver, SpinDriverNrvWait));

        return true;
    }

    return false;
}

bool SpinDriver::tryShoot() {
    if (MR::isGreaterStep(this, 20)) {
        setNerve(GET_NERVE(SpinDriver, SpinDriverNrvShoot));
        return true;
    }

    return false;
}

bool SpinDriver::tryEndShoot() {
    if (!mShootPath || MR::isGreaterStep(this, _124)) {
        MR::endBindAndSpinDriverJump(this, _C4);

        _8C = 0;

        endCamera();

        setNerve(GET_NERVE(SpinDriver, SpinDriverNrvCoolDown));

        return true;
    }

    return false;
}

bool SpinDriver::tryForceCancel() {
    if (!_8C) {
        setNerve(GET_NERVE(SpinDriver, SpinDriverNrvCoolDown));
        return true;
    }

    return false;
}

bool SpinDriver::tryEndCoolDown() {
    if (MR::isGreaterStep(this, 60)) {
        setNerve(GET_NERVE(SpinDriver, SpinDriverNrvWait));
        return true;
    }

    return false;
}

bool SpinDriver::trySwitchOff() {
    if (MR::isValidSwitchAppear(this) && !MR::isOnSwitchAppear(this)) {
        kill();
        return true;
    }

    return false;
}

void SpinDriver::exeTryDemo() {
}

void SpinDriver::exeNonActive() {
    if (MR::isFirstStep(this)) {
        MR::validateClipping(this);

        MR::startBck(this, "Wait");
        MR::startBpk(this, "Wait");
    }

    bool isSwingOr2P = MR::isPadSwing(WPAD_CHAN0) || MR::isPlayerPointedBy2POnTriggerButton();
    if (isSwingOr2P) {
        _108 += 0.05f;
        if (_108 > 0.15f) {
            _108 = 0.15f;
        }
    }
}

void SpinDriver::exeAppear() {
    if (MR::isFirstStep(this)) {
        if (!_142) {
            MR::startSystemSE("SE_SY_SPIN_DRIVER_APPEAR");
            MR::startSound(this, "SE_OJ_SPIN_DRV_APPEAR");
        }

        MR::startBck(this, "Appear");
        mSpinDriverCamera->startAppearCamera(this, _E8, _D0, mPosition);
    }

    if (MR::isBckStopped(this) && MR::isGreaterStep(this, mSpinDriverCamera->getAppearCameraFrames())) {
        setNerve(GET_NERVE(SpinDriver, SpinDriverNrvWait));

        if (mSpinDriverCamera->isUseAppearCamera(this)) {
            MR::endDemoWaitCameraInterpolating(this, "出現");
            mSpinDriverCamera->endAppearCamera(this);
        }
    }
}

void SpinDriver::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::validateClipping(this);

        MR::startBck(this, "Wait");
        MR::startBpk(this, "Wait");
    }

    bool isSwingOr2P = MR::isPadSwing(WPAD_CHAN0) || MR::isPlayerPointedBy2POnTriggerButton();
    if (isSwingOr2P) {
        _108 += 0.05f;
    }

    if (_13C > 0) {
        _108 += 0.008f;

        MR::startLevelSound(this, "SE_OJ_LV_SPIN_DRV_SHINE");

        if (!_140) {
            MR::emitEffect(this, "SpinDriverLight");
            MR::startCSSound("CS_SPIN_BIND", nullptr, 0);
        }
    } else {
        MR::deleteEffect(this, "SpinDriverLight");
    }

    if (trySwitchOff()) {
        return;
    }
}

void SpinDriver::exeCapture() {
    if (tryForceCancel()) {
        MR::deleteEffect(this, "SpinDriverLight");
        return;
    }

    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("SpinDriverWait", "SpinDriverCapture");
        MR::startBck(this, "Active");

        MR::emitEffect(this, "SpinDriverLight");
    }

    MR::startLevelSound(this, "SE_OJ_LV_SPIN_DRV_SHINE");
    MR::startLevelSound(this, "SE_OJ_LV_SPIN_DRV_CAPTURE");

    moveBindPosToCenter();

    _11C = MR::calcNerveRate(this, 40);

    updateBindActorMatrix(_11C);

    _108 += 0.008f;

    MR::tryRumblePadWeak(this, WPAD_CHAN0);

    _13C = 60;

    if (!tryStartShoot() && tryEndCapture()) {
        return;
    }
}

void SpinDriver::exeShootStart() {
    if (tryForceCancel()) {
        return;
    }

    if (MR::isFirstStep(this)) {
        _F4 = _B8;

        MR::deleteEffect(this, "SpinDriverLight");
        MR::emitEffect(this, "SpinDriverStart");

        MR::startBckPlayer("Spin", "SpinDriverShoot");
        MR::startBck(this, "Active");
        MR::startBpk(this, "Active");

        startCamera();
    }

    if (MR::isStep(this, 4)) {
        MR::startCSSound("CS_SPIN_DRIVE_SHORT", "SE_SY_CS_SPIN_DRV_START", 0);
    }

    f32 progress = MR::clamp(getNerveStep() / 20.0f, 0.0f, 1.0f);
    f32 val = 2.0f * progress;
    val *= val;
    val *= val;

    TVec3f upVec;
    MR::calcUpVec(&upVec, this);

    TVec3f vec50;
    vec50.set(mPosition - upVec * 200.0f * (1.0f - val));

    f32 val2 = MR::clamp(2.0f * progress, 0.0f, 1.0f);
    _B8.set(_F4 * (1.0f - val2) + vec50 * val2);

    updateBindActorMatrix(val2 + _11C * (1.0f - val2));

    _108 += 0.04f;

    MR::tryRumblePadMiddle(this, 0);

    if (tryShoot()) {
    }
}

void SpinDriver::exeShoot() {
    if (tryForceCancel()) {
        return;
    }

    if (MR::isFirstStep(this)) {
        calcShootMotionTime();

        if (MR::hasME()) {
            MR::startSystemME("ME_MAGIC");
        } else {
            MR::startSystemSE("SE_SY_S_SPIN_DRV_ME_ALT");
        }

        MR::startSound(this, "SE_OJ_SPIN_DRV_JUMP");
        MR::startSound(_8C, "SE_PV_JUMP_JOY");

        if (MR::isInAreaObj("Water", mPosition)) {
            MR::startSound(this, "SE_PM_SIN_DIV_IN_WATER_2");
        }

        MR::startBckPlayer("SpaceFlyShortStart", "SpinDriverFlyStart");

        MR::shakeCameraNormal();
        MR::tryRumblePadVeryStrong(this, WPAD_CHAN0);
    }

    if (mShootPath != nullptr) {
        updateBindPosition();
        updateCamera();

        calcParabolicBindPose();

        if (MR::isStep(this, _128)) {
            MR::startBckPlayer("SpaceFlyShort", "SpinDriverFlyLoop");
        }

        if (MR::isStep(this, _12C)) {
            MR::startBckPlayer("SpaceFlyEnd", "SpinDriverFlyEnd");
        }

        if (MR::isLessStep(this, _130)) {
            MR::startLevelSound(_8C, "SE_PM_LV_SPIN_DRV_FLY");
        }

        if (MR::isStep(this, _130)) {
            MR::startBckPlayer("Fall", "SpinDriverFall");
        }
    } else {
        _C4 = _E8 * _100;
    }

    if (tryEndShoot()) {
        return;
    }
}

void SpinDriver::exeCoolDown() {
    if (MR::isFirstStep(this)) {
        MR::startBpk(this, "Wait");
    }

    if (!tryEndCoolDown() && trySwitchOff()) {
        return;
    }
}

bool SpinDriver::startBind(HitSensor* pSensor) {
    if (!canStartBind()) {
        return false;
    }

    bool isSwingOr2P = MR::isPadSwing(WPAD_CHAN0) || MR::isPlayerPointedBy2POnTriggerButton();

    if (isSwingOr2P) {
        if (MR::hasME()) {
            MR::startSystemME("ME_MAGIC");
        } else {
            MR::startSystemSE("SE_SY_S_SPIN_DRV_ME_ALT");
        }

        MR::startSound(pSensor->mHost, "SE_PM_SPIN_ATTACK");

        if (MR::isInAreaObj("Water", mPosition)) {
            MR::startSound(this, "SE_PM_SPIN_DRV_IN_WATER_1");
        }

        setNerve(GET_NERVE(SpinDriver, SpinDriverNrvShootStart));
    } else {
        if (_138 && _141 && mPosition.squared(pSensor->mPosition) < 57600.0f) {
            setNerve(GET_NERVE(SpinDriver, SpinDriverNrvCapture));
        } else {
            return false;
        }
    }

    _8C = pSensor->mHost;
    _B8 = _8C->mPosition;

    _C4 = *MR::getPlayerLastMove();

    f32 mag = _C4.length();
    if (mag > 40.0f) {
        _C4 *= 40.0f / mag;
    }

    _11C = 0.0f;

    TRot3f rotation;
    rotation.set(_8C->getBaseMtx());
    rotation.getQuat(_98);

    _A8 = _98;

    ::cSpaceFlyStartFrame = MR::getBckFrameMax(_8C, "SpaceFlyStart");
    ::cSpaceFlyEndFrame = MR::getBckFrameMax(_8C, "SpaceFlyEnd");

    MR::invalidateClipping(this);
    return true;
}

void SpinDriver::cancelBind() {
    if (_8C != nullptr) {
        MR::endBindAndPlayerJump(this, _C4, 0);

        _8C = nullptr;

        endCamera();
    }
}

void SpinDriver::updateBindPosition() {
    f32 progress = static_cast< f32 >(getNerveStep()) / _124;

    TVec3f oldPos(_B8);

    TVec3f position;
    mShootPath->calcPosition(&position, progress);

    _B8 = position;
    _C4 = position - oldPos;
}

void SpinDriver::moveBindPosToCenter() {
    _B8 += _C4;

    f32 mag;
    TVec3f dir(mPosition - _B8);
    MR::separateScalarAndDirection(&mag, &dir, dir);

    _C4 += dir * 1.5f * (mag / 120.0f);
    _C4.x *= 0.8f;
    _C4.y *= 0.8f;
    _C4.z *= 0.8f;
}

void SpinDriver::updateBindActorMatrix(f32 a1) {
    TPos3f makeMtx;
    MR::makeMtxUpFrontPos(&makeMtx, _E8, _D0, mPosition);

    TQuat4f quat;
    makeMtx.getQuat(quat);

    _A8.slerp(_98, quat, a1);
}

void SpinDriver::calcBindActorMatrix() {
    TPos3f baseMtx;
    baseMtx.setQuat(_A8);
    baseMtx.setTrans(_B8);
    MR::setBaseTRMtx(_8C, baseMtx);
}

// void SpinDriver::calcParabolicBindPose()

void SpinDriver::turnBindHead(const TVec3f& rVec, f32 f1) {
    TVec3f yDir;
    _A8.getYDir(yDir);

    TQuat4f q;
    q.setRotate(yDir, rVec, f1);
    _A8.mult(q);
    _A8.normalize();
}

void SpinDriver::calcShootMotionTime() {
    if (_124 < ::cSpaceFlyEndFrame + 20) {
        _128 = -1;
        _12C = 0;
        _130 = _124;
    } else {
        _12C = _124 - (::cSpaceFlyEndFrame + 20);
        _130 = _124 - 20;
        _128 = _12C / 5;
    }
}

void SpinDriver::startCamera() {
    if (mSpinDriverCamera) {
        mSpinDriverCamera->start(_E8, _D0, mPosition);
    }
}

void SpinDriver::updateCamera() {
    if (mSpinDriverCamera) {
        mSpinDriverCamera->update(_C4, _B8);
    }
}

void SpinDriver::cancelCamera() {
    if (mSpinDriverCamera) {
        mSpinDriverCamera->cancel();
    }
}

void SpinDriver::endCamera() {
    if (mSpinDriverCamera) {
        mSpinDriverCamera->end();
    }
}

bool SpinDriver::canStartBind() const {
    return isNerve(GET_NERVE(SpinDriver, SpinDriverNrvWait));
}

bool SpinDriver::canBind(HitSensor* pSensor) const {
    if (!canStartBind()) {
        return false;
    }

    bool isSwingOr2P = MR::isPadSwing(WPAD_CHAN0) || MR::isPlayerPointedBy2POnTriggerButton();
    if (isSwingOr2P) {
        return true;
    }

    if (_138 && _141 && mPosition.squared(pSensor->mPosition) < 457600.0f) {
        return true;
    }

    return false;
}
