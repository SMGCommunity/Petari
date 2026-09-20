#include "Game/LiveActor/ShadowVolumeOval.hpp"
#include "Game/LiveActor/LiveActor.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/MathUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include <JSystem/JGeometry/TMatrix.hpp>

ShadowVolumeOval::ShadowVolumeOval() : ShadowVolumeModel("影描画[ボリューム楕球]"), mSize(100.0f, 100.0f, 200.0f) {
    initVolumeModel("ShadowVolumeSphere");
}

bool ShadowVolumeOval::isDraw() const {
    ShadowController* controller = getController();

    return controller->isProjected() && controller->isDraw();
}

void ShadowVolumeOval::loadModelDrawMtx() const {
    MtxPtr dropMtx;
    ShadowController* controller = getController();
    dropMtx = controller->_18;
    TVec3f direction;
    controller->getDropDir(&direction);

    TVec3f size(mSize / 100.0f);
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

    TPos3f scaledMtx;
    scaledMtx.set(dropMtx);
    scaledMtx.setTrans(TVec3f(0.0f, 0.0f, 0.0f));
    MR::preScaleMtx(scaledMtx, size);

    TPos3f inverseMtx;
    inverseMtx.invert(scaledMtx);

    TVec3f localDirection;
    inverseMtx.mult(direction, localDirection);

    TPos3f mtx;
    mtx.identity();
    MR::makeMtxUpNoSupport(&mtx, localDirection);
    mtx.concat(scaledMtx, mtx);

    TVec3f position;
    controller->getProjectionPos(&position);
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

    mtx.setXDir(side);
    mtx.setZDir(front);
    PSMTXConcat(MR::getCameraViewMtx(), mtx, mtx);
    GXLoadPosMtxImm(mtx, 0);
}

void ShadowVolumeOval::setSize(const TVec3f& rSize) {
    mSize = rSize;
}
