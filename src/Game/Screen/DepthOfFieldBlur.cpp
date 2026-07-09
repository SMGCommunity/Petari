#include "Game/Screen/DepthOfFieldBlur.hpp"
#include "Game/Screen/CopyFilterNegater.hpp"
#include "Game/Screen/ImageEffectLocalUtil.hpp"
#include "Game/Screen/ImageEffectResource.hpp"
#include "Game/Screen/ImageEffectSystemHolder.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "Game/Util/DrawUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include <JSystem/JUtility/JUTTexture.hpp>

namespace {
    // static const s32 sFirstBlurCount = _;
    // static const f32 sFirstBlurRadius = _;
    // static const f32 sBlendRate = _;
    static const f32 sViewDistance = 10000.0f;
    static const s8 sScaleExp = 1;
};  // namespace

DepthOfFieldBlur::DepthOfFieldBlur(const char* pName)
    : ImageEffectBase(pName), _14(), _18(), _1C(), _20(), _24(), _28(), mIntensity(1.0f), mBlurMaxDist(248), mBlurMinDist(242) {
    MR::connectToSceneImageEffect(this);
    MR::createImageEffectSystemHolder();
    MR::getImageEffectResource()->createDOFTexture();
    _14 = MR::getImageEffectResource()->_0;
    _18 = MR::getImageEffectResource()->_10;
    _1C = MR::getImageEffectResource()->_4;
    _20 = MR::getImageEffectResource()->_8;
    _24 = MR::getImageEffectResource()->_C;
    _28 = new JUTTexture(128, 2, GX_TF_I8);
    _28->mWrapS = GX_CLAMP;
    _28->mWrapT = GX_CLAMP;
    _28->mMinType = GX_LINEAR;
    _28->mMagType = GX_LINEAR;
}

void DepthOfFieldBlur::draw() const {
    if (!isSomething()) {
        return;
    }

    CopyFilterNegater negator;
    ImageEffectLocalUtil::capture(_18, 4, 0, GX_TF_RGB565, false, 0);
    ImageEffectLocalUtil::capture(_1C, 1, 0, GX_TF_RGB565, true, 0);
    drawZAlphaTex();
    ImageEffectLocalUtil::capture(_28, 1, 0, GX_CTF_R8, false, 0);
    ImageEffectLocalUtil::setupDrawTexture();
    createBlurTexture();
    ImageEffectLocalUtil::capture(_20, 1, 0, GX_TF_Z8, true, 0);
    drawFinal();
    MR::loadProjectionMtx();
    MR::loadViewMtx();
}

void DepthOfFieldBlur::drawZAlphaTex() const {
    Mtx44 projectionMtx;
    C_MTXOrtho(projectionMtx, 0.0f, JUTVideo::getManager()->getEfbHeight(), 0.0f, JUTVideo::getManager()->getFbWidth(), -::sViewDistance,
               ::sViewDistance);
    GXSetProjection(projectionMtx, GX_ORTHOGRAPHIC);
    MR::loadViewMtxFor2DModel();
    GXClearVtxDesc();
    GXSetVtxAttrFmt(GX_VTXFMT1, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT1, GX_VA_CLR0, GX_POS_XY, GX_S8, 0);
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);

    TMtx34f posMtx;
    posMtx.identity();
    GXLoadPosMtxImm(posMtx.toMtxPtr(), GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);
    GXSetCullMode(GX_CULL_NONE);
    GXSetClipMode(GX_CLIP_ENABLE);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(0);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXLoadTexMtxImm(posMtx.toMtxPtr(), GX_TEXMTX0, GX_MTX2x4);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetNumIndStages(0);
    GXSetTevDirect(GX_TEVSTAGE0);
    GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, GXColor());
    GXSetColorUpdate(GX_TRUE);
    GXSetAlphaUpdate(GX_FALSE);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
    GXSetDither(GX_FALSE);
    GXSetBlendMode(GX_BM_NONE, GX_BL_ZERO, GX_BL_ZERO, GX_LO_CLEAR);
    GXSetLineWidth(12, GX_TO_ZERO);
    GXBegin(GX_LINESTRIP, GX_VTXFMT1, 4);
    {
        u8 blurMaxDist = 248;

        if (mBlurMaxDist >= 0) {
            blurMaxDist = mBlurMaxDist;
        }

        u8 blurMinDist = 242;

        if (mBlurMinDist >= 0) {
            blurMinDist = mBlurMinDist;
        }

        if (blurMinDist >= blurMaxDist) {
            GXPosition3f32(0.0f, 1.0f, 0.0f);
            GXPosition3u8(0, 0, 0);
            GXPosition3f32(blurMinDist * 0.5f, 1.0f, 0.0f);
            GXPosition3u8(0, 0, 0);
            GXPosition3f32(blurMaxDist * 0.5f, 1.0f, 0.0f);
            GXPosition3u8(255, 255, 255);
            GXPosition3f32(128.0f, 1.0f, 0.0f);
            GXPosition3u8(255, 255, 255);
        } else {
            GXPosition3f32(128.0f, 1.0f, 0.0f);
            GXPosition3u8(0, 0, 0);
            GXPosition3f32(blurMinDist * 0.5f, 1.0f, 0.0f);
            GXPosition3u8(0, 0, 0);
            GXPosition3f32(blurMaxDist * 0.5f, 1.0f, 0.0f);
            GXPosition3u8(255, 255, 255);
            GXPosition3f32(0.0f, 1.0f, 0.0f);
            GXPosition3u8(255, 255, 255);
        }
    }
    GXEnd();
}

void DepthOfFieldBlur::createBlurTexture() const {
    ImageEffectLocalUtil::drawTexture(_1C, 4, 0, 255, ImageEffectLocalUtil::TexDrawType_0);
    ImageEffectLocalUtil::capture(_24, 4, 0, GX_TF_RGB565, false, 0);
    ImageEffectLocalUtil::blurTexture(_24, 4, 0, 4, _10 * mIntensity * 0.005f, 1.0f);
    ImageEffectLocalUtil::capture(_24, 4, 0, GX_TF_RGB565, false, 0);
}

// https://decomp.me/scratch/NIh1o
void DepthOfFieldBlur::drawFinal() const {
    ImageEffectLocalUtil::drawTexture(_18, 4, 0, 255, ImageEffectLocalUtil::TexDrawType_0);
    GXSetNumTexGens(3);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetTexCoordGen2(GX_TEXCOORD2, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, GX_PTIDENTITY);
    GXSetNumTevStages(2);
    GXSetNumIndStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_CPREV);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevIndirect(GX_TEVSTAGE1, GX_INDTEXSTAGE0, GX_ITF_8, GX_ITB_ST, GX_ITM_0, GX_ITW_0, GX_ITW_0, GX_FALSE, GX_FALSE, GX_ITBA_OFF);
    GXSetIndTexOrder(GX_INDTEXSTAGE0, GX_TEXCOORD2, GX_TEXMAP2);
    f32 offsetMtx[][3] = {{0.5f, 0.0f, 0.0f}, {0.0f, 0.5f, 0.0f}};
    GXSetIndTexMtx(GX_ITM_0, offsetMtx, ::sScaleExp);
    GXSetIndTexCoordScale(GX_INDTEXSTAGE0, GX_ITS_1, GX_ITS_1);
    _28->load(GX_TEXMAP1);
    _20->load(GX_TEXMAP2);
    ImageEffectLocalUtil::drawTexture(_24, 1, 0, _10 * mIntensity * 255.0f, ImageEffectLocalUtil::TexDrawType_2);
    GXSetNumTevStages(1);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetNumIndStages(0);
    GXSetTevDirect(GX_TEVSTAGE0);
    GXSetTevDirect(GX_TEVSTAGE1);
    GXSetTevDirect(GX_TEVSTAGE2);
    GXSetTevDirect(GX_TEVSTAGE3);
    ImageEffectLocalUtil::setupDrawTexture();
}
