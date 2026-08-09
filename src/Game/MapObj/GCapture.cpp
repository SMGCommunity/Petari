#include "Game/MapObj/GCapture.hpp"
#include "Game/Camera/CameraTargetMtx.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/LiveActor/Nerve.hpp"
#include "Game/MapObj/BlueStarCupsulePlanet.hpp"
#include "Game/MapObj/GCaptureRibbon.hpp"
#include "Game/Util.hpp"
#include "Game/Util/PlayerUtil.hpp"

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

// GCapture::control

void GCapture::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    if (_108) {
        if (!MR::isSensorAutoRush(pSender)) {
            if (MR::tryGetItem(pSender, pReceiver)) {
                return;
            }
        }
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
