#include "Game/Screen/BloomEffectSimple.hpp"
#include "Game/Screen/CopyFilterNegater.hpp"
#include "Game/Screen/ImageEffectLocalUtil.hpp"
#include "Game/Screen/ImageEffectResource.hpp"
#include "Game/Screen/ImageEffectSystemHolder.hpp"
#include "Game/Util/DrawUtil.hpp"

namespace {
    // static const s32 sLen = _;
    // static const s32 sDivNum = _;
};  // namespace

namespace {
    class TexSpec {
    public:
        TexSpec() {
        }

        void set(JUTTexture*, s32, s32, GXTexFmt);

        /* 0x00 */ JUTTexture* _0;
        /* 0x04 */ s32 _4;
        /* 0x08 */ s32 _8;
        /* 0x0C */ GXTexFmt _C;
    };
};  // namespace

BloomEffectSimple::BloomEffectSimple()
    : ImageEffectBase("簡易ブルーム"), _14(), _18(), _1C(), _20(), _24(), _28(), _2C(), mMaskFilter(), mThreshold(205), mIntensity(0.3f) {
    MR::connectToSceneImageEffect(this);
    MR::createImageEffectSystemHolder();
    MR::getImageEffectResource()->createBloomTexture();

    _14 = MR::getImageEffectResource()->_0;
    _18 = MR::getImageEffectResource()->_4;
    _1C = MR::getImageEffectResource()->_C;
    _20 = MR::getImageEffectResource()->_10;
    _24 = MR::getImageEffectResource()->_14;
    _28 = MR::getImageEffectResource()->_18;
    _2C = MR::getImageEffectResource()->_1C;
}

void BloomEffectSimple::draw() const {
    if (!isSomething()) {
        return;
    }

    CopyFilterNegater copyFilterNegator;

    TexSpec texSpec;
    texSpec.set(_20, 4, 0, GX_TF_RGB565);

    ImageEffectLocalUtil::capture(texSpec._0, texSpec._4, texSpec._8, texSpec._C, false, 0);
    ImageEffectLocalUtil::capture(_18, texSpec._4, texSpec._8, GX_TF_RGB565, true, 0);
    MR::reinitGX();
    ImageEffectLocalUtil::setupDrawTexture();
    ImageEffectLocalUtil::drawTexture(_18, 4, 0, 255, ImageEffectLocalUtil::TexDrawType_0);
    ImageEffectLocalUtil::capture(_1C, 4, 0, GX_TF_RGB565, false, 0);

    switch (mMaskFilter) {
    case 0:
        ImageEffectLocalUtil::capture(_2C, 4, 0, GX_TF_I8, false, 0);
        break;
    case 1:
        ImageEffectLocalUtil::capture(_2C, 4, 0, GX_CTF_R8, false, 0);
        break;
    case 2:
        ImageEffectLocalUtil::capture(_2C, 4, 0, GX_CTF_G8, false, 0);
        break;
    case 3:
        ImageEffectLocalUtil::capture(_2C, 4, 0, GX_CTF_B8, false, 0);
        break;
    }

    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_TEXC, GX_CC_RASC, GX_CC_ONE, GX_CC_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_COMP_R8_GT, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_KONST);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    ImageEffectLocalUtil::drawTexture(_2C, 4, 0, mThreshold, ImageEffectLocalUtil::TexDrawType_3);
    ImageEffectLocalUtil::capture(_24, 4, 0, GX_TF_RGB565, true, 0);
    GXSetBlendMode(GX_BM_NONE, GX_BL_ZERO, GX_BL_ZERO, GX_LO_CLEAR);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);
    ImageEffectLocalUtil::setupDrawTexture();
    texSpec.set(_24, 8, 0, GX_TF_RGB565);
    ImageEffectLocalUtil::blurTexture(texSpec._0, texSpec._4, texSpec._8, 8, 0.009f, 1.0f);
    ImageEffectLocalUtil::capture(_28, texSpec._4, texSpec._8, texSpec._C, false, 0);
    ImageEffectLocalUtil::drawTexture(_20, 4, 0, 255, ImageEffectLocalUtil::TexDrawType_0);
    ImageEffectLocalUtil::drawTexture(_28, 1, 0, mThreshold * _10 * 255.0f, ImageEffectLocalUtil::TexDrawType_1);
}

void TexSpec::set(JUTTexture* pParam1, s32 param2, s32 param3, GXTexFmt param4) {
    _0 = pParam1;
    _4 = param2;
    _8 = param3;
    _C = param4;
}
