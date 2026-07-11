#include "Game/Util/DrawUtil.hpp"
#include "Game/Scene/GameSceneFunction.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/Color.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "JSystem/J3DGraphBase/J3DShape.hpp"
#include "JSystem/J3DGraphBase/J3DSys.hpp"
#include "JSystem/JGeometry/TMatrix.hpp"
#include "JSystem/JUtility/JUTVideo.hpp"
#include "revolution/gx/GXCull.h"
#include "revolution/gx/GXEnum.h"
#include "revolution/gx/GXGeometry.h"
#include "revolution/gx/GXPixel.h"
#include "revolution/gx/GXStruct.h"
#include "revolution/gx/GXTransform.h"
#include "revolution/mtx.h"
#include <revolution/gx/GXRegs.h>

/* 0x805E0BE0 */ static u8 sImgTexObj[] = {0,0xFF,0,0xFF,0,0xFF,0,0xFF,0,0xFF,0,0xFF,0,0xFF,0,0xFF,0,0xFF,0,0xFF,0,0xFF,0,0xFF,0,0xFF,0,0xFF,0,0xFF,0,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
/* 0x806B2610 */ static bool sIsReinitTextureCache;
/* 0x806B2614 */ static GXTexCacheSize sReinitTextureCacheSize;
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
        GXSetAlphaUpdate(0);
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
        GXSetZMode(0, GX_ALWAYS, 0);

        Mtx44 projMtx;
        C_MTXOrtho(projMtx, 0.0f, -MR::getScreenHeight(), 0.0f, MR::getScreenWidth(), -cViewDistance, cViewDistance);
        GXSetProjection(projMtx, GX_ORTHOGRAPHIC);

        Mtx viewMtx;
        PSMTXIdentity(viewMtx);
        PSMTXCopy(viewMtx, j3dSys.mViewMtx);
    }

    // ! regswap
    void setDefaultViewportAndScissor() {
        s32 width = MR::getFrameBufferWidth();
        s32 height = MR::getScreenHeight();

        GXSetViewport(0.0f, 0.0f, width, height, 0.0f, 1.0f);
        GXSetScissor(0, 0, width, height);
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
        GXSetZMode(0, GX_NEVER, 0);
    }

    // ! regswap
    void clearZBuffer() {
        GXRenderModeObj* pRenderModeObj = JUTVideo::getManager()->getRenderMode();
        u16 height = pRenderModeObj->efbHeight;
        u16 width = pRenderModeObj->fbWidth;

        Mtx44 projMtx;
        C_MTXOrtho(projMtx, 0.0f, width, 0.0f, height, 0.0f, 1.0f);
        GXSetProjection(projMtx, GX_ORTHOGRAPHIC);
        MR::setDefaultViewportAndScissor();

        TMtx34f mtxImm;
        mtxImm.identity();
        GXLoadPosMtxImm(mtxImm, 0);

        GXSetCurrentMtx(0);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_U16, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_POS_XYZ, GX_U8, 0);
        GXSetNumChans(0);
        GXSetChanCtrl(GX_COLOR0A0, 0, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
        GXSetChanCtrl(GX_COLOR1A1, 0, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 60, 0, 125);
        GXInitTexObj(&clear_z_tobj, sImgTexObj, 4, 4, GX_TF_Z24X8, GX_REPEAT, GX_REPEAT, 0);
        GXInitTexObjLOD(&clear_z_tobj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, 0, 0, GX_ANISO_1);
        GXLoadTexObj(&clear_z_tobj, GX_TEXMAP0);
        GXSetNumTevStages(1);
        GXColor tev0;
        tev0.r = 0;
        tev0.g = 0;
        tev0.b = 0;
        tev0.a = 0;
        GXSetTevColor(GX_TEVREG0, tev0);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
        GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
        GXSetZTexture(GX_ZT_REPLACE, GX_TF_Z24X8, 0);
        GXSetZCompLoc(0);
        GXSetBlendMode(GX_BM_NONE, GX_BL_ZERO, GX_BL_ZERO, GX_LO_NOOP);
        GXSetColorUpdate(0);
        GXSetAlphaUpdate(0);
        GXSetZMode(1, GX_ALWAYS, 1);
        GXSetCullMode(GX_CULL_BACK);
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);

        GX_WRITE_U16(0);
        GX_WRITE_U16(0);
        GX_WRITE_U8(0);
        GX_WRITE_U8(0);
        GX_WRITE_U16(width);
        GX_WRITE_U16(0);
        GX_WRITE_U8(1);
        GX_WRITE_U8(0);
        GX_WRITE_U16(width);
        GX_WRITE_U16(height);
        GX_WRITE_U8(1);
        GX_WRITE_U8(1);
        GX_WRITE_U16(0);
        GX_WRITE_U16(height);
        GX_WRITE_U8(0);
        GX_WRITE_U8(1);

        GXSetZTexture(GX_ZT_DISABLE, GX_TF_Z24X8, 0);
        GXSetZCompLoc(1);
        GXSetColorUpdate(1);
    }

    // void clearAlphaBuffer(u8 a1) {}
    // void clearAlphaBuffer(u8 a1, const TVec2f& a2, const TVec2f& a3) {}

    // ! swapped/missing instructions
    void fillScreenSetup(const GXColor& rColor) {
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_U16, 0);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);

        TMtx34f mtxImm;
        mtxImm.identity();
        GXLoadPosMtxImm(mtxImm, 0);

        s32 height = MR::getScreenHeight();
        s32 width = MR::getFrameBufferWidth();

        Mtx44 projMtx;
        C_MTXOrtho(projMtx, 0.0f, width, 0.0f, height, 0.0f, 1.0f);
        GXSetProjection(projMtx, GX_ORTHOGRAPHIC);

        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, 0, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
        GXSetChanCtrl(GX_COLOR1A1, 0, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
        GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
        GXSetZMode(0, GX_LEQUAL, 0);
        GXSetZCompLoc(0);
        GXSetCullMode(GX_CULL_NONE);
        GXSetDither(0);
        GXSetNumTexGens(0);
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
        GXSetTevColor(GX_TEVREG0, rColor);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
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
