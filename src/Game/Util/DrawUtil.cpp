#include "Game/Util/DrawUtil.hpp"
#include "Game/Scene/GameSceneFunction.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "JSystem/J3DGraphBase/J3DShape.hpp"
#include "JSystem/J3DGraphBase/J3DSys.hpp"
#include "revolution/gx/GXCull.h"
#include "revolution/gx/GXEnum.h"
#include "revolution/gx/GXPixel.h"
#include "revolution/gx/GXTransform.h"
#include "revolution/mtx.h"
#include <revolution/gx/GXRegs.h>

static bool sIsReinitTextureCache;
static GXTexCacheSize sReinitTextureCacheSize;
const JUTTexture* mShadowTex;
TVec3f mShadowVec;

namespace {
    GXTexObj clear_z_tobj;

    const f32 cViewDistance = 10000.0;
}; // namespace

namespace MR {
    void drawInit() {
        j3dSys.offFlag(2);
        j3dSys.drawInit();
        j3dSys.setTexCacheRegion(GX_TEXCACHE_128K);
    }

    void reinitGX() {
        j3dSys.reinitGX();
        GXSetAlphaUpdate(GX_FALSE);
        J3DShape::resetVcdVatCache();
        if (sIsReinitTextureCache) {
            j3dSys.setTexCacheRegion(sReinitTextureCacheSize);
        }
    }

    void resetTextureCacheSize() {
        if (sIsReinitTextureCache) {
            j3dSys.setTexCacheRegion(sReinitTextureCacheSize);
        }
    }

    void loadViewMtxFor2DModel() {
        Mtx viewMtx;
        PSMTXIdentity(viewMtx);
        PSMTXCopy(viewMtx, j3dSys.mViewMtx);
    }

    void drawInitFor2DModel() {
        MR::reinitGX();
        GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);

        Mtx44 projMtx;
        C_MTXOrtho(projMtx, 0.0f, -MR::getScreenHeight(), 0.0f, MR::getScreenWidth(), -cViewDistance, cViewDistance);
        GXSetProjection(projMtx, GX_ORTHOGRAPHIC);

        Mtx viewMtx;
        PSMTXIdentity(viewMtx);
        PSMTXCopy(viewMtx, j3dSys.mViewMtx);
    }

    // ! regswap
    void setDefaultViewportAndScissor() {
        s32 screenWidth = MR::getFrameBufferWidth();
        s32 screenHeight = MR::getScreenHeight();

        GXSetViewport(0.0f, 0.0f, screenWidth, screenHeight, 0.0f, 1.0f);
        GXSetScissor(0, 0, screenWidth, screenHeight);
    }

    void loadProjectionMtxFor2D() {
        J2DOrthoGraphSimple orthoGraph;
        orthoGraph.J2DOrthoGraph::setPort();
    }

    // ! unfinished
    void setupDrawForNW4RLayout(f32 a1, bool) {
        Mtx44 projMtx;
        f32 v1 = MR::getScreenHeight() * 0.5f * a1;
        f32 v2 = 608.0f * 0.5f * a1;
        C_MTXOrtho(projMtx, -v1, v1, -v2, v2, -cViewDistance, cViewDistance);
        GXSetProjection(projMtx, GX_ORTHOGRAPHIC);
        GXSetCullMode(GX_CULL_NONE);
        GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    }

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
        TProj3f cameraProjection;
        cameraProjection.setInline(MR::getCameraProjectionMtx());
        cameraProjection.mMtx[2][0] = 0.0f;
        cameraProjection.mMtx[2][1] = 0.0f;
        cameraProjection.mMtx[2][2] = -1.0f;
        cameraProjection.mMtx[2][3] = 0.0f;
        cameraProjection.mMtx[3][0] = 0.0f;
        cameraProjection.mMtx[3][1] = 0.0f;
        cameraProjection.mMtx[3][2] = 0.0f;
        cameraProjection.mMtx[3][3] = 1.0f;
        Mtx matrix;
        PSMTXConcat(cameraProjection.toMtx44Ptr(), MR::getCameraViewMtx(), matrix);
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
