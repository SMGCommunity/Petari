#pragma once

#include "Game/Screen/DepthOfFieldBlur.hpp"
#include "Game/Screen/CopyFilterNegater.hpp"
#include "Game/Screen/ImageEffectLocalUtil.hpp"
#include "Game/Screen/ImageEffectResource.hpp"
#include "Game/Screen/ImageEffectSystemHolder.hpp"
#include "Game/Util/CameraUtil.hpp"
#include "JSystem/JUtility/JUTTexture.hpp"
#include "revolution/gx/GXEnum.h"
#include "revolution/gx/GXTev.h"

DepthOfFieldBlur::DepthOfFieldBlur(const char* pName) : ImageEffectBase(pName) {
    _14 = nullptr;
    _18 = nullptr;
    _1C = nullptr;
    _20 = nullptr;
    _24 = nullptr;
    _28 = nullptr;
    mIntensity = 1.0f;
    mBlurMaxDist = 248;
    mBlurMinDist = 242;
    MR::connectToSceneImageEffect(this);
    MR::createImageEffectSystemHolder();
    MR::getImageEffectResource()->createDOFTexture();
    _14 = MR::getImageEffectResource()->_0;
    _18 = MR::getImageEffectResource()->_10;
    _1C = MR::getImageEffectResource()->_4;
    _20 = MR::getImageEffectResource()->_8;
    _24 = MR::getImageEffectResource()->_C;
    _28 = new JUTTexture(0x80, 2, GX_TF_I8);
    _28->mWrapS = 0;
    _28->mWrapT = 0;
    _28->mMinType = 1;
    _28->mMagType = 1;
}

void DepthOfFieldBlur::draw() const {
    bool v1 = false;

    if (_C || _D) {
        v1 = true;
    }

    if (v1) {
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
}

/*
void DepthOfFieldBlur::drawZAlphaTex() const {
}
*/

void DepthOfFieldBlur::createBlurTexture() const {
    ImageEffectLocalUtil::drawTexture(_1C, 4, 0, 255, ImageEffectLocalUtil::UNK_0);
    ImageEffectLocalUtil::capture(_24, 4, 0, GX_TF_RGB565, false, 0);
    ImageEffectLocalUtil::blurTexture(_24, 4, 0, 4, ((0.0049f) * (_10 * mIntensity)), 1.0f);
    ImageEffectLocalUtil::capture(_24, 4, 0, GX_TF_RGB565, false, 0);
}

const f32 offs[2][3] = {{0.5f, 0.0f, 0.0f}, {0.0f, 0.5f, 0.0f}};

// https://decomp.me/scratch/NIh1o
void DepthOfFieldBlur::drawFinal() const {
    ImageEffectLocalUtil::drawTexture(_18, 4, 0, 255, ImageEffectLocalUtil::UNK_0);
    GXSetNumTexGens(3);
    GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, 0, 0x7D);
    GXSetTexCoordGen2(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, 0, 0x7D);
    GXSetTexCoordGen2(GX_TEXCOORD2, GX_TG_MTX2x4, GX_TG_TEX0, 0x3C, 0, 0x7D);
    GXSetNumTevStages(2);
    GXSetNumIndStages(1);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_CPREV);
    GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA, GX_CA_ZERO);
    GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, 1, GX_TEVPREV);
    GXSetTevIndirect(GX_TEVSTAGE1, GX_INDTEXSTAGE0, GX_ITF_8, GX_ITB_ST, GX_ITM_0, GX_ITW_0, GX_ITW_0, 0, 0, GX_ITBA_OFF);
    GXSetIndTexOrder(GX_INDTEXSTAGE0, GX_TEXCOORD2, GX_TEXMAP2);
    GXSetIndTexMtx(GX_ITM_0, offs, 1);
    GXSetIndTexCoordScale(GX_INDTEXSTAGE0, GX_ITS_1, GX_ITS_1);
    _28->load(GX_TEXMAP1);
    _20->load(GX_TEXMAP2);
    f32 val = (mIntensity * _10) * 255.0f;
    ImageEffectLocalUtil::drawTexture(_24, 1, 0, val, ImageEffectLocalUtil::UNK_2);
    GXSetNumTevStages(1);
    GXSetTevOp(GX_TEVSTAGE0, GX_PASSCLR);
    GXSetNumIndStages(0);
    GXSetTevDirect(GX_TEVSTAGE0);
    GXSetTevDirect(GX_TEVSTAGE1);
    GXSetTevDirect(GX_TEVSTAGE2);
    GXSetTevDirect(GX_TEVSTAGE3);
    ImageEffectLocalUtil::setupDrawTexture();
}
