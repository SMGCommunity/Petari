#include "Game/LiveActor/ShadowVolumeLine.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DirectDraw.hpp"
#include "Game/Util/MathUtil.hpp"

ShadowVolumeLine::ShadowVolumeLine() : ShadowVolumeDrawer("影描画[ボリュームライン]") {
    mFromShadowController = 0;
    mToShadowController = 0;
    mFromWidth = 100.0f;
    mToWidth = 100.0f;
}

void ShadowVolumeLine::loadModelDrawMtx() const {
    GXLoadPosMtxImm(MR::getCameraViewMtx(), 0);
    GXSetCurrentMtx(0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
}

void ShadowVolumeLine::drawShape() const {
    const ShadowController* fromController = mFromShadowController;
    const ShadowController* toController = mToShadowController;
    TVec3f points[8];
    TVec3f from;
    TVec3f to;
    calcBaseDropPosition(&from, fromController);
    calcBaseDropPosition(&to, toController);
    TVec3f direction;
    MR::normalizeOrZero(to - from, &direction);
    if (MR::isNearZero(direction)) {
        return;
    }

    TVec3f fromDrop;
    TVec3f toDrop;
    fromController->getDropDir(&fromDrop);
    toController->getDropDir(&toDrop);
    TVec3f fromSide;
    fromSide.cross(fromDrop, direction);
    if (MR::isNearZero(fromSide)) {
        return;
    }

    MR::normalize(&fromSide);
    TVec3f toSide;
    toSide.cross(toDrop, direction);
    if (MR::isNearZero(toSide)) {
        return;
    }

    MR::normalize(&toSide);
    f32 fromLength = mFromWidth + calcBaseDropLength(fromController);
    f32 toLength = mToWidth + calcBaseDropLength(toController);
    points[0].scaleAdd(-mFromWidth, fromSide, from);
    points[1].scaleAdd(mFromWidth, fromSide, from);
    points[2].scaleAdd(fromLength, fromDrop, points[0]);
    points[3].scaleAdd(fromLength, fromDrop, points[1]);
    points[4].scaleAdd(-mToWidth, toSide, to);
    points[5].scaleAdd(mToWidth, toSide, to);
    points[6].scaleAdd(toLength, toDrop, points[4]);
    points[7].scaleAdd(toLength, toDrop, points[5]);

    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    TDDraw::sendPoint(points[1]);
    TDDraw::sendPoint(points[5]);
    TDDraw::sendPoint(points[7]);
    TDDraw::sendPoint(points[3]);
    GXEnd();

    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    TDDraw::sendPoint(points[0]);
    TDDraw::sendPoint(points[2]);
    TDDraw::sendPoint(points[6]);
    TDDraw::sendPoint(points[4]);
    GXEnd();

    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 10);
    TDDraw::sendPoint(points[0]);
    TDDraw::sendPoint(points[1]);
    TDDraw::sendPoint(points[2]);
    TDDraw::sendPoint(points[3]);
    TDDraw::sendPoint(points[6]);
    TDDraw::sendPoint(points[7]);
    TDDraw::sendPoint(points[4]);
    TDDraw::sendPoint(points[5]);
    TDDraw::sendPoint(points[0]);
    TDDraw::sendPoint(points[1]);
    GXEnd();
}

void ShadowVolumeLine::setFromShadowController(const ShadowController* pController) {
    mFromShadowController = pController;
}

void ShadowVolumeLine::setToShadowController(const ShadowController* pController) {
    mToShadowController = pController;
}

void ShadowVolumeLine::setFromWidth(f32 width) {
    mFromWidth = width;
}

void ShadowVolumeLine::setToWidth(f32 width) {
    mToWidth = width;
}

ShadowVolumeLine::~ShadowVolumeLine() {
}
