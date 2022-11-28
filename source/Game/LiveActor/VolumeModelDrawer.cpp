#include "Game/LiveActor/VolumeModelDrawer.h"
#include "Game/Scene/SceneObjHolder.h"
#include "Game/System/ResourceHolder.h"
#include <cstdio>

VolumeModelDrawInit::VolumeModelDrawInit() : NameObj("ボリュームモデル描画初期化") {
    MR::FunctorV0F func(*MR::setupShadowVolumeDraw);
    MR::registerPreDrawFunction(func, 0x1C);
}

VolumeModelDrawer::VolumeModelDrawer(const char *pName, const char *pFileName, MtxPtr mtx) : NameObj(pName) {
    mMtx = 0;
    mModelData = 0;
    mColor.r = 0xFF;
    mColor.g = 0xA9;
    mColor.b = 0;
    mColor.a = 0xFF;
    MR::createSceneObj(SceneObj_VolumeModelDrawInit);
    mMtx = mtx;
    char buf[0x100];
    snprintf(buf, 0x100, "%s.arc", pFileName);
    ResourceHolder* resHolder = MR::createAndAddResourceHolder(buf);
    mModelData = (J3DModelData*)resHolder->mModelResTable->getRes(pFileName);
}

void VolumeModelDrawer::draw() const {
    MR::loadProjectionMtx();
    loadModelDrawMtx();
    GXSetCurrentMtx(0);
    GXSetColorUpdate(0);
    GXSetAlphaUpdate(1);
    GXSetDstAlpha(1, 0);
    GXSetCullMode(GX_CULL_NONE);
    GXColor color_1 = mColor;
    GXSetTevColor(GX_TEVREG0, color_1);
    GXSetZMode(0, GX_ALWAYS, 0);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ZERO, GX_LO_COPY);
    MR::drawSimpleModel(mModelData);
    GXSetDstAlpha(0, 0);
    GXSetZMode(1, GX_GEQUAL, 0);
    GXSetCullMode(GX_CULL_FRONT);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ZERO, GX_LO_COPY);
    MR::drawSimpleModel(mModelData);
    GXSetCullMode(GX_CULL_BACK);
    GXSetBlendMode(GX_BM_SUBTRACT, GX_BL_ZERO, GX_BL_ZERO, GX_LO_COPY);
    MR::drawSimpleModel(mModelData);
    GXColor color_2 = mColor;
    GXSetTevColor(GX_TEVREG0, color_2);
    GXSetCullMode(GX_CULL_NONE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_DSTALPHA, GX_BL_ONE, GX_LO_COPY);
    GXSetColorUpdate(1);
    GXSetDstAlpha(1, 0);
    GXSetZMode(0, GX_ALWAYS, 0);
    MR::drawSimpleModel(mModelData);
}

void VolumeModelDrawer::loadModelDrawMtx() const {
    Mtx dest;
    PSMTXConcat(MR::getCameraViewMtx(), mMtx, dest);
    GXLoadPosMtxImm(dest, 0);
}

VolumeModelDrawer::~VolumeModelDrawer() {

}

VolumeModelDrawInit::~VolumeModelDrawInit() {
    
}