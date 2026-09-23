#include "Game/LiveActor/ShadowVolumeSphere.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/CameraUtil.hpp"
#include <revolution/gx.h>

namespace {
    const f32 sModelScale = 100.0f;
}

void ShadowVolumeSphere_FORCE_MATCH_SDATA2() {
    0.0f;
}

ShadowVolumeSphere::~ShadowVolumeSphere() {
}

ShadowVolumeSphere::ShadowVolumeSphere() : ShadowVolumeModel("影描画[ボリューム球]"), mRadius(100.0f) {
    initVolumeModel("ShadowVolumeSphere");
}

void ShadowVolumeSphere::setRadius(f32 radius) {
    mRadius = radius;
}

bool ShadowVolumeSphere::isDraw() const {
    ShadowController* controller = getController();

    return controller->isProjected() && controller->isDraw();
}

void ShadowVolumeSphere::loadModelDrawMtx() const {
    ShadowController* controller = getController();
    f32 radius = mRadius / ::sModelScale;
    if (controller->isFollowHostScale()) {
        radius *= controller->getHost()->mScale.x;
    }

    TPos3f mtx;
    mtx.identity();
    mtx.mMtx[0][0] = radius;
    mtx.mMtx[0][1] = 0.0f;
    mtx.mMtx[0][2] = 0.0f;
    mtx.mMtx[1][0] = 0.0f;
    mtx.mMtx[1][1] = radius;
    mtx.mMtx[1][2] = 0.0f;
    mtx.mMtx[2][0] = 0.0f;
    mtx.mMtx[2][1] = 0.0f;
    mtx.mMtx[2][2] = radius;
    TVec3f position;
    controller->getProjectionPos(&position);
    mtx.setTrans(position);

    PSMTXConcat(MR::getCameraViewMtx(), mtx, mtx);
    GXLoadPosMtxImm(mtx, 0);
}
