#include "Game/MapObj/GCapture.hpp"
#include "Game/LiveActor/HitSensor.hpp"
#include "Game/MapObj/GCaptureRibbon.hpp"
#include "Game/LiveActor/Nerve.hpp"

namespace NrvGCapture {
    NEW_NERVE(GCaptureNrvWait, GCapture, Wait);
    NEW_NERVE_ONEND(GCaptureNrvCapture, GCapture, Capture, Capture);
    NEW_NERVE_ONEND(GCaptureNrvRecapture, GCapture, Recapture, Recapture);
    NEW_NERVE(GCaptureNrvTraction, GCapture, Traction);
    NEW_NERVE(GCaptureNrvHold, GCapture, Hold);
    NEW_NERVE(GCaptureNrvBreak, GCapture, Break);
    NEW_NERVE(GCaptureNrvCoolDown, GCapture, CoolDown);
};

void GCapture::draw() const {
    mCaptureRibbon->draw();
}

// GCapture::control

void GCapture::attackSensor(HitSensor *pSender, HitSensor *pReceiver) {
    if (_108) {
        if (!MR::isSensorAutoRush(pSender)) {
            if (MR::tryGetItem(pSender, pReceiver)) {
                return;
            }
        }
    }
}

void GCapture::updateRibbon(const TVec3f &rVec, s32 a2) {
    TVec3f v13(mPosition);
    v13 -= rVec;
    TVec3f v12;
    MR::normalizeOrZero(v13, &v12);
    f32 scalar = _128;
    TVec3f v9(v12);
    v9.scale(scalar);
    TVec3f v10(v9);
    v10.scale(180.0f);
    v13 -= v10;
    f32 rate = MR::calcNerveRate(this, a2);
    TVec3f v8(v13);
    v8.scale(rate * rate);
    TVec3f v11(rVec);
    v11 += v8;
    mCaptureRibbon->lengthen(rVec, v11);
    updateRibbonPointEffectMatrix(v11);
}

void GCapture::updateRibbonPointEffectMatrix(const TVec3f &rVec) {
    TVec3f v5;
    f32 z = _8C.mMtx[2][3];
    f32 y = _8C.mMtx[1][3];
    f32 x = _8C.mMtx[0][3];
    v5.set<f32>(x, y, z);
    TVec3f v4(v5);
    v4 -= rVec;
    if (MR::isNearZero(v4, 0.001f)) {
        _8C.setTrans(rVec);
    }
    else {
        MR::makeMtxUpNoSupportPos(&_8C, v4, rVec);
    }
}

void GCapture::addRotateAccelPointing() {
    TVec3f rotate;
    if (MR::calcStarPointerStrokeRotateMoment(&rotate, mRotation, 200.0f, 0)) {
        TVec3f v3(rotate);
        v3.scale(0.055103999f);
        _F0 += v3;
        f32 mag = PSVECMag(&_F0);
        if (mag > 0.2f) {
            _F0.scale(0.2f / mag);
        }
    }
}
