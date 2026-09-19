#include "Game/MapObj/SuperSpinDriver.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/SpinDriverCamera.hpp"
#include "Game/MapObj/SpinDriverOperateRing.hpp"
#include "Game/MapObj/SpinDriverPathDrawer.hpp"
#include "Game/MapObj/SpinDriverShootPath.hpp"
#include "Game/MapObj/SpinDriverUtil.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/ActorShadowUtil.hpp"
#include "Game/Util/ActorSwitchUtil.hpp"
#include "Game/Util/AreaObjUtil.hpp"
#include "Game/Util/DemoUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/EventUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/GravityUtil.hpp"
#include "Game/Util/JMapUtil.hpp"
#include "Game/Util/JointUtil.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>
#include <JSystem/JGeometry/TVec.hpp>
#include "math_types.hpp"
#include <revolution/wpad.h>

/* it seems like this file was compiled with an earlier compiler version */

namespace {
    static f32 sCanBindTime = 90.0f;
};  // namespace

namespace NrvSuperSpinDriver {
    NEW_NERVE(SuperSpinDriverNrvTryDemo, SuperSpinDriver, TryDemo);
    NEW_NERVE(SuperSpinDriverNrvEmptyNonActive, SuperSpinDriver, EmptyNonActive);
    NEW_NERVE(SuperSpinDriverNrvEmptyAppear, SuperSpinDriver, EmptyAppear);
    NEW_NERVE(SuperSpinDriverNrvEmptyWait, SuperSpinDriver, EmptyWait);
    NEW_NERVE(SuperSpinDriverNrvNonActive, SuperSpinDriver, NonActive);
    NEW_NERVE(SuperSpinDriverNrvAppear, SuperSpinDriver, Appear);
    NEW_NERVE(SuperSpinDriverNrvWait, SuperSpinDriver, Wait);
    NEW_NERVE(SuperSpinDriverNrvCapture, SuperSpinDriver, Capture);
    NEW_NERVE(SuperSpinDriverNrvShootStart, SuperSpinDriver, ShootStart);
    NEW_NERVE_ONEND(SuperSpinDriverNrvShoot, SuperSpinDriver, Shoot, Shoot);
    NEW_NERVE(SuperSpinDriverNrvCoolDown, SuperSpinDriver, CoolDown);
};  // namespace NrvSuperSpinDriver

// Fix JGeometry inlining
void DUMMY() {
    TVec3f a, b, c;
    c *= 1.0f;
    a += b;
    a = b - c;

    TPos3f d;
    d.setEulerY(1.0f);

    TQuat4f e;
    d.makeQuat(e);
}

SuperSpinDriver::SuperSpinDriver(const char* pName, s32 color)
    : LiveActor(pName), mBindActor(), mShootPath(), mSpinDriverCamera(), mOperateRing(), mPathDrawer(), mEmptyModel(), _A4(0, 0, 0, 1),
      _B4(0, 0, 0, 1), _C4(0, 0, 0), _D0(0, 0, 0), mShootPathDirection(0, 1, 0), _E8(0, 0, 1), _F4(1, 0, 0), _100(0, 1, 0),
      mShootPathPosition(0, 0, 0), _118(0, 0, 0), _124(0, 0, 0), _134(), _138(), _13C(), mFrontAngle(), _144(), _148(0), mShadowLength(-1.0f),
      mFlightTime(300), _154(50), _158(230), _15C(280), mDrawPathRangeIdx(-1), mPlayerLandRotation(), _168(), mAlreadyDoneFlagIdx(-1), mColor(color),
      _174(true), _178(), _17C(), _17D(), mIsPullPlayer(true), mIsDisableJingle() {
}

void SuperSpinDriver::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    initModelManagerWithAnm("SuperSpinDriver", nullptr, false);
    MR::connectToSceneNoSilhouettedMapObjStrongLight(this);

    initParamFromJMapInfo(rIter);
    initGravityAxis();
    initSensor();
    initEffectKeeper(0, nullptr, false);
    initSound(6, false);
    initEventCamera(rIter);
    initNerve(GET_NERVE(SuperSpinDriver, SuperSpinDriverNrvWait));

    MR::initShadowVolumeFlatModel(this, "SuperSpinDriverShadow", MR::getJointMtx(this, "Outside"));
    initOperateRing();
    initShootPath(rIter);
    initShootPathDraw(rIter);
    MR::setClippingFar200m(this);
    initColor();
    initEmptyModel();

    if (MR::useStageSwitchReadA(this, rIter)) {
        MR::listenStageSwitchOnA(this, MR::Functor(this, &SuperSpinDriver::requestActive));

        if (isRightToUse()) {
            setNerve(GET_NERVE(SuperSpinDriver, SuperSpinDriverNrvNonActive));
        } else {
            setNerve(GET_NERVE(SuperSpinDriver, SuperSpinDriverNrvEmptyNonActive));
        }
    }

    if (MR::useStageSwitchReadB(this, rIter)) {
        MR::listenStageSwitchOnOffB(this, MR::Functor(this, &SuperSpinDriver::requestShow), MR::Functor(this, &SuperSpinDriver::requestHide));
    }

    MR::useStageSwitchSleep(this, rIter);
    initAppearState(rIter);
}

void SuperSpinDriver::initAfterPlacement() {
    SpinDriverUtil::setShadowAndClipping(this, &_124, mShadowLength, 300.0f, &_130);

    if (mEmptyModel != nullptr) {
        SpinDriverUtil::setShadowAndClipping(mEmptyModel, &_124, mShadowLength, 300.0f, &_130);
    }
}

void SuperSpinDriver::initParamFromJMapInfo(const JMapInfoIter& rIter) {
    MR::getJMapInfoArg0NoInit(rIter, &mFlightTime);

    mShadowLength = -1.0f;
    MR::getJMapInfoArg1NoInit(rIter, &mShadowLength);

    s32 isPullPlayer = 0;
    if (MR::getJMapInfoArg2NoInit(rIter, &isPullPlayer)) {
        mIsPullPlayer = isPullPlayer != 0;
    }

    s32 isDisableJingle = 0;
    if (MR::getJMapInfoArg4NoInit(rIter, &isDisableJingle)) {
        mIsDisableJingle = isDisableJingle == 1;
    }
}

void SuperSpinDriver::initGravityAxis() {
    MR::offCalcGravity(this);

    TVec3f gravity;
    MR::calcGravityVectorOrZero(this, &gravity, nullptr, 0);

    if (MR::isNearZero(gravity)) {
        gravity.set< f32 >(0.0f, -1.0f, 0.0f);
    }

    mGravity.set(gravity);
}

void SuperSpinDriver::initSensor() {
    initHitSensor(2);
    MR::addHitSensor(this, "bind", ATYPE_SPIN_CANNON_BIND, 16, 300.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensorPosRide(this, "body", 16, 300.0f, &_C4, TVec3f(0.0f, 0.0f, 0.0f));
    MR::invalidateHitSensor(this, "body");
}

void SuperSpinDriver::initEmptyModel() {
    if (!isNeedEmptyModel()) {
        return;
    }

    mEmptyModel = MR::createModelObjMapObjStrongLight("無効モデル", "SuperSpinDriverEmpty", getBaseMtx());
    MR::startBtp(mEmptyModel, "SuperSpinDriverEmpty");
    MR::setBtpFrameAndStop(mEmptyModel, mColor);
    MR::initShadowVolumeFlatModel(mEmptyModel, "SuperSpinDriverShadow", MR::getJointMtx(this, "Outside"));
    mEmptyModel->mGravity.set(mGravity);

    if (isRightToUse()) {
        onUse();
    } else {
        offUse();
        setNerve(GET_NERVE(SuperSpinDriver, SuperSpinDriverNrvEmptyWait));
    }
}

void SuperSpinDriver::initEventCamera(const JMapInfoIter& rIter) {
    mSpinDriverCamera = new SpinDriverCamera();
    mSpinDriverCamera->init(rIter, this);
}

void SuperSpinDriver::initShootPath(const JMapInfoIter& rIter) {
    mPlayerLandRotation = 0.0f;
    MR::getJMapInfoArg5NoInit(rIter, &mPlayerLandRotation);

    mShootPath = new SpinDriverShootPath();
    mShootPath->initUsingParabolic(rIter, mPosition);
    mShootPath->calcInitPose(&_100, &_E8, &_F4, mPlayerLandRotation);
}

void SuperSpinDriver::initShootPathDraw(const JMapInfoIter& rIter) {
    s32 drawPathRangeIdx = -1;
    f32 coord = 0.0f;

    MR::getJMapInfoArg7NoInit(rIter, &drawPathRangeIdx);
    mDrawPathRangeIdx = MR::registerStorageSpinDriverPathDrawRange(this, rIter, drawPathRangeIdx, &coord);

    mPathDrawer = new SpinDriverPathDrawer(mShootPath);
    mPathDrawer->setCoord(coord);
    mPathDrawer->initWithoutIter();
}

void SuperSpinDriver::initColor() {
    MR::startBtp(this, "SuperSpinDriver");
    MR::setBtpFrameAndStop(this, mColor);

    switch (mColor) {
    case 0:
        MR::startBrk(this, "Yellow");
        mPathDrawer->setColorNormal();
        break;
    case 1:
        MR::startBrk(this, "Green");
        mPathDrawer->setColorGreen();
        break;
    case 2:
        MR::startBrk(this, "Pink");
        mPathDrawer->setColorPink();
        break;
    }
}

void SuperSpinDriver::initOperateRing() {
    mOperateRing = new SpinDriverOperateRing("操作リング");
    mOperateRing->initWithoutIter();
}

void SuperSpinDriver::initAppearState(const JMapInfoIter& rIter) {
    bool isAppear = true;

    if (MR::useStageSwitchReadAppear(this, rIter)) {
        MR::syncStageSwitchAppear(this);
        mSpinDriverCamera->initAppearCamera(rIter, this);
        isAppear = false;
    }

    if (MR::tryRegisterDemoCast(this, rIter) && MR::isRegisteredDemoActionAppear(this)) {
        isAppear = false;
    }

    if (MR::joinToGroupArray(this, rIter, "惑星出現", 32)) {
        mSpinDriverCamera->initAppearCamera(rIter, this);
        isAppear = false;
    }

    if (isAppear) {
        makeActorAppeared();
        return;
    }

    s32 respawnType = 0;
    MR::getJMapInfoArg6NoInit(rIter, &respawnType);

    if (respawnType == 1) {
        _168 = 0;
        mAlreadyDoneFlagIdx = MR::setupAlreadyDoneFlag("ドライバ出現済み", rIter, &_168);
    } else {
        _168 = 0;
        mAlreadyDoneFlagIdx = -1;
    }

    if (_168 != 0) {
        makeActorAppeared();
    } else {
        makeActorDead();
    }
}

void SuperSpinDriver::makeActorAppeared() {
    LiveActor::makeActorAppeared();

    if (mPathDrawer != nullptr && mPathDrawer->_B0 > 0.0f) {
        startPathDraw();
    }
}

void SuperSpinDriver::appear() {
    if (!MR::isDead(this)) {
        return;
    }

    LiveActor::appear();

    if (isRightToUse()) {
        onUse();
        requestAppear();
    } else {
        offUse();
        requestEmptyAppear();
    }

    if (mAlreadyDoneFlagIdx >= 0) {
        MR::updateAlreadyDoneFlag(mAlreadyDoneFlagIdx, 1);
    }
}

void SuperSpinDriver::control() {
    if (!_174 && isFarPlayer()) {
        _174 = true;
    }

    if (_178 > 0) {
        _178--;
    }

    _17C = _178 == 1;

    mFrontAngle += _144;
    mFrontAngle = MR::repeat(mFrontAngle, -PI, TWO_PI);
    _144 *= 0.985f;
}

void SuperSpinDriver::calcAndSetBaseMtx() {
    TPos3f mtx;
    MR::makeMtxUpFrontPos(&mtx, _100, _E8, mPosition);

    TPos3f mtxRotate;
    mtxRotate.identity();
    mtxRotate.setEulerY(mFrontAngle);

    mtx.concat(mtx, mtxRotate);
    MR::setBaseTRMtx(this, mtx);
}

void SuperSpinDriver::startClipped() {
    LiveActor::startClipped();
    MR::deleteEffectAll(this);
}

void SuperSpinDriver::endClipped() {
    LiveActor::endClipped();
    endPathDraw();
}

void SuperSpinDriver::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (mBindActor != nullptr && !MR::isSensorAutoRush(pSender) && MR::tryGetItem(pSender, pReceiver)) {
        return;
    }
}

bool SuperSpinDriver::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (msg == ACTMES_IS_RUSH_TAKEOVER) {
        _178 = 60;
        return canBind(pSender, pReceiver);
    }

    if (msg == ACTMES_AUTORUSH_BEGIN) {
        _178 = 60;

        if (tryBind(pSender, pReceiver)) {
            return true;
        }
    }

    if (msg == ACTMES_RUSH_CANCEL) {
        if (isNerve(GET_NERVE(SuperSpinDriver, SuperSpinDriverNrvShootStart)) ||
            isNerve(GET_NERVE(SuperSpinDriver, SuperSpinDriverNrvShoot)) && MR::isLessStep(this, 40)) {
            return false;
        }

        if (mBindActor != nullptr) {
            mBindActor = nullptr;
            mSpinDriverCamera->cancel();
            return true;
        }
    }

    if (msg == ACTMES_RUSH_FORCE_CANCEL && mBindActor != nullptr) {
        mBindActor = nullptr;
        mSpinDriverCamera->cancel();
        return true;
    }

    if (msg == ACTMES_UPDATE_BASEMTX && mBindActor != nullptr) {
        updateBindActorMatrix();
        return true;
    }

    return false;
}

bool SuperSpinDriver::canBind(HitSensor* pSender, HitSensor* pReceiver) const {
    if (!isNerveEnableBind()) {
        return false;
    }

    if (isSwingOr2PTrigger()) {
        return true;
    }

    if (mIsPullPlayer && _174 && MR::isNear(pSender, pReceiver, 240.0f)) {
        return true;
    }

    return false;
}

bool SuperSpinDriver::tryEndCapture() {
    if (MR::isGreaterStep(this, 60) && _C4.distance(mPosition) < 15.0f) {
        cancelBind();
        _174 = false;
        setNerve(GET_NERVE(SuperSpinDriver, SuperSpinDriverNrvWait));
        return true;
    }

    return false;
}

bool SuperSpinDriver::tryForceCancel() {
    if (mBindActor == nullptr) {
        setNerve(GET_NERVE(SuperSpinDriver, SuperSpinDriverNrvCoolDown));
        return true;
    }

    return false;
}

bool SuperSpinDriver::tryShootStart() {
    bool isSwingOrPointed = MR::isPadSwing(WPAD_CHAN0) || MR::isPlayerPointedBy2POnTriggerButton();

    if (isSwingOrPointed) {
        MR::deleteEffect(this, "SuperSpinDriverLight");
        setNerve(GET_NERVE(SuperSpinDriver, SuperSpinDriverNrvShootStart));
        return true;
    }

    return false;
}

bool SuperSpinDriver::tryShoot() {
    if (MR::isGreaterStep(this, 45)) {
        setNerve(GET_NERVE(SuperSpinDriver, SuperSpinDriverNrvShoot));
        return true;
    }

    return false;
}

bool SuperSpinDriver::tryEndShoot() {
    if (MR::isGreaterEqualStep(this, mFlightTime)) {
        endBind();
        setNerve(GET_NERVE(SuperSpinDriver, SuperSpinDriverNrvCoolDown));
        return true;
    }

    return false;
}

bool SuperSpinDriver::tryEndCoolDown() {
    if (MR::isGreaterStep(this, 60) && _178 == 0) {
        setNerve(GET_NERVE(SuperSpinDriver, SuperSpinDriverNrvWait));
        return true;
    }

    return false;
}

bool SuperSpinDriver::trySwitchOff() {
    if (MR::isValidSwitchAppear(this) && !MR::isOnSwitchAppear(this)) {
        kill();
        return true;
    }

    return false;
}

void SuperSpinDriver::requestAppear() {
    MR::invalidateClipping(this);

    if (mSpinDriverCamera->isUseAppearCamera(this)) {
        MR::requestStartDemo(this, "出現", GET_NERVE(SuperSpinDriver, SuperSpinDriverNrvAppear),
                             GET_NERVE(SuperSpinDriver, SuperSpinDriverNrvTryDemo));
    } else {
        setNerve(GET_NERVE(SuperSpinDriver, SuperSpinDriverNrvAppear));
    }
}

void SuperSpinDriver::requestEmptyAppear() {
    MR::invalidateClipping(this);

    if (mSpinDriverCamera->isUseAppearCamera(this)) {
        MR::requestStartDemo(this, "出現", GET_NERVE(SuperSpinDriver, SuperSpinDriverNrvEmptyAppear),
                             GET_NERVE(SuperSpinDriver, SuperSpinDriverNrvTryDemo));
    } else {
        setNerve(GET_NERVE(SuperSpinDriver, SuperSpinDriverNrvEmptyAppear));
    }
}

void SuperSpinDriver::requestActive() {
    if (isNerve(GET_NERVE(SuperSpinDriver, SuperSpinDriverNrvNonActive))) {
        requestAppear();
    } else if (isNerve(GET_NERVE(SuperSpinDriver, SuperSpinDriverNrvEmptyNonActive))) {
        requestEmptyAppear();
    }
}

void SuperSpinDriver::requestHide() {
    if (!MR::isDead(this)) {
        if (mBindActor != nullptr) {
            endBind();
        }

        makeActorDead();
    }
}

void SuperSpinDriver::requestShow() {
    if (MR::isDead(this)) {
        makeActorAppeared();
    }
}

void SuperSpinDriver::exeTryDemo() {
}

void SuperSpinDriver::exeEmptyNonActive() {
    if (MR::isFirstStep(this)) {
        MR::validateClipping(this);
    }

    if (isRightToUse()) {
        onUse();
        setNerve(GET_NERVE(SuperSpinDriver, SuperSpinDriverNrvNonActive));
    }
}

void SuperSpinDriver::exeEmptyAppear() {
    if (MR::isFirstStep(this)) {
        mSpinDriverCamera->startAppearCamera(this, _100, _E8, mPosition);

        if (!mIsDisableJingle) {
            MR::startSystemSE("SE_SY_SPIN_DRIVER_APPEAR");
            MR::startSound(this, "SE_OJ_S_SPIN_DRV_APPEAR");
        }
    }

    if (MR::isBckStopped(this)) {
        s32 frames = mSpinDriverCamera->getAppearCameraFrames();

        if (MR::isGreaterStep(this, frames)) {
            setNerve(GET_NERVE(SuperSpinDriver, SuperSpinDriverNrvEmptyWait));

            if (mSpinDriverCamera->isUseAppearCamera(this)) {
                mSpinDriverCamera->endAppearCamera(this);
                MR::endDemoWaitCameraInterpolating(this, "出現");
            }
        }
    }
}

void SuperSpinDriver::exeEmptyWait() {
    if (MR::isFirstStep(this)) {
        MR::validateClipping(this);
    }

    if (isRightToUse()) {
        onUse();
        setNerve(GET_NERVE(SuperSpinDriver, SuperSpinDriverNrvWait));
    }
}

void SuperSpinDriver::exeNonActive() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "NonActive", nullptr);
        MR::validateClipping(this);
    }

    addSwingSignRotateY();
}

void SuperSpinDriver::exeAppear() {
    if (MR::isFirstStep(this)) {
        mSpinDriverCamera->startAppearCamera(this, _100, _E8, mPosition);

        if (!mIsDisableJingle) {
            MR::startSystemSE("SE_SY_SPIN_DRIVER_APPEAR");
            MR::startSound(this, "SE_OJ_S_SPIN_DRV_APPEAR");
        }

        MR::startBck(this, "Appear", nullptr);
        _144 = 0.0f;
    }

    if (MR::isBckStopped(this)) {
        s32 frames = mSpinDriverCamera->getAppearCameraFrames();

        if (MR::isGreaterStep(this, frames)) {
            setNerve(GET_NERVE(SuperSpinDriver, SuperSpinDriverNrvWait));

            if (mSpinDriverCamera->isUseAppearCamera(this)) {
                mSpinDriverCamera->endAppearCamera(this);
                MR::endDemoWaitCameraInterpolating(this, "出現");
            }
        }
    }
}

void SuperSpinDriver::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::startBck(this, "Wait", nullptr);
        MR::validateClipping(this);
    }

    if (MR::isGreaterStep(this, ::sCanBindTime)) {
        addSwingSignRotateY();
    }

    if (_178 > 0) {
        MR::startLevelSound(this, "SE_OJ_LV_S_SPIN_DRV_SHINE");

        if (!_17C) {
            MR::emitEffect(this, "SuperSpinDriverLight");
            MR::startCSSound("CS_SPIN_BIND", nullptr, 0);
        }

    } else {
        MR::deleteEffect(this, "SuperSpinDriverLight");
    }

    trySwitchOff();
}

void SuperSpinDriver::exeCapture() {
    if (tryForceCancel()) {
        MR::deleteEffect(this, "SuperSpinDriverLight");
    } else {
        if (MR::isFirstStep(this)) {
            MR::emitEffect(this, "SuperSpinDriverLight");
            MR::startBckPlayer("SpinDriverWait", "SuperSpinDriverCapture");
            _144 = 0.0f;
        }

        MR::startLevelSound(this, "SE_OJ_LV_S_SPIN_DRV_SHINE");
        MR::startLevelSound(this, "SE_OJ_LV_SPIN_DRV_CAPTURE");
        moveBindPosToCenter();
        f32 rate = MR::calcNerveRate(this, 60);
        _134 = rate;
        updateBindActorPoseToShoot((f64)rate);
        _144 += 0.0040f;
        MR::tryRumblePadWeak(this, WPAD_CHAN0);
        _178 = 60;

        if (!tryEndCapture()) {
            if (!tryShootStart()) {
                return;
            }
        }
    }
}

void SuperSpinDriver::endShoot() {
    MR::invalidateHitSensor(this, "body");
    mOperateRing->reset();
}

void SuperSpinDriver::exeCoolDown() {
    // BUG, is supposed to be a conditional to call tryEndCoolDown
    if (MR::isFirstStep(this)) {
    }

    if (!tryEndCoolDown()) {
        trySwitchOff();
    }
}

void SuperSpinDriver::exeShootStart() {
    if (tryForceCancel()) {
        return;
    }

    if (MR::isFirstStep(this)) {
        MR::startSound(mBindActor, "SE_PM_SPIN_ATTACK");
        MR::startCSSound("CS_SPIN_DRIVE_LONG", "SE_SY_CS_S_SPIN_DRV_START", 0);
        MR::startSound(this, "SE_OJ_S_SPIN_DRV_PREP_JUMP");

        if (MR::isInAreaObj("Water", mPosition)) {
            MR::startSound(this, "SE_PM_SPIN_DRV_IN_WATER_1");
        }

        MR::deleteEffectAll(this);
        MR::emitEffect(this, "SuperSpinDriverStart");
        MR::startBck(this, "Start", nullptr);
        MR::startBckPlayer("SuperSpinDriverStart", "SuperSpinDriverShoot");
        _118 = _C4;
        updateBindActorPoseToShoot(1.0f);
    }

    f32 rate = MR::calcNerveRate(this, 15);

    _C4 = mPosition * rate + _118 * (1.0f - rate);
    _144 *= 0.8f;

    MR::tryRumblePadMiddle(this, WPAD_CHAN0);

    if (tryShoot()) {
        return;
    }
}

void SuperSpinDriver::exeShoot() {
    if (tryForceCancel()) {
        return;
    }

    if (MR::isFirstStep(this)) {
        calcShootMotionTime();
        MR::validateHitSensor(this, "body");

        if (MR::hasME()) {
            MR::startSystemME("ME_MAGIC_L");
        } else {
            MR::startSystemSE("SE_SY_S_SPIN_DRV_ME_ALT");
        }

        MR::startSound(this, "SE_OJ_S_SPIN_DRV_JUMP");
        MR::startSound(mBindActor, "SE_PV_JUMP_JOY");

        if (MR::isInAreaObj("Water", mPosition)) {
            MR::startSound(this, "SE_PM_SPIN_DIV_IN_WATER_2");
        }

        MR::startBckPlayer("SpaceFlyStart", "SuperSpinDriverFlyStart");
        MR::shakeCameraStrong();

        MR::tryRumblePadVeryStrong(this, WPAD_CHAN0);
        mShootPathDirection.set(_100);
        mOperateRing->reset();
        startPathDraw();
    }

    f32 rate = MR::calcNerveRate(this, mFlightTime);
    updatePathDraw(rate);
    updateOperateRate();
    updateBindPosition(rate);

    if (!MR::isNearZero(mShootPathDirection)) {
        TVec3f stack_2C;
        TVec3f stack_20 = mShootPathDirection + (mOperateRing->mDirection * mOperateRing->mRadiusRate) * 0.8f;
        MR::normalize(stack_20, &stack_2C);
        turnBindHead(stack_2C, 0.4f);
    }

    if (_154 <= getNerveStep() && getNerveStep() <= _158) {
        f32 v = MR::normalize(getNerveStep(), _154, _158);
        _138 = _13C * MR::getEaseOutValue(v, 0.0f, 1.0f, 1.0f);
    }

    _148 = PI * MR::normalize(getNerveStep(), _158, _15C);
    updateShootMotion();
    mSpinDriverCamera->update(mShootPathDirection, mShootPathPosition);

    if (tryEndShoot()) {
        return;
    }
}

void SuperSpinDriver::updateShootMotion() {
    if (MR::isStep(this, _154)) {
        MR::startBckPlayer("SpaceFlyLoop", "SuperSpinDriverFlyLoop");
    }

    if (MR::isLessStep(this, _158)) {
        MR::startLevelSound(mBindActor, "SE_PM_LV_S_SPIN_DRV_FLY");
    }

    if (MR::isStep(this, _158)) {
        MR::startBckPlayer("SpaceFlyEnd", "SuperSpinDriverFlyEnd");
        MR::startSound(mBindActor, "SE_PM_S_SPIN_DRV_COOL_DOWN");
        MR::startSound(mBindActor, "SE_PV_JUMP_S");
    }

    if (MR::isStep(this, _15C)) {
        MR::startBckPlayer("Fall", "SuperSpinDriverFall");
    }
}

void SuperSpinDriver::cancelBind() {
    if (mBindActor != nullptr) {
        MR::endBindAndPlayerJump(this, _D0, 0);
        mBindActor = nullptr;
    }

    mSpinDriverCamera->cancel();
}

void SuperSpinDriver::endBind() {
    MR::endBindAndSpinDriverJump(this, _D0);
    mBindActor = nullptr;
    mSpinDriverCamera->end();
}

void SuperSpinDriver::updateBindActorPoseToShoot(f32 a1) {
    TPos3f stack_18;
    stack_18.identity();

    MR::makeMtxUpFront(&stack_18, _100, _E8);

    TQuat4f stack_8;
    stack_18.makeQuat(stack_8);

    _B4 = _A4;
    _B4.slerp(stack_8, a1);
}

void SuperSpinDriver::startPathDraw() {
    if (mPathDrawer != nullptr) {
        if (MR::isDead(mPathDrawer)) {
            mPathDrawer->appear();
            MR::emitEffect(this, "EndGlow");
        }
    }
}

void SuperSpinDriver::endPathDraw() {
    if (mPathDrawer != nullptr) {
        if (!MR::isDead(mPathDrawer)) {
            MR::emitEffect(this, "EndGlow");
        }
    }
}

void SuperSpinDriver::updatePathDraw(f32 coord) {
    if (mPathDrawer == nullptr) {
        return;
    }

    mPathDrawer->setCoord(coord);

    if (mDrawPathRangeIdx >= 0) {
        MR::updateStorageSpinDriverPathDrawRange(mDrawPathRangeIdx, mPathDrawer->_B0);
    }
}

void SuperSpinDriver::updateOperateRate() {
    f32 rate = 0.0f;

    if (_17D && _154 > 0 && _158 > 0 && _15C > 0) {
        if (MR::isLessStep(this, _154)) {
            rate = 0.0f;
        } else if (MR::isLessStep(this, _158)) {
            s32 min = _154;
            rate = MR::normalize(getNerveStep(), _154, _154 + MR::min(5, _15C - _158));
        } else if (MR::isLessStep(this, _15C)) {
            rate = 1.0f - MR::normalize(getNerveStep(), _158, _15C);
        }
    }

    mOperateRing->setRadiusRate(rate);
}

void SuperSpinDriver::updateBindPosition(f32 coord) {
    mShootPath->calcPosition(&mShootPathPosition, coord);

    TVec3f direction;
    mShootPath->calcDirection(&direction, coord, 0.0099999998f);

    if (!MR::isNearZero(direction)) {
        mShootPathDirection = direction;
    }

    mOperateRing->update(mShootPathPosition, mShootPathDirection);
    TVec3f stack_20 = _C4;
    TVec3f stack_14 = mShootPathPosition + mOperateRing->_A4;
    _C4.set(stack_14);
    _D0 = _C4 - stack_20;
}

/*
void SuperSpinDriver::calcShootMotionTime() {
    if (_150 >= 20) {
        if (_150 >= 70) {
            _158 = _150 - 70;
            _15C = _150 - 20;

            f32 v2 = 0.2f * _150;

            if (v2 > 90) {
                v2 = 90;
            }

            _154 = 10 * (v2 / 10);
        }
    }
}
*/

void SuperSpinDriver::addSwingSignRotateY() {
    if (isSwingOr2PTrigger()) {
        _144 += 0.1f;
        if (_144 > 0.23f) {
            _144 = 0.23f;
        }
    }
}

void SuperSpinDriver::onUse() {
    if (mEmptyModel != nullptr) {
        mEmptyModel->kill();
    }

    MR::showModel(this);
}

void SuperSpinDriver::offUse() {
    if (mEmptyModel != nullptr) {
        mEmptyModel->appear();
    }

    MR::hideModelAndOnCalcAnim(this);
}

bool SuperSpinDriver::isNeedEmptyModel() const {
    switch (mColor) {
    case 1:
        return true;
    default:
        return false;
    }
}

bool SuperSpinDriver::isRightToUse() const {
    switch (mColor) {
    case 1:
        return MR::isOnGameEventFlagGreenDriver();
    default:
        return true;
    }
}

bool SuperSpinDriver::isNerveEnableBind() const {
    return isNerve(GET_NERVE(SuperSpinDriver, SuperSpinDriverNrvWait)) && MR::isGreaterStep(this, ::sCanBindTime);
}

bool SuperSpinDriver::isSwingOr2PTrigger() const {
    return MR::isPadSwing(WPAD_CHAN0) || MR::isPlayerPointedBy2POnTriggerButton();
}

bool SuperSpinDriver::isFarPlayer() const {
    return !MR::isNearPlayerAnyTime(this, 350.0f);
}

namespace MR {
    NameObj* createSuperSpinDriverYellow(const char* pName) {
        return new SuperSpinDriver(pName, 0);
    }

    NameObj* createSuperSpinDriverGreen(const char* pName) {
        return new SuperSpinDriver(pName, 1);
    }

    NameObj* createSuperSpinDriverPink(const char* pName) {
        return new SuperSpinDriver(pName, 2);
    }
};  // namespace MR

SuperSpinDriver::~SuperSpinDriver() {
}
