#include "Game/MapObj/GCapture.hpp"
#include "Game/Camera/CameraTargetMtx.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/BlueStarCupsulePlanet.hpp"
#include "Game/MapObj/GCaptureRibbon.hpp"
#include "Game/Util.hpp"
#include "Game/Util/ActorMovementUtil.hpp"
#include "Game/Util/PlayerUtil.hpp"
#include "Game/Util/StarPointerUtil.hpp"

namespace NrvGCapture {
    NEW_NERVE(GCaptureNrvWait, GCapture, Wait);
    NEW_NERVE_ONEND(GCaptureNrvCapture, GCapture, Capture, Capture);
    NEW_NERVE_ONEND(GCaptureNrvRecapture, GCapture, Recapture, Recapture);
    NEW_NERVE(GCaptureNrvTraction, GCapture, Traction);
    NEW_NERVE(GCaptureNrvHold, GCapture, Hold);
    NEW_NERVE(GCaptureNrvBreak, GCapture, Break);
    NEW_NERVE(GCaptureNrvCoolDown, GCapture, CoolDown);
};  // namespace NrvGCapture

void GCapture::draw() const {
    mCaptureRibbon->draw();
}

void GCapture::control() {
    if (_134 > 0) {
        MR::requestBlueStarGuidance();
        _134--;
    }

    if (_12C > 30) {
        mTarget = nullptr;
        _12C = 0;
    }

    _139 = 0;
    _130 = FLOAT_MAX;

    if (mTarget != nullptr) {
        _12C++;
    }

    if (_108) {
        if (0.0f == _124) {
            _CC = mPosition;
        } else {
            f32 easeInOut = MR::getEaseInOutValue(_124, 0.0f, 1.0f, 1.0f);
            TVec3f v7(_E4);
            v7.scale(easeInOut);
            TVec3f v8(mPosition);
            v8 += v7;
            _CC = v8;
        }

        calcBindActorPose();
        _F0 *= 0.991f;
        updateCameraTargetMatrix();
    }
}

void GCapture::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (_108 != nullptr) {
        if (!MR::isSensorAutoRush(pSender)) {
            if (MR::tryGetItem(pSender, pReceiver)) {
                return;
            }
        }
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
    _CC = _D8;
    TVec3f v7(_CC);
    v7 -= mPosition;
    _E4 = v7;

    _124 = 1.0f;
    MR::zeroVelocity(this);

    TRot3f mtx;
    mtx.set(_108->getBaseMtx());
    mtx.getQuat(_BC);
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
    if (mTarget != nullptr && MR::testCorePadTriggerA(0)) {
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

    if (MR::isGreaterStep(this, 40) || (!MR::testCorePadButtonA(0) && MR::isGreaterStep(this, 30)) || ret) {
        releaseTractTarget();
        setNerve(&NrvGCapture::GCaptureNrvWait::sInstance);
        return true;
    }

    return false;
}

bool GCapture::tryEndTraction() {
    if (!MR::testCorePadButtonA(0) && MR::isGreaterStep(this, 30)) {
        releaseTractTarget();
        setNerve(&NrvGCapture::GCaptureNrvHold::sInstance);
        return true;
    }

    return false;
}

bool GCapture::tryRecapture() {
    if (mTarget != nullptr && MR::testCorePadTriggerA(0)) {
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

// GCapture::tryBreak

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

void GCapture::updateRibbon(const TVec3f& rVec, s32 a2) {
    TVec3f v13(mPosition - rVec);
    TVec3f v12;
    MR::normalizeOrZero(v13, &v12);
    v13 -= v12 * _128 * 180.0f;
    f32 rate = MR::calcNerveRate(this, a2);
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

// GCapture::requestTarget

bool GCapture::isRequestedTarget(GCaptureTargetable* pTarget) {
    if (pTarget == nullptr) {
        return false;
    }

    return mTarget == pTarget;
}

void GCapture::decideTractTarget() {
    if (_110 != mTarget) {
        releaseTractTarget();
        mTarget->canEndHold();
        _110 = mTarget;
        mCaptureRibbon->reset();
        mTarget = nullptr;
    }
}

void GCapture::releaseTractTarget() {
    if (_110 != nullptr) {
        _110->isReleaseForce();
        mCaptureRibbon->reset();
    }

    _110 = nullptr;
}

void GCapture::updateCameraTargetMatrix() {
    TPos3f mtx;
    mtx.identity();

    if (_108) {
        mtx.setTrans(*MR::getPlayerPos());
    } else {
        mtx.setTrans(_CC);
    }

    mTargetMtx->mMatrix.set(mtx);
}
