#include "Game/Util/DrawUtil.hpp"
#include "Game/Scene/GameSceneFunction.hpp"
#include "Game/Screen/ScreenAlphaCapture.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/Color.hpp"
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
static bool sIsReinitTextureCache;
static GXTexCacheSize sReinitTextureCacheSize;

const JUTTexture* mShadowTex;
TVec3f mShadowVec;

namespace {
    GXTexObj clear_z_tobj;

    const f32 cNearZ = -10000.0f;
    const f32 cFarZ = 10000.0f;

    inline s32 getScreenHeightInline() {
        return MR::getScreenHeight();
    }
};  // namespace

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
        s32 height = getScreenHeightInline();
        GXSetViewport(0.0f, 0.0f, width, height, 0.0f, 1.0f);
        GXSetScissor(0, 0, width, height);
    }

    void loadProjectionMtxFor2D() {
        J2DOrthoGraphSimple orthoGraph;
        orthoGraph.setPort();
    }

    // ! unfinished - https://decomp.me/scratch/VkVF2
    void setupDrawForNW4RLayout(f32 a1, bool) {
        f32 v1 = MR::getScreenHeight() * 0.5f * a1;
        f32 v2 = 608.0f * 0.5f * a1;

        Mtx44 projMtx;
        C_MTXOrtho(projMtx, v1, -v1, -v2, v2, cNearZ, cFarZ);
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
        GXLoadPosMtxImm(mtxImm, 0);
        GXSetCurrentMtx(0);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_U16, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_POS_XYZ, GX_U8, 0);
        GXSetNumChans(0);
        GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
        GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 60, GX_FALSE, 125);
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
        GXPosition2u16(0, 0);
        GXPosition2u8(0, 0);
        GXPosition2u16(width, 0);
        GXPosition2u8(1, 0);
        GXPosition2u16(width, height);
        GXPosition2u8(1, 1);
        GXPosition2u16(0, height);
        GXPosition2u8(0, 1);
        GXEnd();
        GXSetZTexture(GX_ZT_DISABLE, GX_TF_Z24X8, 0);
        GXSetZCompLoc(GX_TRUE);
        GXSetColorUpdate(GX_TRUE);
    }

    void fillScreenSetup(const GXColor& rColor) {
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_U16, 0);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        TMtx34f mtxImm;
        mtxImm.identity();
        GXLoadPosMtxImm(mtxImm, 0);
        GXSetCurrentMtx(0);
        Mtx44 projMtx;
        C_MTXOrtho(projMtx, 0.0f, getScreenHeightInline(), 0.0f, MR::getFrameBufferWidth(), -1.0f, 1.0f);
        GXSetProjection(projMtx, GX_ORTHOGRAPHIC);
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
        GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
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
        u16 maxX = rMax.x;
        u16 minX = rMin.x;
        u16 minY = rMin.y;
        u16 maxY = rMax.y;
        GXPosition2u16(minX, minY);
        GXPosition2u16(maxX, minY);
        GXPosition2u16(maxX, maxY);
        GXPosition2u16(minX, maxY);
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
        GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
        GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
        GXSetChanMatColor(GX_COLOR0A0, Color8(0));
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
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

    // ! stack issue - https://decomp.me/scratch/5hwXh
    void fillSilhouetteColor() {
        MR::captureScreenAlpha(0);
        MR::loadScreenAlphaTexture(0, GX_TEXMAP0);
        Mtx44 projMtx;
        C_MTXOrtho(projMtx, 0.0f, MR::getScreenHeight(), 0.0f, MR::getScreenWidth(), ::cNearZ, ::cFarZ);
        GXSetProjection(projMtx, GX_ORTHOGRAPHIC);
        GXSetTevDirect(GX_TEVSTAGE0);
        GXSetNumIndStages(0);
        TMtx34f mtxImm;
        mtxImm.identity();
        GXLoadPosMtxImm(mtxImm, 0);
        GXSetCurrentMtx(0);
        GXClearVtxDesc();
        GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
        GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_U16, 0);
        GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_POS_XYZ, GX_F32, 0);
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
        GXSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, 0, GX_DF_NONE, GX_AF_NONE);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 60, GX_FALSE, 125);
        GXSetNumTevStages(1);
        GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
        GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
        GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
        GXColor kcolor0;
        kcolor0.r = 0;
        kcolor0.g = 0;
        kcolor0.b = 0;
        kcolor0.a = 1;
        GXSetTevKColor(GX_KCOLOR0, kcolor0);
        GXSetTevKAlphaSel(GX_TEVSTAGE0, GX_TEV_KASEL_K0_A);
        GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_TEXA, GX_CA_KONST, GX_CA_A0, GX_CA_ZERO);
        GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_COMP_RGB8_GT, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
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
        u16 width = MR::getScreenWidth();
        u16 height = MR::getScreenHeight();
        GXPosition2u16(0, 0);
        GXPosition2f32(0.0f, 0.0f);
        GXPosition2u16(width, 0);
        GXPosition2f32(1.0f, 0.0f);
        GXPosition2u16(0, height);
        GXPosition2f32(0.0f, 1.0f);
        GXPosition2u16(width, height);
        GXPosition2f32(1.0f, 1.0f);
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

    void nonFilteredCapture(JUTTexture* pTexture, s16 width, s16 height) {
        GXRenderModeObj* pRenderModeObj = JUTVideo::getManager()->getRenderMode();
        GXSetCopyFilter(GX_FALSE, pRenderModeObj->sample_pattern, 0, pRenderModeObj->vfilter);
        pTexture->capture(width, height, (GXTexFmt)pTexture->getFormat(), false, 0);
        GXSetCopyFilter(GX_FALSE, pRenderModeObj->sample_pattern, 1, pRenderModeObj->vfilter);
    }

    void simpleDraw(J3DModel* pModel, J3DMaterial* pMaterial) {
        if (pMaterial && pMaterial->mShape && !pMaterial->mShape->checkFlag(1)) {
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

void J2DOrthoGraphSimple::setPort() {
    J2DOrthoGraph::setPort();
}
