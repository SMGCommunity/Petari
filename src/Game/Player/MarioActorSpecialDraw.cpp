#include "Game/Player/MarioActor.hpp"
#include <revolution/gx/GXEnum.h>
#include <revolution/gx/GXFrameBuf.h>
#include <revolution/gx/GXPixel.h>

void MarioActor::initScreenBox() {
    //_B44 = new (32) u8[0x80000];
    _B28 = 0.0f;
    _B24 = 0.0f;
    _B30 = 0.0f;
    _B2C = 0.0f;
    _B38 = 0.0f;
    _B34 = 0.0f;
    _B40 = 0.0f;
    _B3C = 0.0f;
}

bool MarioActor::isUseScreenBox() const {
    if (_A08 == 3) {
        return true;
    }

    return !(_A08 - 7);
}

//void MarioActor::calcScreenBoxRange() {}

void MarioActor::captureScreenBox() const {
    if (!isUseScreenBox()) {
        return;
    }
    if (_B3C < 1.0f || _B40 < 1.0f) {
        return;
    }

    GXSetTexCopyDst(_B3C, _B40, GX_TF_RGB565, GX_FALSE);
    GXSetTexCopySrc(_B34, _B38, _B3C, _B40);
    GXSetColorUpdate(GX_FALSE);
    GXSetAlphaUpdate(GX_TRUE);

    GXColor clear = {};

    GXSetCopyClear(clear, 0xFFFFFF);
    GXCopyTex(_B44, GX_TRUE);
    GXSetColorUpdate(GX_ENABLE);
    GXSetAlphaUpdate(GX_FALSE);
    GXSetDstAlpha(GX_FALSE, 0);
}

//void MarioActor::writeBackScreenBox() const {}

void MarioActor::calc1stPersonView() {
    f32 length = (mCamPos - mPosition).length();
    f32 maxVal = 1000.0f;
    f32 val = 1.0f;

    if (maxVal > length && length > 300.0f) {
        val = 1.0f - (maxVal - length) / (maxVal - 300.0f);
    }

    u8 compareVal = val * 255.0f;
    if (compareVal == 0) {
        _1A1 = true;
        hideBeeFur();
    } else {
        _1A1 = false;
        updateAlphaDL(compareVal);
    }
}

//void MarioActor::hideBeeFur() {}

//void MarioActor::calcFogLighting() {}

void MarioActor::resetFog() {
    // FIXME: contructing a color8, but never using it
    Color8 color(0, 0, 0, 0);
    _1A8 = 0;
    _1A4 = 0.0f;
    _1AA = 0;
}

//void MarioActor::updateAlphaDL(u8) {}

//void MarioActor::updateSimpleAlphaDL(u8) {}

//void MarioActor::updateReflectAlphaDL(u8) {}

//void MarioActor::updateLightDL(const Color8&, const Color8&, const Color8&, f32) {}

//void MarioActor::createRainbowDL() {}

//void MarioActor::drawScreenBlend() const {}

//void MarioActor::updateRandomTexture(f32) {}

//void MarioActor::drawWallShade(const TVec3f&, const TVec3f&, f32) const {}

void MarioActor::drawSpinInhibit() const {

}

//void MarioActor::drawColdWaterDamage() const {}

void MarioActor::setRasterScroll(s32 i1, s32 i2, s32 i3) {
    // FIXME: regswap
    _1E8 = i2;
    _1E2 = 1;
    _1E4 = _1E4 * 0.9f + i1 * 0.1f;
    _1EC = i3;
}

void MarioActor::updateRasterScroll() {
    if (_1E2 ){
        _1E2 = 0;
        return;
    }

    _1E4 *= 0.9f;

    if (_1E4 < 1.0f) {
        _1E8 = 0;
        _1E4 = 0.0f;
    }

    if (_1EC > 1.0f) {
        return;
    }

    _1EC = 0.0f;
}

//void MarioActor::drawRasterScroll(f32, s16, f32) const {}

void MarioActor::drawMosaic() const {}

void MarioActor::drawLifeUp() const {}

//void MarioActor::calcSpinEffect() {}

//void MarioActor::drawSpinEffect() const {}

//void MarioActor::drawSphereMask() const {}

//void MarioActor::initDarkMask() {}

//void MarioActor::updateDarkMask(u16) {}

//bool MarioActor::drawDarkMask() const {}

//void MarioActor::showBeeFur() {}
