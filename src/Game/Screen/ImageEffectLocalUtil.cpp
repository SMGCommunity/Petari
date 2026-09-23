#include "Game/Screen/ImageEffectLocalUtil.hpp"
#include "Game/NameObj/NameObjAdaptor.hpp"
#include "Game/Scene/SceneFunction.hpp"
#include "Game/Screen/BloomEffect.hpp"
#include "Game/Util/Color.hpp"
#include "Game/Util/DrawUtil.hpp"
#include "Game/Util/ObjUtil.hpp"
#include "Game/Util/ScreenUtil.hpp"
#include "Game/Util/SystemUtil.hpp"
#include <JSystem/JUtility/JUTTexture.hpp>

namespace {
    void setTextureTrans(f32 x, f32 y) {
        TPos3f transMtx;
        transMtx.makeTrans(x, y, 0.0f);
        GXLoadTexMtxImm(transMtx.toMtxPtr(), GX_TEXMTX0, GX_MTX2x4);
    }
};  // namespace

void MR::connectToSceneImageEffect(NameObj* pObj) {
    MR::connectToScene(pObj, MR::MovementType_ImageEffect, MR::CalcAnimType_Environment, MR::DrawBufferType_None, MR::DrawType_ImageEffect);
}

void MR::connectToSceneImageEffectMovement(NameObj* pObj) {
    MR::connectToScene(pObj, MR::MovementType_ImageEffect, MR::CalcAnimType_None, MR::DrawBufferType_None, MR::DrawType_None);
}

void ImageEffectLocalUtil::capture(JUTTexture* pTexture, s32 divisions, s32 tile, GXTexFmt format, bool clear, u8 filter) {
    pTexture->capture((tile % divisions) * MR::getFrameBufferWidth() / divisions, (tile / divisions) * MR::getFrameBufferHeight() / divisions, format,
                      clear, filter);
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
    GXLoadPosMtxImm(posMtx.toMtxPtr(), GX_PNMTX0);
    GXSetCurrentMtx(GX_PNMTX0);

    GXSetCullMode(GX_CULL_NONE);
    GXSetClipMode(GX_CLIP_ENABLE);
    GXSetNumChans(1);
    GXSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
    GXSetNumTexGens(1);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0, GX_FALSE, GX_PTIDENTITY);
    GXLoadTexMtxImm(posMtx.toMtxPtr(), GX_TEXMTX0, GX_MTX2x4);
    GXSetNumTevStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
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

void ImageEffectLocalUtil::drawTexture(JUTTexture* pTexture, s32 divisions, s32 tile, u8 intensity, ETexDrawType texDrawType) {
    pTexture->load(GX_TEXMAP0);

    if (texDrawType == TexDrawType_2) {
        GXSetChanMatColor(GX_COLOR0A0, Color8(255, 255, 255, intensity));
        GXSetChanAmbColor(GX_COLOR0A0, Color8(0, 0, 0, 255));
    } else {
        GXSetChanMatColor(GX_COLOR0A0, Color8(intensity, intensity, intensity, 255));
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

    sendTextureVertex(divisions, tile);
}

void ImageEffectLocalUtil::sendTextureVertex(s32 divisions, s32 tile) {
    s32 row = tile / divisions;
    s32 column = tile % divisions;
    f32 width = static_cast< f32 >(MR::getFrameBufferWidth()) / divisions;
    f32 height = static_cast< f32 >(MR::getFrameBufferHeight()) / divisions;
    f32 left = column * width;
    f32 right = (column + 1) * width;
    f32 bottom = (row + 1) * height;
    f32 top = row * height;

    GXBegin(GX_TRIANGLESTRIP, GX_VTXFMT0, 4);
    GXPosition3f32(left, bottom, 0.0f);
    GXTexCoord2f32(0.0f, 1.0f);

    GXPosition3f32(left, top, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);

    GXPosition3f32(right, bottom, 0.0f);
    GXTexCoord2f32(1.0f, 1.0f);

    GXPosition3f32(right, top, 0.0f);
    GXTexCoord2f32(1.0f, 0.0f);
    GXEnd();
}

void ImageEffectLocalUtil::blurTexture(JUTTexture* pTexture, s32 divisions, s32 tile, u32 sampleCount, f32 radius, f32 intensity) {
    f32 aspect = MR::isScreen16Per9() ? 1.333f : 1.0f;
    f32 verticalRadius = radius * aspect;
    s32 sampleIntensity = 255.0f * intensity / sampleCount;

    for (u32 i = 0; i < sampleCount; i++) {
        f32 angle = 2.0f * (i * JGeometry::TUtil< f32 >::PI()) / sampleCount;
        ::setTextureTrans(radius * JMACosRadian(angle), verticalRadius * JMASinRadian(angle));
        ETexDrawType type = TexDrawType_0;
        if (i != 0) {
            type = TexDrawType_1;
        }

        drawTexture(pTexture, divisions, tile, sampleIntensity, type);
    }

    ::setTextureTrans(0.0f, 0.0f);
}

void MR::connectToSceneNormalBloom(BloomEffect* pBloomEffect) {
    MR::connectToScene(pBloomEffect, MR::MovementType_None, MR::CalcAnimType_Environment, MR::DrawBufferType_None, MR::DrawType_None);

    NameObjAdaptor* pPreDrawAdaptor = new NameObjAdaptor("BloomEffect::preDraw");

    pPreDrawAdaptor->connectToDraw(MR::Functor(pBloomEffect, &BloomEffect::preDraw));
    MR::connectToScene(pPreDrawAdaptor, MR::MovementType_None, MR::CalcAnimType_None, MR::DrawBufferType_None, MR::DrawType_BloomEffectPreDraw);

    NameObjAdaptor* pPostDrawAdaptor = new NameObjAdaptor("BloomEffect::postDraw");

    pPostDrawAdaptor->connectToDraw(MR::Functor(pBloomEffect, &BloomEffect::postDraw));
    MR::connectToScene(pPostDrawAdaptor, MR::MovementType_None, MR::CalcAnimType_None, MR::DrawBufferType_None, MR::DrawType_BloomEffectPostDraw);
}
