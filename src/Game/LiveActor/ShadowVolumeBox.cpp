#include "Game/LiveActor/ShadowVolumeBox.hpp"

void ShadowVolumeBox::setSize(const TVec3f &rSize) {
    mSize.set<f32>(rSize);
}

void ShadowVolumeBox::loadModelDrawMtx() const {
    GXLoadPosMtxImm(MR::getCameraViewMtx(), 0);
    GXSetCurrentMtx(0);
    GXClearVtxDesc();
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    makeVertexBuffer();
}

void ShadowVolumeBox::drawShape() const {
    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 0xE);
    TDDraw::sendPoint(_28);
    TDDraw::sendPoint(_34);
    TDDraw::sendPoint(_40);
    TDDraw::sendPoint(_4C);
    TDDraw::sendPoint(_A0);
    TDDraw::sendPoint(_AC);
    TDDraw::sendPoint(_70);
    TDDraw::sendPoint(_7C);
    TDDraw::sendPoint(_58);
    TDDraw::sendPoint(_64);
    TDDraw::sendPoint(_88);
    TDDraw::sendPoint(_94);
    TDDraw::sendPoint(_28);
    TDDraw::sendPoint(_34);
    GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, 8);
    TDDraw::sendPoint(_B8);
    TDDraw::sendPoint(_28);
    TDDraw::sendPoint(_40);
    TDDraw::sendPoint(_A0);
    TDDraw::sendPoint(_70);
    TDDraw::sendPoint(_58);
    TDDraw::sendPoint(_88);
    TDDraw::sendPoint(_28);
    GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, 8);
    TDDraw::sendPoint(_C4);
    TDDraw::sendPoint(_34);
    TDDraw::sendPoint(_94);
    TDDraw::sendPoint(_64);
    TDDraw::sendPoint(_7C);
    TDDraw::sendPoint(_AC);
    TDDraw::sendPoint(_4C);
    TDDraw::sendPoint(_34);
}

ShadowVolumeBox::~ShadowVolumeBox() {

}
