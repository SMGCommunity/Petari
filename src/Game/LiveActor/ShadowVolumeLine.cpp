#include "Game/LiveActor/ShadowVolumeLine.hpp"

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

void ShadowVolumeLine::setFromShadowController(const ShadowController *pController) {
    mFromShadowController = pController;
}

void ShadowVolumeLine::setToShadowController(const ShadowController *pController) {
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