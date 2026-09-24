#include "Game/Scene/GameSceneFunction.hpp"
#include "Game/Screen/ScreenAlphaCapture.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/Color.hpp"
#include "Game/Util/DrawUtil.hpp"
#include "Game/Util/MtxUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include <JSystem/J3DGraphAnimator/J3DJoint.hpp>
#include <JSystem/J3DGraphAnimator/J3DModel.hpp>
#include <JSystem/J3DGraphBase/J3DMaterial.hpp>
#include <JSystem/J3DGraphBase/J3DPacket.hpp>
#include <JSystem/J3DGraphBase/J3DShape.hpp>
#include <JSystem/J3DGraphBase/J3DSys.hpp>
#include <JSystem/JGeometry/TMatrix.hpp>
#include <JSystem/JUtility/JUTVideo.hpp>
#include <revolution/gx/GXBump.h>
#include <revolution/gx/GXCull.h>
#include <revolution/gx/GXEnum.h>
#include <revolution/gx/GXFrameBuf.h>
#include <revolution/gx/GXGeometry.h>
#include <revolution/gx/GXPixel.h>
#include <revolution/gx/GXRegs.h>
#include <revolution/gx/GXStruct.h>
#include <revolution/gx/GXTransform.h>
#include <revolution/gx/GXVert.h>

static u8 sTexImgObj[] = {0,    0xFF, 0,    0xFF, 0,    0xFF, 0,    0xFF, 0,    0xFF, 0,    0xFF, 0,    0xFF, 0,    0xFF,
                          0,    0xFF, 0,    0xFF, 0,    0xFF, 0,    0xFF, 0,    0xFF, 0,    0xFF, 0,    0xFF, 0,    0xFF,
                          0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                          0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
static bool sIsReinitTextureCache = true;
static GXTexCacheSize sReinitTextureCacheSize = GX_TEXCACHE_128K;

void DrawUtil_FORCE_MATCH_SDATA2() {
    (void)1.0f;
    (void)0.0f;
    (void)0.5f;
    (void)-1.0f;
    (void)-10000.0f;
    (void)10000.0f;
    (void)608.0f;
    (void)-1000.0f;
    (void)1000.0f;
    (void)-0.5f;
    (void)-30000.0f;
    (void)30000.0f;
}

namespace {
    GXTexObj clear_z_tobj;

    const f32 cNearZ = -10000.0f;
    const f32 cFarZ = 10000.0f;

    inline s32 getScreenHeightInline() {
        return MR::getScreenHeight();
    }
};  // namespace

const JUTTexture* mShadowTex;
TVec3f mShadowVec;

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
        C_MTXOrtho(projMtx, 0.0f, -MR::getScreenHeight(), 0.0f, MR::getScreenWidth(), ::cNearZ, ::cFarZ);
        GXSetProjection(projMtx, GX_ORTHOGRAPHIC);

        Mtx viewMtx;
        PSMTXIdentity(viewMtx);
        PSMTXCopy(viewMtx, j3dSys.mViewMtx);
    }

    void setDefaultViewportAndScissor() {
        s32 width = MR::getFrameBufferWidth();
        s32 height = ::getScreenHeightInline();
        GXSetViewport(0.0f, 0.0f, width, height, 0.0f, 1.0f);
        GXSetScissor(0, 0, width, height);
    }

    void loadProjectionMtxFor2D() {
        J2DOrthoGraphSimple orthoGraph;
        orthoGraph.setPort();
    }

    void setupDrawForNW4RLayout(f32 scale, bool) {
        f32 height = MR::getScreenHeight();
        f32 width = 608.0f;
        f32 halfWidth = width / 2.0f;
        height *= 0.5f;
        halfWidth *= scale;
        height *= scale;
        Mtx44 projMtx;
        C_MTXOrtho(projMtx, height, -height, -halfWidth, halfWidth, -1000.0f, 1000.0f);
        GXSetProjection(projMtx, GX_ORTHOGRAPHIC);
        GXSetCullMode(GX_CULL_NONE);
        GXSetZMode(GX_FALSE, GX_NEVER, GX_FALSE);
    }

    void clearZBuffer() {
        JUTVideo* pJUTVideo = JUTVideo::getManager();
        u16 width = pJUTVideo->getFbWidth();
        u16 height = pJUTVideo->getEfbHeight();
        Mtx44 projMtx;
        C_MTXOrtho(projMtx, 0.0f, height, 0.0f, width, 0.0f, 1.0f);
        GXSetProjection(projMtx, GX_ORTHOGRAPHIC);
        MR::setDefaultViewportAndScissor();
        TMtx34f mtxImm;
        mtxImm.identity();
        GXLoadPosMtxImm(mtxImm, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_U16, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_POS_XYZ, GX_U8, 0);
        GXSetNumChans(0);
        GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
        GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
        GXInitTexObj(&::clear_z_tobj, sTexImgObj, 4, 4, GX_TF_Z24X8, GX_REPEAT, GX_REPEAT, GX_FALSE);
        GXInitTexObjLOD(&::clear_z_tobj, GX_NEAR, GX_NEAR, 0.0f, 0.0f, 0.0f, GX_FALSE, GX_FALSE, GX_ANISO_1);
        GXLoadTexObj(&::clear_z_tobj, GX_TEXMAP0);
        GXSetNumTevStages(1);
        GXColor tev0;
        tev0.r = 0;
        tev0.g = 0;
        tev0.b = 0;
        tev0.a = 0;
        GXSetTevColor(GX_TEVREG0, tev0);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_C0);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_A0);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
        GXSetZTexture(GX_ZT_REPLACE, GX_TF_Z24X8, 0);
        GXSetZCompLoc(GX_FALSE);
        GXSetBlendMode(GX_BM_NONE, GX_BL_ZERO, GX_BL_ZERO, GX_LO_NOOP);
        GXSetColorUpdate(GX_FALSE);
        GXSetAlphaUpdate(GX_FALSE);
        GXSetZMode(GX_TRUE, GX_ALWAYS, GX_TRUE);
        GXSetCullMode(GX_CULL_BACK);

        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        {
            GXPosition2u16(0, 0);
            GXTexCoord2u8(0, 0);

            GXPosition2u16(width, 0);
            GXTexCoord2u8(1, 0);

            GXPosition2u16(width, height);
            GXTexCoord2u8(1, 1);

            GXPosition2u16(0, height);
            GXTexCoord2u8(0, 1);
        }
        GXEnd();

        GXSetZTexture(GX_ZT_DISABLE, GX_TF_Z24X8, 0);
        GXSetZCompLoc(GX_TRUE);
        GXSetColorUpdate(GX_TRUE);
    }

    void clearAlphaBuffer(u8 alpha) {
        JUTVideo* pVideo = JUTVideo::getManager();
        f32 height = pVideo->getEfbHeight();
        f32 width = pVideo->getFbWidth();
        clearAlphaBuffer(alpha, TVec2f(0.0f, 0.0f), TVec2f(width, height));
    }

    void clearAlphaBuffer(u8 alpha, const TVec2f& rPosition, const TVec2f& rSize) {
        u16 width = rSize.x;
        u16 height = rSize.y;
        Mtx44 projection;
        C_MTXOrtho(projection, 0.0f, ::getScreenHeightInline(), 0.0f, getFrameBufferWidth(), -1.0f, 1.0f);
        GXSetProjection(projection, GX_ORTHOGRAPHIC);
        GXSetCurrentMtx(GX_PNMTX0);
        Mtx matrix;
        PSMTXIdentity(matrix);
        GXLoadPosMtxImm(matrix, GX_PNMTX0);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
        GXSetNumTexGens(0);
        GXSetNumTevStages(1);
        GXSetTevDirect(GX_TEVSTAGE0);
        GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
        GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
        GXSetCoPlanar(GX_FALSE);
        GXSetClipMode(GX_CLIP_DISABLE);
        GXSetCullMode(GX_CULL_NONE);
        GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
        GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
        GXSetBlendMode(GX_BM_NONE, GX_BL_ZERO, GX_BL_ZERO, GX_LO_COPY);
        GXSetColorUpdate(GX_FALSE);
        GXSetAlphaUpdate(GX_TRUE);
        GXSetDstAlpha(GX_TRUE, alpha);

        GXBegin(GX_TRIANGLES, GX_VTXFMT0, 6);
        GXPosition3f32(rPosition.x, rPosition.y, 0.0f);
        GXPosition3f32(width + rPosition.x, rPosition.y, 0.0f);
        GXPosition3f32(width + rPosition.x, height + rPosition.y, 0.0f);
        GXPosition3f32(rPosition.x, rPosition.y, 0.0f);
        GXPosition3f32(width + rPosition.x, height + rPosition.y, 0.0f);
        GXPosition3f32(rPosition.x, height + rPosition.y, 0.0f);
        GXEnd();

        GXSetDstAlpha(GX_FALSE, 0);
        GXSetClipMode(GX_CLIP_ENABLE);
    }

    void fillScreenSetup(const GXColor& rColor) {
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_U16, 0);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        TMtx34f mtxImm;
        mtxImm.identity();
        GXLoadPosMtxImm(mtxImm, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        Mtx44 projMtx;
        C_MTXOrtho(projMtx, 0.0f, ::getScreenHeightInline(), 0.0f, MR::getFrameBufferWidth(), -1.0f, 1.0f);
        GXSetProjection(projMtx, GX_ORTHOGRAPHIC);
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
        GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
        GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
        GXSetZMode(GX_FALSE, GX_LEQUAL, GX_FALSE);
        GXSetZCompLoc(GX_FALSE);
        GXSetCullMode(GX_CULL_NONE);
        GXSetDither(GX_FALSE);
        GXSetNumTexGens(0);
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
        GXSetTevColor(GX_TEVREG0, rColor);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    }

    void fillScreenArea(const TVec2s& rMin, const TVec2s& rMax) {
        GXBegin(GX_QUADS, GX_VTXFMT0, 4);
        {
            u16 maxX = rMax.x;
            u16 minX = rMin.x;
            u16 minY = rMin.y;
            u16 maxY = rMax.y;

            GXPosition2u16(minX, minY);
            GXPosition2u16(maxX, minY);
            GXPosition2u16(maxX, maxY);
            GXPosition2u16(minX, maxY);
        }
        GXEnd();
    }

    void fillScreen(const GXColor& color) {
        fillScreenSetup(color);
        u16 width = getFrameBufferWidth();
        u16 height = getScreenHeight();
        fillScreenArea(TVec2s(0, 0), TVec2s(width, height));
    }

    void setupShadowVolumeDraw() {
        GXSetTevDirect(GX_TEVSTAGE0);
        GXSetNumIndStages(0);
        GXSetNumTexGens(0);
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
        GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
        GXSetChanMatColor(GX_COLOR0A0, Color8(0));
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
        GXSetZCompLoc(GX_FALSE);
        GXSetZMode(GX_TRUE, GX_GEQUAL, GX_FALSE);
        GXSetClipMode(GX_CLIP_ENABLE);
    }

    void drawSimpleModel(J3DModelData* pModelData) {
        J3DMaterial* pMaterial = pModelData->getJointNodePointer(0)->mMesh;
        j3dSys.setVtxPos(pModelData->mVertexData.mVtxPosArray);
        j3dSys.setVtxNrm(pModelData->mVertexData.mVtxNrmArray);
        j3dSys.setVtxCol(pModelData->mVertexData.mVtxColorArray[0]);

        while (pMaterial != nullptr) {
            pMaterial->mShape->simpleDraw();
            pMaterial = pMaterial->mNext;
        }
    }

    void fillSilhouetteColor() {
        MR::captureScreenAlpha(0);
        MR::loadScreenAlphaTexture(0, GX_TEXMAP0);
        Mtx44 projMtx;
        C_MTXOrtho(projMtx, 0.0f, MR::getScreenHeight(), 0.0f, MR::getScreenWidth(), -1.0f, 1.0f);
        GXSetProjection(projMtx, GX_ORTHOGRAPHIC);
        GXSetTevDirect(GX_TEVSTAGE0);
        GXSetNumIndStages(0);
        TMtx34f mtxImm;
        mtxImm.identity();
        GXLoadPosMtxImm(mtxImm, GX_PNMTX0);
        GXSetCurrentMtx(GX_PNMTX0);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_U16, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_POS_XYZ, GX_F32, 0);
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
        GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetTevKColor(GX_KCOLOR0, Color8(0, 0, 0, 1));
        GXSetTevKAlphaSel(GX_TEVSTAGE0, GX_TEV_KASEL_K0_A);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_TEXA, GX_CA_KONST, GX_CA_A0, GX_CA_ZERO);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_COMP_RGB8_GT, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        GXSetAlphaCompare(GX_GREATER, 0, GX_AOP_AND, GX_ALWAYS, 0);
        GXSetZMode(GX_TRUE, GX_ALWAYS, GX_FALSE);
        GXSetZCompLoc(GX_FALSE);
        GXSetCullMode(GX_CULL_NONE);
        GXSetDither(GX_FALSE);
        GXSetColorUpdate(GX_TRUE);
        GXSetAlphaUpdate(GX_TRUE);
        GXSetDstAlpha(GX_TRUE, 0);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
        GXSetTevColor(GX_TEVREG0, Color8(0x00000080));

        GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 4);
        {
            u16 width = MR::getScreenWidth();
            u16 height = MR::getScreenHeight();

            GXPosition2u16(0, 0);
            GXTexCoord2f32(0.0f, 0.0f);

            GXPosition2u16(width, 0);
            GXTexCoord2f32(1.0f, 0.0f);

            GXPosition2u16(0, height);
            GXTexCoord2f32(0.0f, 1.0f);

            GXPosition2u16(width, height);
            GXTexCoord2f32(1.0f, 1.0f);
        }
        GXEnd();

        GXSetDstAlpha(GX_FALSE, 0);
        GXSetAlphaUpdate(GX_FALSE);
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

    void setMarioShadowVec(const TVec3f& rVec) {
        mShadowVec = rVec;
    }

    void loadTexProjectionMtx(u32 id) {
        TProj3f cameraProjection = MR::getCameraProjectionMtx();
        cameraProjection.mMtx[2][0] = 0.0f;
        cameraProjection.mMtx[2][1] = 0.0f;
        cameraProjection.mMtx[2][2] = -1.0f;
        cameraProjection.mMtx[2][3] = 0.0f;
        cameraProjection.mMtx[3][0] = 0.0f;
        cameraProjection.mMtx[3][1] = 0.0f;
        cameraProjection.mMtx[3][2] = 0.0f;
        cameraProjection.mMtx[3][3] = 1.0f;
        TPos3f matrix;
        MR::multMtx(matrix, MR::getCameraViewMtx(), cameraProjection);
        TPos3f mat;
        mat.identity();
        mat[0][0] = 0.5f;
        mat[0][2] = 0.5f;
        mat[1][1] = -0.5f;
        mat[1][2] = 0.5f;
        MR::multMtx(matrix, matrix, mat);
        GXLoadTexMtxImm(matrix, id, GX_MTX3x4);
    }

    void nonFilteredCapture(JUTTexture* pTexture, s16 width, s16 height) {
        GXRenderModeObj* pRenderModeObj = JUTVideo::getManager()->getRenderMode();
        GXSetCopyFilter(GX_FALSE, pRenderModeObj->sample_pattern, GX_FALSE, pRenderModeObj->vfilter);
        pTexture->capture(width, height, (GXTexFmt)pTexture->getFormat(), false, 0);
        GXSetCopyFilter(GX_FALSE, pRenderModeObj->sample_pattern, GX_TRUE, pRenderModeObj->vfilter);
    }

    void simpleDraw(J3DModel* pModel, J3DMaterial* pMaterial) {
        if (pMaterial != nullptr && pMaterial->mShape != nullptr && !pMaterial->mShape->checkFlag(1)) {
            J3DShape::resetVcdVatCache();
            J3DMatPacket* pMatPacket = pModel->getMatPacket(pMaterial->mIndex);
            J3DShapePacket* pShapePacket = pModel->getShapePacket(pMaterial->mShape->mIndex);
            j3dSys.setMatPacket(pMatPacket);
            pMatPacket->callDL();
            pShapePacket->getShape()->loadPreDrawSetting();

            if (pShapePacket->isValidDisplayListObj()) {
                pShapePacket->callDL();
            }

            pShapePacket->drawFast();
        }
    }

    void activateGameSceneDraw3D() {
        GameSceneFunction::activateDraw3D();
    }

    void deactivateGameSceneDraw3D() {
        GameSceneFunction::deactivateDraw3D();
    }
};  // namespace MR

J2DOrthoGraphSimple::J2DOrthoGraphSimple() : J2DOrthoGraph(0.0f, 0.0f, MR::getFrameBufferWidth(), ::getScreenHeightInline(), -30000.0f, 30000.0f) {
    TBox2f bounds(0.0f, 0.0f, MR::getScreenWidth(), MR::getScreenHeight());
    setOrtho(0.0f, 0.0f, bounds.getWidth(), bounds.getHeight(), -30000.0f, 30000.0f);
}

void J2DOrthoGraphSimple::setPort() {
    J2DOrthoGraph::setPort();
}
