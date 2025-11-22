#include "Game/Camera/CameraContext.hpp"
#include "Game/Scene/GameSceneFunction.hpp"
#include "Game/Util/DrawUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include <revolution/gx/GXRegs.h>

const JUTTexture* mShadowTex;
TVec3f            mShadowVec;

namespace {
    GXTexObj clear_z_tobj;
};

namespace MR {
    void fillScreenArea(const TVec2s& rMin, const TVec2s& rMax) {
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        GX_WRITE_U16(rMin.x);
        GX_WRITE_U16(rMin.y);
        GX_WRITE_U16(rMax.x);
        GX_WRITE_U16(rMin.y);
        GX_WRITE_U16(rMax.x);
        GX_WRITE_U16(rMax.y);
        GX_WRITE_U16(rMin.x);
        GX_WRITE_U16(rMax.y);
    }

    void fillScreen(const GXColor& color) {
        fillScreenSetup(color);

        u16 width = getFrameBufferWidth();
        u16 height = getScreenHeight();

        fillScreenArea(TVec2s(0, 0), TVec2s(width, height));
    }

    const JUTTexture* getMarioShadowTex() {
        return mShadowTex;
    }

    const JUTTexture* getMarioShadowTexForLoad() {
        return mShadowTex;
    }

    const TVec3f& getMarioShadowVec() {
        return mShadowVec;
    }

    void setMarioShadowTex(const JUTTexture* pShadowTex) {
        mShadowTex = pShadowTex;
    }

    void activateGameSceneDraw3D() {
        GameSceneFunction::activateDraw3D();
    }

    void deactivateGameSceneDraw3D() {
        GameSceneFunction::deactivateDraw3D();
    }
}; // namespace MR
