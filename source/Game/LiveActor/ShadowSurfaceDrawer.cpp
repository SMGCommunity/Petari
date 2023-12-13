#include "Game/LiveActor/ShadowSurfaceDrawer.hpp"
#include "Game/LiveActor/ShadowVolumeDrawer.hpp"
#include "Game/Scene/SceneObjHolder.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/DirectDraw.hpp"

/*
ShadowSurfaceDrawInit::ShadowSurfaceDrawInit(const char *pName) : NameObj(pName) {
    MR::FunctorV0F functor(*ShadowSurfaceDrawInit::initDraw);
    MR::registerPreDrawFunction(functor, 0x26);
}
*/

#ifdef NON_MATCHING
void ShadowSurfaceDrawInit::initDraw() {
    TDDraw::setup(0, 1, 1);
    GXSetChanCtrl(GX_COLOR0A0, 0, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
    GXSetChanCtrl(GX_COLOR1A1, 0, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
    GXColor materialColor;
    materialColor.r = 0;
    materialColor.g = 0;
    materialColor.b = 0;
    materialColor.a = 0x80;
    GXSetChanMatColor(GX_COLOR0A0, materialColor);
    GXSetZMode(1, GX_LEQUAL, 0);
    GXSetCullMode(GX_CULL_BACK);
    GXSetColorUpdate(1);
    GXSetAlphaUpdate(1);
    GXSetDstAlpha(1, 0);
}
#endif

ShadowSurfaceDrawer::ShadowSurfaceDrawer(const char *pName) : ShadowDrawer(pName) {
    MR::createSceneObj(SceneObj_ShadowSurfaceDrawerInit);
    MR::connectToScene(this, -1, -1, -1, 0x26);
}

ShadowSurfaceDrawInit::~ShadowSurfaceDrawInit() {

}