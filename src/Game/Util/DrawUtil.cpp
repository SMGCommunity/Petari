#include "Game/Util/DrawUtil.hpp"
#include "Game/Camera/CameraContext.hpp"
#include "Game/Scene/GameSceneFunction.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/CameraUtil.hpp"
#include <revolution/gx/GXRegs.h>

const JUTTexture* mShadowTex;
TVec3f mShadowVec;

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

    // TODO
    // The TPos3f here is actually a Mtx44
    void loadTexProjectionMtx(u32 id) {
        TPos3f cameraProjection;
        cameraProjection.setInline(*MR::getCameraProjectionMtx());
        cameraProjection.mMtx[2][0] = 0.0f;
        cameraProjection.mMtx[2][1] = 0.0f;
        cameraProjection.mMtx[2][2] = -1.0f;
        cameraProjection.mMtx[2][3] = 0.0f;
        cameraProjection.mMtx[3][0] = 0.0f;
        cameraProjection.mMtx[3][1] = 0.0f;
        cameraProjection.mMtx[3][2] = 0.0f;
        cameraProjection.mMtx[3][3] = 1.0f;
        Mtx matrix;
        PSMTXConcat(cameraProjection.toMtxPtr(), MR::getCameraViewMtx(), matrix);
        TMtx34f mat;
        mat.identity();
        mat[0][0] = 0.5f;
        mat[0][2] = 0.5f;
        mat[1][1] = -0.5f;
        mat[1][2] = 0.5f;
        PSMTXConcat(mat, matrix, matrix);
        GXLoadTexMtxImm(matrix, id, GX_MTX3x4);
    }

    void activateGameSceneDraw3D() {
        GameSceneFunction::activateDraw3D();
    }

    void deactivateGameSceneDraw3D() {
        GameSceneFunction::deactivateDraw3D();
    }
};  // namespace MR
