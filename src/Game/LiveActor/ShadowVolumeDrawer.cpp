#include "Game/LiveActor/ShadowVolumeDrawer.hpp"
#include "Game/Scene/SceneObjHolder.hpp"

namespace {
    static Color8 sShapeColor(0xC0, 0xC0, 0, 4);
    static Color8 sDebugShapeColor(0xFF, 0, 0, 0x80);
};

ShadowVolumeDrawInit::ShadowVolumeDrawInit() : NameObj("シャドウボリューム描画初期化") {
    MR::FunctorV0F func(*MR::setupShadowVolumeDraw);
    MR::registerPreDrawFunction(func, 0x27);
}

ShadowVolumeDrawer::ShadowVolumeDrawer(const char *pName) : ShadowDrawer(pName) {
    mStartDrawShapeOffset = 0.0f;
    mEndDrawShapeOffset = 0.0f;
    mIsCutDropShadow = false;
    MR::createSceneObj(SceneObj_ShadowVolumeDrawer);
    MR::connectToScene(this, -1, -1, -1, 0x27);
}

void ShadowVolumeDrawer::setStartDrawShepeOffset(f32 offs) {
    mStartDrawShapeOffset = offs;
}

void ShadowVolumeDrawer::setEndDrawShepeOffset(f32 offs) {
    mEndDrawShapeOffset = offs;
}

void ShadowVolumeDrawer::onCutDropShadow() {
    mIsCutDropShadow = true;
}

void ShadowVolumeDrawer::offCutDropShadow() {
    mIsCutDropShadow = false;
}

void ShadowVolumeDrawer::calcBaseDropPosition(TVec3f *pVec) const {
    calcBaseDropPosition(pVec, getController());
}

f32 ShadowVolumeDrawer::calcBaseDropLength() const {
    return calcBaseDropLength(getController());
}

#ifdef NON_MATCHING
// reg usage issues at the bottom
f32 ShadowVolumeDrawer::calcBaseDropLength(const ShadowController *pController) const {
    f32 length = pController->getDropLength();

    if (mIsCutDropShadow) {
        if (pController->isProjected()) {
            length = pController->getProjectionLength();
        }
    }

    f32 negStart = -mStartDrawShapeOffset;
    length += mEndDrawShapeOffset + negStart;
    return length;
}
#endif

void ShadowVolumeDrawer::loadModelDrawMtx() const {

}

void ShadowVolumeDrawer::drawShape() const {

}

bool ShadowVolumeDrawer::isDraw() const {
    return getController()->isDraw();
}

void ShadowVolumeDrawer::draw() const {
    if (isDraw()) {
        loadModelDrawMtx();
        GXColor shapeColor = reinterpret_cast<GXColor&>(sShapeColor);
        GXSetTevColor(GX_TEVREG0, shapeColor);
        GXSetColorUpdate(0);
        GXSetDstAlpha(0, 0);
        GXSetCullMode(GX_CULL_FRONT);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_LO_COPY);
        drawShape();
        GXSetCullMode(GX_CULL_BACK);
        GXSetBlendMode(GX_BM_SUBTRACT, GX_BL_ZERO, GX_BL_ZERO, GX_LO_COPY);
        drawShape();
        GXSetCullMode(GX_CULL_NONE);
        GXSetColorUpdate(1);
        GXSetDstAlpha(1, 0);
    }
}

ShadowVolumeDrawInit::~ShadowVolumeDrawInit() {

}