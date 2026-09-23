#include "Game/LiveActor/ShadowVolumeOvalPole.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include <revolution/gx.h>

namespace {
    const f32 sModelScale = 100.0f;
}

void ShadowVolumeOvalPole_FORCE_MATCH_SDATA2() {
    1.0f;
    0.0f;
}

ShadowVolumeOvalPole::~ShadowVolumeOvalPole() {
}

ShadowVolumeOvalPole::ShadowVolumeOvalPole() : ShadowVolumeModel("影描画[ボリューム楕円柱]") {
    mSize.x = 100.0f;
    mSize.y = 100.0f;
    mSize.z = 200.0f;
    initVolumeModel("ShadowVolumeCylinder");
}

void ShadowVolumeOvalPole::loadModelDrawMtx() const {
    ShadowController* controller = getController();
    MtxPtr dropMtx = controller->_18;
    TPos3f scaledMtx;
    TPos3f inverseMtx;
    TPos3f mtx;
    TVec3f position;
    calcBaseDropPosition(&position);
    TVec3f direction;
    controller->getDropDir(&direction);
    direction.negate();
    f32 modelScale = ::sModelScale;
    TVec3f size(mSize / modelScale);
    if (size.x <= 0.01f) {
        size.x = 0.01f;
    }

    if (size.y <= 0.01f) {
        size.y = 0.01f;
    }

    if (size.z <= 0.01f) {
        size.z = 0.01f;
    }

    if (controller->isFollowHostScale()) {
        size *= controller->getHost()->mScale;
    }

    scaledMtx.set(dropMtx);
    scaledMtx.setTrans(TVec3f(0.0f, 0.0f, 0.0f));
    MR::preScaleMtx(scaledMtx, size);
    inverseMtx.invert(scaledMtx);
    TVec3f localDirection;
    inverseMtx.mult(direction, localDirection);
    mtx.identity();
    MR::makeMtxUpNoSupport(&mtx, localDirection);
    mtx.concat(scaledMtx, mtx);
    mtx.setTrans(position);

    TVec3f side;
    TVec3f up;
    TVec3f front;
    mtx.getXDir(side);
    mtx.getYDir(up);
    mtx.getZDir(front);
    if (!MR::normalizeOrZero(&up)) {
        side.orthogonalize(up);
        front.orthogonalize(up);
    }

    up.scale(calcBaseDropLength() / ::sModelScale);
    mtx.setXDir(side);
    mtx.setYDir(up);
    mtx.setZDir(front);
    PSMTXConcat(MR::getCameraViewMtx(), mtx, mtx);
    GXLoadPosMtxImm(mtx, 0);
}

void ShadowVolumeOvalPole::setSize(const TVec3f& rSize) {
    mSize = rSize;
}
