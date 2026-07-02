#include "Game/Screen/ImageEffectLocalUtil.hpp"
#include "Game/NameObj/NameObjAdaptor.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Screen/BloomEffect.hpp"
#include "Game/Util/Color.hpp"
#include "Game/Util/DrawUtil.hpp"
#include "Game/Util/Functor.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include <JSystem/JUtility/JUTTexture.hpp>

namespace {
    void setTextureTrans(f32 x, f32 y) {
        Mtx transMtx = {
            {1.0f, 0.0f, 0.0f, x},
            {0.0f, 1.0f, 0.0f, y},
            {0.0f, 1.0f, 1.0f, 0.0f},
        };

        GXLoadTexMtxImm(transMtx, GX_TEXMTX0, GX_MTX3x4);
    }
};  // namespace

void MR::connectToSceneImageEffect(NameObj* pObj) {
    MR::connectToScene(pObj, MovementType_ImageEffect, CalcAnimType_Environment, -1, DrawType_ImageEffect);
}

void MR::connectToSceneImageEffectMovement(NameObj* pObj) {
    MR::connectToScene(pObj, MovementType_ImageEffect, -1, -1, -1);
}

void ImageEffectLocalUtil::capture(JUTTexture* pTexture, s32 param2, s32 param3, GXTexFmt format, bool param5, u8 param6) {
    pTexture->capture((param2 % MR::getFrameBufferWidth()) / param2, (param3 % MR::getFrameBufferHeight()) / param3, format, param5, param6);
}

void ImageEffectLocalUtil::setupDrawTexture() {
    Mtx44 projectionMtx;
    C_MTXOrtho(projectionMtx, 0.0f, MR::getFrameBufferHeight(), 0.0f, MR::getFrameBufferWidth(), -10000.0f, 10000.0f);
    GXSetProjection(projectionMtx, GX_ORTHOGRAPHIC);

    MR::loadViewMtxFor2DModel();

    GXClearVtxDesc();
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);

    TMtx34f posMtx;
    posMtx.identity();
    GXLoadPosMtxImm((MtxPtr)&posMtx, GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);

    GXSetCullMode(GX_CULL_NONE);
    GXSetClipMode(GX_CLIP_ENABLE);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT0, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXLoadTexMtxImm(posMtx, GX_TEXMTX0, GX_MTX3x4);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_DECAL);
    GXSetZTexture(GX_ZT_DISABLE, GX_TF_Z24X8, 0);
    GXSetNumIndStages(0);
    GXSetTevDirect(GX_TEVSTAGE0);
    GXSetTevDirect(GX_TEVSTAGE1);
    GXSetTevDirect(GX_TEVSTAGE2);
    GXSetTevDirect(GX_TEVSTAGE3);
    GXSetTevDirect(GX_TEVSTAGE4);
    GXSetTevDirect(GX_TEVSTAGE5);
    GXSetTevDirect(GX_TEVSTAGE6);
    GXSetTevDirect(GX_TEVSTAGE7);
    GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, Color8(0, 0, 0, 0));
    GXSetColorUpdate(GX_TRUE);
    GXSetAlphaUpdate(GX_FALSE);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_OR, GX_ALWAYS, 0);
    GXSetDither(GX_FALSE);
}

void ImageEffectLocalUtil::drawTexture(JUTTexture* pTexture, s32 param2, s32 param3, u8 param4, ETexDrawType texDrawType) {
    pTexture->load(GX_TEXMAP0);

    if (texDrawType == TexDrawType_2) {
        GXSetChanMatColor(GX_COLOR0A0, Color8(255, 255, 255, param4));
        GXSetChanAmbColor(GX_COLOR0A0, Color8(0, 0, 0, 255));
    } else {
        GXSetChanMatColor(GX_COLOR0A0, Color8(param4, param4, param4, 255));
        GXSetChanAmbColor(GX_COLOR0A0, Color8(0, 0, 0, 255));
    }

    switch (texDrawType) {
    case TexDrawType_0:
        GXSetBlendMode(GX_BM_NONE, GX_BL_ZERO, GX_BL_ZERO, GX_LO_CLEAR);
        break;
    case TexDrawType_1:
        GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_ONE, GX_LO_CLEAR);
        break;
    case TexDrawType_2:
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
        break;
    case TexDrawType_3:
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCCLR, GX_BL_ZERO, GX_LO_CLEAR);
        break;
    }

    sendTextureVertex(param2, param3);
}
/*
void ImageEffectLocalUtil::sendTextureVertex(s32 param1, s32 param2) {
    f32 local1 = static_cast< f32 >(MR::getFrameBufferWidth()) / param1;
    f32 local2 = static_cast< f32 >(MR::getFrameBufferHeight()) / param1;
    f32 dVar9;
    f32 dVar8;
    f32 dVar7;

    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 4);
    {
        GXPosition3f32(dVar8, dVar7, 0.0f);
        GXTexCoord2f32(0.0f, 1.0f);

        GXPosition3f32(dVar8, dVar9, 0.0f);
        GXTexCoord2f32(0.0f, 0.0f);

        GXPosition3f32(dVar10, dVar7, 0.0f);
        GXTexCoord2f32(1.0f, 1.0f);

        GXPosition3f32(dVar10, dVar9, 0.0f);
        GXTexCoord2f32(1.0f, 0.0f);
    }
    GXEnd();
}

void ImageEffectLocalUtil::blurTexture(JUTTexture* pTexture, s32 param2, s32 param3, u32 param4, f32 param5, f32 param6) {
    f32 local1 = MR::isScreen16Per9() ? 1.333f : 1.0f;

    for (s32 i = 0; i < param4; i++) {
        ::setTextureTrans(MR::cos(...) * param5, MR::sin(...) * param6);
        drawTexture(
    }

    ::setTextureTrans(0.0f, 0.0f);
}
*/
void MR::connectToSceneNormalBloom(BloomEffect* pBloomEffect) {
    MR::connectToScene(pBloomEffect, -1, CalcAnimType_Environment, -1, -1);

    NameObjAdaptor* preDrawAdaptor = new NameObjAdaptor("BloomEffect::preDraw");

    preDrawAdaptor->connectToDraw(MR::Functor(pBloomEffect, &BloomEffect::preDraw));
    MR::connectToScene(preDrawAdaptor, -1, -1, -1, DrawType_BloomEffectPreDraw);

    NameObjAdaptor* postDrawAdaptor = new NameObjAdaptor("BloomEffect::postDraw");

    postDrawAdaptor->connectToDraw(MR::Functor(pBloomEffect, &BloomEffect::postDraw));
    MR::connectToScene(postDrawAdaptor, -1, -1, -1, DrawType_BloomEffectPostDraw);
}
