#include "Game/MapObj/GCapture.hpp"
#include "Game/Camera/CameraTargetMtx.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/BlueStarCupsulePlanet.hpp"
#include "Game/MapObj/GCaptureRibbon.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/ActorSensorUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/EffectUtil.hpp"
#include "Game/Util/GamePadUtil.hpp"
#include "Game/Util/GravityUtil.hpp"
#include "Game/Util/JMapInfo.hpp"
#include "Game/Util/LiveActorUtil.hpp"
#include "Game/Util/MapUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/SoundUtil.hpp"
#include "Game/Util/SpringValue.hpp"
#include "Game/Util/StarPointerUtil.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "JSystem/JGeometry/TVec.hpp"
#include "math_types.hpp"
#include "revolution/types.h"
#include "revolution/wpad.h"

void GCapture_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)MR::epsilon();
    (void)0.0f;
    (void)2.0f;
    (void)1.0f;
    (void)0.5f;
    (void)0.1f;
    (void)FLOAT_MAX;
    (void)0.05f;
    (void)0.91f;
    (void)200.0f;
    (void)80.0f;
    (void)75.0f;
    (void)0.992f;
    (void)0.99f;
    (void)0.002f;
    (void)0.033333335f;
    (void)180.0f;
    (void)0.001f;
    (void)130.0f;
    (void)500.0f;
    (void)0.7f;
}

namespace {
    // sMinActiveTime
    // sMaxHoldTime
    // sHitEnableTime
    // sPointRadius
    // sBreakTime
    // sWandSpinStartScreenSpeed
    // sWandSpinMaxScreenSpeed
    // sWandSpinPower
    // sWandTouchRefrectPower
    // sWandMaxSpinSpeed
    // sRollMotionStartSpinSpeed
    // sFreeSpinXPower
    // sCaptureAccel
    // sCaptureFric
    // sCaptureTime
    // sCaptureCancelTime
    // sRecaptureTime
    // sGapFillSpeed
    // sInTouchableRangeMarginTime
    // sNormalFreq
    // sNearTargetFreq
    // sTractionPower
    // sNearParamStartRange
    // sTractionPowerUpSpeed
    // sTractionPowerDownSpeed
    // sTractLineDispTime
    // sTractSpinTime
    // sTractSpinAccel
    // sTractSpinFreq
    // sGroundReboundRate
    // sReflectAccel
    // sMinBindRadius
    // sMaxBindRadius
    // sMaxBubbleRadius
    // sBubbleGrowSpeed
    // sStartActiveEffectTime
    // sFreeSpinYPower
    // sNearTargetTractionPower
}  // namespace

namespace NrvGCapture {
    NEW_NERVE(GCaptureNrvWait, GCapture, Wait);
    NEW_NERVE_ONEND(GCaptureNrvCapture, GCapture, Capture, Capture);
    NEW_NERVE_ONEND(GCaptureNrvRecapture, GCapture, Recapture, Recapture);
    NEW_NERVE(GCaptureNrvTraction, GCapture, Traction);
    NEW_NERVE(GCaptureNrvHold, GCapture, Hold);
    NEW_NERVE(GCaptureNrvBreak, GCapture, Break);
    NEW_NERVE(GCaptureNrvCoolDown, GCapture, CoolDown);
};  // namespace NrvGCapture

GCapture::GCapture(const char* pName)
    : LiveActor(pName), _BC(0.0f, 0.0f, 0.0f, 1.0f), mEffectPos(0, 0, 0), _D8(0, 0, 0), _E4(0, 0, 0), _F0(0, 0, 0), _FC(0, 0, 1), _108(), mTarget(),
      _110(), mCaptureRibbon(), mTractPower(), _124(), _128(0.1f), _12C(), _130(FLOAT_MAX), mGuidanceTime(), mIsStarPointerPointing(), _139() {
    mCaptureRibbon = new GCaptureRibbon("Gキャプチャーリボン");
    mTargetMtx = new CameraTargetMtx("カメラターゲットダミー");
    mSpringValue = new SpringValue(1.0f, 1.0f, 0.05f, 0.91f, 0.0f);
    _8C.identity();
}

void GCapture::init(const JMapInfoIter& rIter) {
    MR::initDefaultPos(this, rIter);
    MR::connectToScene(this, 34, 5, -1, 19);
    initSound(8, false);
    initHitSensor(2);
    MR::addHitSensorRide(this, "body", 16, 200.0f, TVec3f(0.0f, 0.0f, 0.0f));
    MR::addHitSensor(this, "bind", 104, 16, 80.0f, TVec3f(0.0f, 0.0f, 0.0f));
    getSensor("bind")->invalidate();
    initBinder(75.0f, 0.0f, 8);
    initEffectKeeper(0, "GCapture", false);
    MR::setEffectHostSRT(this, "LightGrow", &mEffectPos, nullptr, nullptr);
    MR::setEffectHostSRT(this, "Light", &mEffectPos, nullptr, nullptr);
    MR::setEffectHostSRT(this, "LightBreak", &mEffectPos, nullptr, nullptr);
    MR::setEffectHostMtx(this, "LightSplash", _8C.mMtx);
    MR::setEffectHostMtx(this, "RibbonPoint", _8C.mMtx);
    MR::setEffectHostMtx(this, "RibbonBreak", _8C.mMtx);
    mCaptureRibbon->initWithoutIter();
    initNerve(&NrvGCapture::GCaptureNrvWait::sInstance);
    MR::initStarPointerTarget(this, 200.0f, TVec3f(0.0f, 0.0f, 0.0f));
    makeActorAppeared();
    MR::invalidateClipping(this);
}

void GCapture::draw() const {
    mCaptureRibbon->draw();
}

void GCapture::control() {
    if (mGuidanceTime > 0) {
        MR::requestBlueStarGuidance();
        mGuidanceTime--;
    }

    if (_12C > 30) {
        mTarget = nullptr;
        _12C = 0;
    }

    _139 = false;
    _130 = FLOAT_MAX;

    if (mTarget != nullptr) {
        _12C++;
    }

    if (_108 != nullptr) {
        if (0.0f == _124) {
            mEffectPos = mPosition;
        } else {
            mEffectPos = mPosition + _E4 * MR::getEaseInOutValue(_124, 0.0f, 1.0f, 1.0f);
        }

        calcBindActorPose();
        _F0.x *= 0.992f;
        _F0.y *= 0.992f;
        _F0.z *= 0.992f;
        updateCameraTargetMatrix();
    }
}

void GCapture::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (_108 != nullptr && !MR::isSensorAutoRush(pSender) && MR::tryGetItem(pSender, pReceiver)) {
        return;
    }
}

bool GCapture::receiveMsgEnemyAttack(u32 msg, HitSensor*, HitSensor*) {
    if (MR::isPlayerElementModeInvincible()) {
        return true;
    }

    return requestDamageCancel(msg);
}

bool GCapture::receiveOtherMsg(u32 msg, HitSensor* pSender, HitSensor* pReceiver) {
    if (msg == ACTMES_IS_RUSH_TAKEOVER) {
        return isNerve(&NrvGCapture::GCaptureNrvCapture::sInstance);
    } else if (msg == ACTMES_AUTORUSH_BEGIN) {
        return requestBind(pSender);
    } else if (msg == ACTMES_RUSH_CANCEL) {
        return requestCancelBind();
    }

    if (msg == ACTMES_UPDATE_BASEMTX && _108 != nullptr) {
        updateBindActorMatrix();
        return true;
    }

    return false;
}

bool GCapture::requestBind(HitSensor* pSensor) {
    if (!isNerve(&NrvGCapture::GCaptureNrvCapture::sInstance)) {
        return false;
    }

    _108 = pSensor->mHost;
    _D8 = _108->mPosition;
    mEffectPos = _D8;
    _E4 = mEffectPos - mPosition;
    _124 = 1.0f;
    MR::zeroVelocity(this);

    TPos3f pos;
    pos.set(_108->getBaseMtx());
    pos.getQuat(_BC);
    _F0.zero();
    MR::startStarPointerModeBlueStar(this);
    MR::invalidateClipping(this);
    setNerve(&NrvGCapture::GCaptureNrvTraction::sInstance);
    MR::emitEffect(this, "LightGrow");
    return true;
}

bool GCapture::requestCancelBind() {
    if (_108 != nullptr && canCancelBind()) {
        _108 = nullptr;
        setNerve(&NrvGCapture::GCaptureNrvCoolDown::sInstance);
        releaseTractTarget();
        MR::deleteEffectAll(this);
        MR::emitEffect(this, "LightBreak");
        MR::startSound(this, "SE_OJ_GCAPTURE_RELEASE");
        MR::endStarPointerMode(this);
        return true;
    }

    return false;
}

bool GCapture::requestDamageCancel(u32 msg) {
    if (_108 != nullptr && canCancelBind() && !MR::isPlayerElementModeInvincible()) {
        endBindByDamage(msg);
        setNerve(&NrvGCapture::GCaptureNrvCoolDown::sInstance);
        return true;
    }

    return false;
}

void GCapture::reset() {
    if (_108 != nullptr) {
        MR::endBindAndPlayerWeakGravityLimitJump(this, mVelocity);
        _108 = nullptr;
        MR::endStarPointerMode(this);
    }

    releaseTractTarget();
    setNerve(&NrvGCapture::GCaptureNrvWait::sInstance);
}

bool GCapture::tryCapture() {
    if (mTarget != nullptr && MR::testCorePadTriggerA(WPAD_CHAN0)) {
        decideTractTarget();
        MR::setBinderRadius(this, 75.0f);
        _110->getTargetPosition(&mPosition);
        setNerve(&NrvGCapture::GCaptureNrvCapture::sInstance);
        return true;
    }

    return false;
}

bool GCapture::tryCancelCapture() {
    bool ret = false;

    if (_110 != nullptr) {
        TVec3f position;
        _110->getTargetPosition(&position);
        if (MR::calcDistance(this, position) >= _110->getPointableRange()) {
            ret = true;
        }
    }

    if (MR::isGreaterStep(this, 40) || (!MR::testCorePadButtonA(WPAD_CHAN0) && MR::isGreaterStep(this, 30)) || ret) {
        releaseTractTarget();
        setNerve(&NrvGCapture::GCaptureNrvWait::sInstance);
        return true;
    }

    return false;
}

bool GCapture::tryEndTraction() {
    if (!MR::testCorePadButtonA(WPAD_CHAN0) && MR::isGreaterStep(this, 30)) {
        releaseTractTarget();
        setNerve(&NrvGCapture::GCaptureNrvHold::sInstance);
        return true;
    }

    return false;
}

bool GCapture::tryRecapture() {
    if (mTarget != nullptr && MR::testCorePadTriggerA(WPAD_CHAN0)) {
        decideTractTarget();
        setNerve(&NrvGCapture::GCaptureNrvRecapture::sInstance);
        return true;
    }

    return false;
}

bool GCapture::tryRetraction() {
    if (MR::isGreaterStep(this, 15)) {
        setNerve(&NrvGCapture::GCaptureNrvTraction::sInstance);
        return true;
    }

    return false;
}

bool GCapture::tryBreak() {
    s32 channel = WPAD_CHAN0;

    if (MR::isPadSwing(channel)) {
        MR::startBckPlayer("Spin2nd", "GCaptureCancel");
        channel = WPAD_CHAN1;
    } else if (MR::testSubPadTriggerZ(channel)) {
        MR::startBckPlayer("GCaptureBreak", "GCaptureBreak");
        channel = WPAD_CHAN1;
    } else if (_110 != nullptr) {
        if (_110->isReleaseForce()) {
            channel = WPAD_CHAN1;
        } else if ((_110->releaseDistance() >= 0.0f) != false) {
            TVec3f targetPos;
            _110->getTargetPosition(&targetPos);
            if (mPosition.distance(targetPos) <= _110->releaseDistance()) {
                channel = WPAD_CHAN1;
            }
        }
    }

    if (channel != WPAD_CHAN0) {
        setNerve(&NrvGCapture::GCaptureNrvBreak::sInstance);
        return true;
    }

    return false;
}

bool GCapture::tryFireDamage() {
    if (MR::isBindedDamageFire(this) && !MR::isPlayerElementModeInvincible()) {
        endBindByFireDamage();
        setNerve(&NrvGCapture::GCaptureNrvCoolDown::sInstance);
        return true;
    }

    return false;
}

bool GCapture::tryRelease() {
    if (MR::isGreaterStep(this, 180) || MR::isBinded(this)) {
        if (MR::sendMsgToBindedSensor(197, this, getSensor("body"))) {
            return false;
        }

        MR::startBckPlayer("Fall", "GCaptureFall");
        releaseTractTarget();
        setNerve(&NrvGCapture::GCaptureNrvBreak::sInstance);
        return true;
    }

    return false;
}

void GCapture::exeCoolDown() {
    if (MR::isFirstStep(this)) {
        MR::offCalcGravity(this);
        mTractPower = 0.0f;
        MR::forceDeleteEffect(this, "Light");
        MR::zeroVelocity(this);
    }

    if (MR::isGreaterStep(this, 10)) {
        setNerve(&NrvGCapture::GCaptureNrvWait::sInstance);
    }
}

void GCapture::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::offCalcGravity(this);
        MR::offBind(this);
        mTractPower = 0.0f;
        MR::forceDeleteEffect(this, "Light");
    }

    if (tryCapture()) {
        return;
    }
}

void GCapture::exeCapture() {
    TVec3f pos;
    _110->getTargetPosition(&pos);
    mPosition.set< f32 >(*MR::getPlayerCenterPos());

    if (MR::isFirstStep(this)) {
        _128 = 0.1f;
        _8C.identity();
        _8C.setTrans(pos);
        MR::emitEffect(this, "RibbonPoint");
        MR::zeroVelocity(this);
        MR::onBind(this);
    }

    if (MR::isStep(this, 30)) {
        getSensor("bind")->validate();
    }

    updateRibbon(pos, 30);
    MR::reboundVelocityFromCollision(this, 0.5f, 0.0f, 1.0f);

    if (tryCancelCapture()) {
        return;
    }
}

void GCapture::endCapture() {
    MR::deleteEffect(this, "RibbonPoint");
    MR::emitEffect(this, "RibbonBreak");
    getSensor("bind")->invalidate();
}

void GCapture::exeRecapture() {
    TVec3f targetPos;
    _110->getTargetPosition(&targetPos);

    if (MR::isFirstStep(this)) {
        mCaptureRibbon->lengthen(targetPos, targetPos);
        _128 = 1.0f;
        _8C.identity();
        _8C.setTrans(targetPos);
        MR::emitEffect(this, "RibbonPoint");
        MR::onCalcGravity(this);
    } else {
        updateRibbon(targetPos, 15);
    }

    downTractPower();
    MR::reboundVelocityFromCollision(this, 0.5f, 0.0f, 1.0f);
    fillGapBindTrans();
    MR::attenuateVelocity(this, 0.99f);

    if (tryFireDamage() || tryRecapture() || tryRetraction() || tryBreak()) {
        return;
    }
}

void GCapture::endRecapture() {
    MR::deleteEffect(this, "RibbonPoint");
    MR::emitEffect(this, "RibbonBreak");
    getSensor("bind")->invalidate();
}

void GCapture::exeTraction() {
    TVec3f targetPos, rand, v22, dir, v20;
    _110->getTargetPosition(&targetPos);

    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("SpaceStruggle", "GCaptureTraction");
        MR::getRandomVector(&rand, 1.0f);
        MR::normalizeOrZero(&rand);
        v22.set(targetPos - mPosition);
        MR::normalizeOrZero(&v22);
        _FC.cross(v22, rand);
        MR::normalizeOrZero(&_FC);
        MR::onCalcGravity(this);
        MR::emitEffect(this, "LightSplash");
    }

    if (MR::isStep(this, 30)) {
        MR::emitEffect(this, "Light");
    }

    if (MR::isLessStep(this, 35)) {
        _F0 += _FC * 0.002f;
    }

    f32 scalar;
    MR::separateScalarAndDirection(&scalar, &dir, targetPos - mPosition);

    if (_128 < 1.0f) {
        _128 += 0.033333335f;
        if (_128 > 1.0f) {
            _128 = 1.0f;
        }
    }

    if (scalar > 180.0f * _128) {
        v20.set(mPosition + dir * _128 * 180.0f);
        mCaptureRibbon->shorten(targetPos, v20);
        if (MR::isNearZero(dir, 0.001f)) {
            _8C.setTrans(v20);
        } else {
            MR::makeMtxUpNoSupportPos(&_8C, dir, v20);
        }
    } else {
        mCaptureRibbon->shorten(targetPos, targetPos);
        _8C.identity();
        _8C.setTrans(targetPos);
    }

    MR::setBinderRadius(this, (75.0f * (1.0f - _128)) + (130.0f * _128));
    f32 velocity;

    if (scalar < 500.0f) {
        f32 tmp = scalar / 500.0f;
        f32 tmp2 = 1.0f - tmp;
        velocity = 0.99f * tmp + (0.75f * tmp2);  // lerp inline, cleanup needed
    } else {
        velocity = 0.99f;
    }

    mVelocity += dir * 0.7f * mTractPower;
    MR::attenuateVelocity(this, velocity);
    upTractPower();
    MR::reboundVelocityFromCollision(this, 0.5f, 0.0f, 1.0f);
    tryAddVelocityReflectJumpCollision();
    fillGapBindTrans();

    if (tryFireDamage() || tryEndTraction() || tryBreak() || tryRecapture()) {
        MR::deleteEffect(this, "LightSplash");
        MR::emitEffect(this, "Light");
    }
}

void GCapture::exeHold() {
    if (MR::isFirstStep(this)) {
        MR::startBckPlayer("SpaceWait", "GCaptureHold");
        MR::onCalcGravity(this);
    }

    MR::startLevelSound(this, "SE_OJ_LV_GCAPTURE_CAPTURE");
    downTractPower();
    fillGapBindTrans();
    MR::reboundVelocityFromCollision(this, 0.5f, 0.0f, 1.0f);
    bool tryAdd = tryAddVelocityReflectJumpCollision();
    mIsStarPointerPointing = MR::isStarPointerPointing(this, 0, true, "弱");
    addRotateAccelPointing();
    MR::attenuateVelocity(this, 0.99f);
    MR::addTransMtx(_8C, mVelocity);

    if (tryFireDamage() || tryRecapture() || tryBreak()) {
        return;
    }

    if (tryAdd || tryRelease()) {
        return;
    }
}

void GCapture::exeBreak() {
    TVec3f dest, yDir;
    TQuat4f rot;
    if (MR::isFirstStep(this)) {
        MR::calcGravityVector(this, mEffectPos, &dest, nullptr, 0);
        _BC.getYDir(yDir);
        rot.setRotate(yDir, -dest);
        _BC.mult(rot, _BC);
        _F0.zero();
        MR::emitEffect(this, "LightBreak");
        MR::startSound(this, "SE_OJ_GCAPTURE_RELEASE");
    }

    if (_124 > 0.0f) {
        _124 -= 0.2f;
        if (_124 < 0.0f) {
            _124 = 0.0f;
        }
    }

    MR::attenuateVelocity(this, 0.99f);

    if (MR::isGreaterStep(this, 5)) {
        setNerve(&NrvGCapture::GCaptureNrvWait::sInstance);
        releaseTractTarget();
        MR::endBindAndPlayerWeakGravityLimitJump(this, mVelocity);
        _108 = nullptr;
        MR::endStarPointerMode(this);
    }
}

void GCapture::endBindByDamage(s32 a1) {
    MR::endBindAndPlayerDamageMsg(this, a1);
    _108 = nullptr;
    MR::deleteEffectAll(this);
    MR::emitEffect(this, "LightBreak");
    MR::startSound(this, "SE_OJ_GCAPTURE_RELEASE");
    releaseTractTarget();
    MR::endStarPointerMode(this);
}

void GCapture::endBindByFireDamage() {
    MR::endBindAndPlayerFireDamage(this);
    _108 = nullptr;
    MR::deleteEffectAll(this);
    MR::emitEffect(this, "LightBreak");
    MR::startSound(this, "SE_OJ_GCAPTURE_RELEASE");
    releaseTractTarget();
    MR::endStarPointerMode(this);
}

void GCapture::calcBindActorPose() {
    f32 scalar;
    TVec3f dir;
    TQuat4f rot;
    MR::separateScalarAndDirection(&scalar, &dir, _F0);
    rot.setRotate(dir, scalar);
    _BC.mult(rot, _BC);
    _BC.normalize();
}

void GCapture::updateBindActorMatrix() {
    TPos3f pos;
    pos.setQT(_BC, mEffectPos);
    MR::setBaseTRMtx(_108, pos);
}

bool GCapture::canRequestTarget() const {
    return !MR::testCorePadButtonA(WPAD_CHAN0);
}

bool GCapture::canCancelBind() const {
    if (isNerve(&NrvGCapture::GCaptureNrvTraction::sInstance) || isNerve(&NrvGCapture::GCaptureNrvHold::sInstance) ||
        isNerve(&NrvGCapture::GCaptureNrvRecapture::sInstance)) {
        return true;
    }

    return false;
}

void GCapture::upTractPower() {
    mTractPower += 0.97f;
    mTractPower = MR::clamp(mTractPower, 0.0f, 1.0f);
}

void GCapture::downTractPower() {
    mTractPower -= 0.12f;
    mTractPower = MR::clamp(mTractPower, 0.0f, 1.0f);
}

void GCapture::fillGapBindTrans() {
    if (_124 > 0.0f) {
        _124 -= 0.05f;
        if (_124 < 0.0f) {
            _124 = 0.0f;
        }
    }
}

void GCapture::updateRibbon(const TVec3f& rVec, s32 step) {
    TVec3f v13(mPosition - rVec);
    TVec3f v12;
    MR::normalizeOrZero(v13, &v12);
    v13 -= v12 * _128 * 180.0f;
    f32 rate = MR::calcNerveRate(this, step);
    TVec3f v11(rVec + v13 * (rate * rate));
    mCaptureRibbon->lengthen(rVec, v11);
    updateRibbonPointEffectMatrix(v11);
}

void GCapture::updateRibbonPointEffectMatrix(const TVec3f& rVec) {
    TVec3f v5;
    f32 z = _8C.mMtx[2][3];
    f32 y = _8C.mMtx[1][3];
    f32 x = _8C.mMtx[0][3];
    v5.set< f32 >(x, y, z);
    TVec3f v4(v5);
    v4 -= rVec;

    if (MR::isNearZero(v4)) {
        _8C.setTrans(rVec);
    } else {
        MR::makeMtxUpNoSupportPos(&_8C, v4, rVec);
    }
}

void GCapture::addRotateAccelPointing() {
    TVec3f rotate;
    if (MR::calcStarPointerStrokeRotateMoment(&rotate, mPosition, 200.0f, 0)) {
        _F0 += rotate * 0.055104f;
        f32 mag = _F0.length();
        if (mag > 0.2f) {
            _F0.scale(0.2f / mag);
        }
    }
}

bool GCapture::tryAddVelocityReflectJumpCollision() {
    bool ret = MR::sendMsgToBindedSensor(196, this, getSensor("body"));
    if (ret) {
        MR::addVelocityToCollisionNormal(this, 25.0f);

        f32 len = mVelocity.length();

        if (len > 25.0f) {
            mVelocity.scale(25.0f / len);
        }
    }

    return ret;
}

bool GCapture::requestTarget(GCaptureTargetable* gCaptureTarget) {
    TVec3f targetPos;
    if (canRequestTarget()) {
        TVec2f pointerPos = *MR::getStarPointerScreenPosition(WPAD_CHAN0);
        gCaptureTarget->getTargetPosition(&targetPos);
        TVec2f screenPos;
        MR::calcScreenPosition(&screenPos, targetPos);
        f32 dist = screenPos.distance(pointerPos);
        if (dist < _130) {
            _130 = dist;
            mTarget = gCaptureTarget;
            _12C = false;
            return true;
        }
    }

    return false;
}

bool GCapture::isRequestedTarget(GCaptureTargetable* gCaptureTarget) {
    if (gCaptureTarget == nullptr) {
        return false;
    }

    return mTarget == gCaptureTarget;
}

void GCapture::decideTractTarget() {
    if (_110 != mTarget) {
        releaseTractTarget();
        mTarget->decidedTarget();
        _110 = mTarget;
        mCaptureRibbon->reset();
        mTarget = nullptr;
    }
}

void GCapture::releaseTractTarget() {
    if (_110 != nullptr) {
        _110->releasedTarget();
        mCaptureRibbon->reset();
    }

    _110 = nullptr;
}

void GCapture::updateCameraTargetMatrix() {
    TPos3f pos;
    pos.identity();

    if (_108 != nullptr) {
        pos.setTrans(*MR::getPlayerPos());
    } else {
        pos.setTrans(mEffectPos);
    }

    mTargetMtx->mMatrix.set(pos.mMtx);
}

namespace MR {
    void createGCapture() {
        MR::createSceneObj(SceneObj_GCapture);
    }

    void resetGCapture() {
        if (MR::isExistSceneObj(SceneObj_GCapture)) {
            GCapture* gCapture = static_cast< GCapture* >(MR::getSceneObjHolder()->getObj(SceneObj_GCapture));
            gCapture->reset();
        }
    }

    bool requestGCaptureTarget(GCaptureTargetable* gCaptureTarget) {
        GCapture* gCapture = static_cast< GCapture* >(MR::getSceneObjHolder()->getObj(SceneObj_GCapture));
        return gCapture->requestTarget(gCaptureTarget);
    }

    void unrequestGCaptureTarget(GCaptureTargetable* gCaptureTarget) {
        GCapture* gCapture = static_cast< GCapture* >(MR::getSceneObjHolder()->getObj(SceneObj_GCapture));
        if (gCapture->isRequestedTarget(gCaptureTarget)) {
            gCapture->mTarget = nullptr;
        }
    }

    bool isRequestedGCaptureTarget(GCaptureTargetable* gCaptureTarget) {
        GCapture* gCapture = static_cast< GCapture* >(MR::getSceneObjHolder()->getObj(SceneObj_GCapture));
        return gCapture->isRequestedTarget(gCaptureTarget);
    }

    void noticeInTouchableRange() {
        GCapture* gCapture = static_cast< GCapture* >(MR::getSceneObjHolder()->getObj(SceneObj_GCapture));
        gCapture->mGuidanceTime = 20;
    }

    bool isOnTractTrigger() {
        return MR::testCorePadTriggerA(WPAD_CHAN0) != false;
    }

    bool isPlayerGCaptured() {
        if (!MR::isExistSceneObj(SceneObj_GCapture)) {
            return false;
        }
        GCapture* gCapture = static_cast< GCapture* >(MR::getSceneObjHolder()->getObj(SceneObj_GCapture));
        if (gCapture == nullptr) {
            return false;
        }

        return gCapture->_108;
    }
}  // namespace MR
