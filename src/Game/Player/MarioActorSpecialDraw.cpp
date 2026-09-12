#include "Game/LiveActor/HitSensor.hpp"
#include "Game/Player/DLchanger.hpp"
#include "Game/Player/J3DModelX.hpp"
#include "Game/Player/MarioActor.hpp"
#include "Game/Player/MarioConst.hpp"
#include "Game/Player/MarioParts.hpp"
#include "Game/Player/MarioState.hpp"
#include "Game/Player/MarioSwim.hpp"
#include "Game/Screen/FullScreenBlur.hpp"
#include "Game/Util.hpp"
#include "Game/Util/DirectDraw.hpp"
#include "Game/Util/FurMulti.hpp"
#include "Game/Util/SchedulerUtil.hpp"
#include <JSystem/JGeometry/TBox.hpp>
#include <JSystem/JUtility/JUTTexture.hpp>
#include <new>
#include <revolution/gd.h>
#include <revolution/gx/GXEnum.h>
#include <revolution/gx/GXFrameBuf.h>
#include <revolution/gx/GXPixel.h>
#include <revolution/os.h>

void MarioActor::initScreenBox() {
    _B44 = new (32) u8[0x20000];
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

void MarioActor::calcScreenBoxRange() {
    if (!isUseScreenBox())
        return;
    TVec3f position;
    position = _B18;
    TVec2f center;
    TVec2f x;
    TVec2f y;
    TVec2f z;
    TVec2f minimum;
    TVec2f maximum;
    TVec2f frameMin;
    TVec2f frameMax;
    MR::calcScreenPosition(&center, position);
    MR::calcScreenPosition(&x, position + TVec3f(80.0f, 0.0f, 0.0f));
    MR::calcScreenPosition(&y, position + TVec3f(0.0f, 80.0f, 0.0f));
    MR::calcScreenPosition(&z, position + TVec3f(0.0f, 0.0f, 80.0f));
    TVec3f radius((x - center).length(), (y - center).length(), (z - center).length());
    f32 range = radius.length();
    minimum = center - TVec2f(range, range);
    maximum = center + TVec2f(range, range);
    _B24 = minimum.x < maximum.x ? minimum.x : maximum.x;
    _B2C = minimum.x > maximum.x ? minimum.x : maximum.x;
    _B28 = minimum.y < maximum.y ? minimum.y : maximum.y;
    _B30 = minimum.y > maximum.y ? minimum.y : maximum.y;
    MR::convertScreenPosToFrameBufferPos(&frameMin, mScreenBoxMin);
    MR::convertScreenPosToFrameBufferPos(&frameMax, mScreenBoxMax);
    s16 left = static_cast< s32 >(frameMin.x) & ~1;
    s16 top = static_cast< s32 >(frameMin.y) & ~1;
    s16 width = static_cast< s32 >(frameMax.x) - left;
    s16 height = static_cast< s32 >(frameMax.y) - top;
    width = (width + 1) & ~1;
    height = (height + 1) & ~1;
    if (width > 256)
        width = 256;
    if (height > 256)
        height = 256;
    if (!width)
        width = 2;
    if (!height)
        height = 2;
    _B34 = left;
    _B38 = top;
    _B3C = width;
    _B40 = height;
    TBox2f capture(_B34, _B38, _B34 + _B3C, _B38 + _B40);
    s32 screenHeight = MR::getScreenHeight();
    f32 frameWidth = MR::getFrameBufferWidth();
    f32 frameHeight = screenHeight;
    TBox2f screen(0.0f, 0.0f, frameWidth, frameHeight);
    TBox2f clipped(capture);
    if (!clipped.intersect(screen)) {
        clipped.i.zero();
        clipped.f.zero();
    }
    _B34 = clipped.i.x;
    _B38 = clipped.i.y;
    _B3C = clipped.getWidth();
    _B40 = clipped.getHeight();
}

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

void MarioActor::writeBackScreenBox() const {
    if (!isUseScreenBox())
        return;
    if (_B3C < 1.0f || _B40 < 1.0f)
        return;
    GXTexObj texture;
    TVec2f minimum;
    TVec2f maximum;
    TDDraw::setup(1, 0, 2);
    GXInitTexObj(&texture, _B44, _B3C, _B40, GX_TF_RGB565, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXLoadTexObj(&texture, GX_TEXMAP0);
    MR::convertFrameBufferPosToScreenPos(&minimum, mScreenBoxPos);
    MR::convertFrameBufferPosToScreenPos(&maximum, mScreenBoxPos + mScreenBoxSize + TVec2f(0.99f, 0.99f));
    GXSetBlendMode(GX_BM_BLEND, GX_BL_INVDSTALPHA, GX_BL_DSTALPHA, GX_LO_NOOP);
    GXSetDstAlpha(GX_TRUE, 0);
    GXSetAlphaUpdate(GX_TRUE);
    GXPixModeSync();
    GXTexModeSync();
    maximum.x = static_cast< s16 >(static_cast< s16 >(static_cast< s32 >(maximum.x) + 1) & ~1);
    GXSetZMode(GX_TRUE, GX_ALWAYS, GX_TRUE);
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(minimum.x, minimum.y, 0.9999f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(maximum.x, minimum.y, 0.9999f);
    GXTexCoord2f32(1.0f, 0.0f);
    GXPosition3f32(maximum.x, maximum.y, 0.9999f);
    GXTexCoord2f32(1.0f, 1.0f);
    GXPosition3f32(minimum.x, maximum.y, 0.9999f);
    GXTexCoord2f32(0.0f, 1.0f);
    GXEnd();
    GXSetDstAlpha(GX_FALSE, 0);
    GXSetAlphaUpdate(GX_FALSE);
    TDDraw::close();
}

void MarioActor::calc1stPersonView() {
    f32 length = (mCamPos - mPosition).length();
    f32 minVal = 300.0f;
    f32 maxVal = 1000.0f;
    f32 val;

    if (length > maxVal) {
        val = 1.0f;
    } else if (length < minVal) {
        val = 0.0f;
    } else {
        val = 1.0f - (maxVal - length) / (maxVal - minVal);
    }

    u8 compareVal = 255.0f * val;
    if (compareVal == 0) {
        _1A1 = true;
        hideBeeFur();
    } else {
        _1A1 = false;
        updateAlphaDL(compareVal);
    }
}

void MarioActor::hideBeeFur() {
    if (mMario->isPlayerModeBee()) {
        _9E8->kill();
        static_cast< FurMulti* >(_9EC)->offDraw(-1);
    }
    if (getCarrySensor())
        MR::hideModel(getCarrySensor()->mHost);
    if (mMario->isPlayerModeInvincible()) {
        _A6E = 0;
        stopEffect("無敵中");
    }
}

void MarioActor::calcFogLighting() {
    Color8 fog(240, 16, 80, 0);
    if (!mMario->isStatusActive(MarioStatus_Paralyze) && isNeedDamageFog()) {
        f32 phase = ((_1A8 + 31) & 63) / 63.0f;
        if (phase > 0.5f)
            phase = 1.0f - phase;
        f32 wave = 2.0f * phase;
        f32 low = getConst().getTable()->mDamageFogLow;
        _1A4 = (low + wave * (getConst().getTable()->mDamageFogHigh - getConst().getTable()->mDamageFogLow)) / 255.0f;
    } else if (_1AA) {
        fog.mGXColor = _1B0.mGXColor;
        if (_1B5)
            fog.g = 255.0f * MR::sin(PI * (_1AA / 20.0f));
        _1A4 = _1AC * MR::sin(PI * (_1AA / static_cast< f32 >(getConst().getTable()->mStarPieceFogTime)));
    } else if (mMario->_434) {
        fog.set(255, 255, 0, 0);
        _1A4 = 0.8f * MR::sin((PI * (mMario->_434 / static_cast< f32 >(getConst().getTable()->mItemDashTimer))) / 2.0f);
    } else {
        resetFog();
    }
    Color8 ambient = *MR::getLightAmbientColor(this);
    Color8 material(255, 255, 255, 255);
    updateLightDL(ambient, material, fog, _1A4);
}

void MarioActor::resetFog() {
    Color8 color(0, 0, 0, 0);
    _1A8 = 0;
    _1A4 = 0.0f;
    _1AA = 0;
}

void MarioActor::updateAlphaDL(u8 alpha) {
    u8 buffer[256] ATTRIBUTE_ALIGN(32);
    GDLObj displayList;
    MR::ProhibitSchedulerAndInterrupts lock(false);
    GDInitGDLObj(&displayList, buffer, sizeof(buffer));
    __GDCurrentDL = &displayList;
    GXColor color = {0, 0, 0, 255};
    color.a = alpha;
    GDSetTevColor(GX_TEVREG2, color);
    GDSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_NOOP);
    GDSetGenMode2(1, 0, 1, 0, GX_CULL_BACK);
    GDSetChanCtrl(GX_COLOR0A0, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    GDSetChanCtrl(GX_COLOR1A1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    GDSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    GDSetTevAlphaCalcAndSwap(GX_TEVSTAGE0, GX_CA_A2, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV,
                             GX_TEV_SWAP0, GX_TEV_SWAP0);
    GDSetTevColorCalc(GX_TEVSTAGE0, GX_CC_TEXC, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GDSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
    GDPadCurr32();
    u32 size = (displayList.ptr - displayList.start + 31) & ~31;
    DLholder* holder = mDLchanger->swap();
    holder->mSize = displayList.ptr - displayList.start;
    MR::copyMemory(holder->mDL, buffer, size);
    DCStoreRange(holder->mDL, size);
}

void MarioActor::updateSimpleAlphaDL(u8 alpha) {
    u8 buffer[256] ATTRIBUTE_ALIGN(32);
    GDLObj displayList;
    MR::ProhibitSchedulerAndInterrupts lock(false);
    GDInitGDLObj(&displayList, buffer, sizeof(buffer));
    __GDCurrentDL = &displayList;
    GDSetDstAlpha(GX_TRUE, alpha);
    GDPadCurr32();
    u32 size = (displayList.ptr - displayList.start + 31) & ~31;
    DLholder* holder = mDLchanger->swap();
    holder->mSize = displayList.ptr - displayList.start;
    MR::copyMemory(holder->mDL, buffer, size);
    DCStoreRange(holder->mDL, size);
}

void MarioActor::updateReflectAlphaDL(u8 alpha) {
    u8 buffer[256] ATTRIBUTE_ALIGN(32);
    GDLObj displayList;
    MR::ProhibitSchedulerAndInterrupts lock(false);
    GDInitGDLObj(&displayList, buffer, sizeof(buffer));
    __GDCurrentDL = &displayList;
    GXColor color = {0, 0, 0, 255};
    color.a = alpha;
    GDSetTevColor(GX_TEVREG2, color);
    GDSetTevAlphaCalcAndSwap(GX_TEVSTAGE0, GX_CA_A2, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV,
                             GX_TEV_SWAP0, GX_TEV_SWAP0);
    GDPadCurr32();
    u32 size = (displayList.ptr - displayList.start + 31) & ~31;
    DLholder* holder = mDLchanger->swap();
    holder->mSize = displayList.ptr - displayList.start;
    MR::copyMemory(holder->mDL, buffer, size);
    DCStoreRange(holder->mDL, size);
}

void MarioActor::updateLightDL(const Color8& ambient, const Color8& material, const Color8& fog, f32 strength) {
    u8 buffer[512] ATTRIBUTE_ALIGN(32);
    GDLObj displayList;
    MR::ProhibitSchedulerAndInterrupts lock(false);
    GDInitGDLObj(&displayList, buffer, sizeof(buffer));
    __GDCurrentDL = &displayList;
    GXColor ambientColor = ambient.mGXColor;
    GDSetChanAmbColor(GX_COLOR0A0, ambientColor);
    GXColor materialColor = material.mGXColor;
    GDSetChanMatColor(GX_COLOR0A0, materialColor);
    GDSetChanCtrl(GX_COLOR1, GX_FALSE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_CLAMP, GX_AF_NONE);
    if (!MR::isNearZero(strength)) {
        f32 nearZ = MR::getNearZ();
        f32 farZ = MR::getFarZ();
        GXColor color = fog.mGXColor;
        f32 start;
        f32 end;
        MR::calcFogStartEnd(mPosition, strength, &start, &end);
        GDSetFog(GX_FOG_LIN, start, end, nearZ, farZ, color);
    }
    GDPadCurr32();
    u32 length = displayList.ptr - displayList.start;
    mCurrDL = 1 - mCurrDL;
    mDLSize = length;
    u32 size = (length + 31) & ~31;
    MR::copyMemory(mDL[mCurrDL], buffer, size);
    DCStoreRange(mDL[mCurrDL], size);
}

void MarioActor::createRainbowDL() {
    DLholder* holder;
    u8 buffer[256] ATTRIBUTE_ALIGN(32);
    GDLObj displayList;
    MR::ProhibitSchedulerAndInterrupts lock(false);
    for (u32 alpha = 0; alpha < 8; alpha++) {
        for (u32 index = 0; index < 8; index++) {
            _94[alpha * 8 + index] = new DLchanger(1, 32);
            GDInitGDLObj(&displayList, buffer, sizeof(buffer));
            __GDCurrentDL = &displayList;
            GXColor color = {0, 0, 0, 0};
            if (index & 1)
                color.r = 255;
            if (index & 2)
                color.g = 255;
            if (index & 4)
                color.b = 255;
            if (!index) {
                color.r = 64;
                color.g = 64;
                color.b = 64;
            }
            color.a = 16 + (alpha + 1) * 24;
            GDSetTevColor(GX_TEVREG0, color);
            GDPadCurr32();
            u32 length = displayList.ptr - displayList.start;
            u32 size = (length + 31) & ~31;
            DLchanger* changer = _94[alpha * 8 + index];
            holder = &changer->mBuffers[changer->mCurrentBuffer];
            holder->mSize = length;
            MR::copyMemory(holder->mDL, buffer, size);
            DCStoreRange(holder->mDL, size);
        }
    }
}

void MarioActor::drawScreenBlend() const {
    if (mMario->_97C && mMario->_97C->getBlurOffset() != 0.0f) {
        MR::drawFullScreenBlur(mMario->_97C->getBlurOffset());
    }
    if (_1C6)
        MR::drawFullScreenBlur(_1C8, _1CC, _1D0, _1D1);
    if (mBeeWallWalk) {
        TVec2f position;
        TVec2f opposite;
        TVec2f minimum;
        TVec2f maximum;
        TDDraw::project2D(&position, mMario->mShadowPos);
        opposite = position + TVec2f(100.0f, 100.0f);
        position = position + TVec2f(-100.0f, -100.0f);
        if (position.x >= opposite.x) {
            minimum.x = opposite.x;
            maximum.x = position.x;
        } else {
            minimum.x = position.x;
            maximum.x = opposite.x;
        }
        if (position.y >= opposite.y) {
            minimum.y = opposite.y;
            maximum.y = position.y;
        } else {
            minimum.y = position.y;
            maximum.y = opposite.y;
        }
        position = minimum + TVec2f(-10.0f, -10.0f);
        opposite = maximum + TVec2f(10.0f, 10.0f);
        TDDraw::setup(0, 0, 2);
        GXSetCullMode(GX_CULL_NONE);
        GXSetAlphaUpdate(GX_FALSE);
        GXSetColorUpdate(GX_TRUE);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_ONE, GX_BL_INVDSTALPHA, GX_LO_NOOP);
        GXSetDstAlpha(GX_TRUE, 0);
        TDDraw::drawFillBox(position, opposite, 0);
        GXSetAlphaUpdate(GX_FALSE);
        MR::loadProjectionMtx();
        MR::loadViewMtx();
    }
    if (_BC4) {
        u32 color = 0xFF000080;
        if (mMario->_1C._3)
            color = 0x00004020;
        TDDraw::setup(0, 1, 2);
        TDDraw::drawFillBox(TVec3f(0.0f, 0.0f, 0.0f),
                            TVec3f(static_cast< f32 >(MR::getScreenWidth()), static_cast< f32 >(MR::getScreenHeight()), 0.0f), color);
        TDDraw::close();
    }
}

void MarioActor::updateRandomTexture(f32 distance) {
    _B88 = 1 - _B88;
    u8* pixel;
    u8* image = mMaskTextures[_B88]->mImage;
    distance = 1.0f - distance / 1000.0f;
    f32 probability = MR::clamp(distance, 0.0f, 1.0f);
    for (u32 y = 0; y < 8; y++) {
        pixel = image + y * 8;
        for (u32 x = 0; x < 8; x++) {
            s32 alpha = *pixel >> 4;
            if (MR::getRandom() < probability)
                alpha += 4;
            else
                alpha--;
            alpha = MR::clamp(alpha, 0, 15);
            *pixel = (alpha * 16) & 0xF0;
            pixel++;
        }
    }
    DCStoreRange(image, 64);
}

void MarioActor::drawWallShade(const TVec3f& position, const TVec3f& normal, f32 unused) const {
    f32 radius = 100.0f;
    TDDraw::setup(1, 1, 0);
    GXSetZMode(GX_TRUE, GX_GREATER, GX_FALSE);
    GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
    GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
    GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA, GX_CA_ZERO);
    GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
    mMaskTextures[_B88]->load(GX_TEXMAP0);
    GXClearVtxDesc();
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0);
    GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
    GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
    Mtx rotation;
    TVec3f tangent(0.0f, normal.z, -normal.y);
    if (MR::isNearZero(tangent))
        tangent.set< f32 >(normal.z, 0.0f, normal.x);
    MR::normalizeOrZero(&tangent);
    PSMTXRotAxisRad(rotation, &normal, 0.3926991f);
    GXBegin(GX_TRIANGLEFAN, GX_VTXFMT0, 18);
    GXPosition3f32(position.x - 5.0f * normal.x, position.y - 5.0f * normal.y, position.z - 5.0f * normal.z);
    GXColor1u32(128);
    GXTexCoord2f32(0.0f, 0.0f);
    for (u32 i = 0; i <= 16; i++) {
        f32 angle = 2.0f * ((i / 16.0f) * PI);
        TVec3f point = position - normal * 5.0f + tangent * radius;
        GXPosition3f32(point.x, point.y, point.z);
        GXColor1u32(1);
        GXTexCoord2f32(10.0f * MR::cos(angle), 10.0f * MR::sin(angle));
        PSMTXMultVecSR(rotation, &tangent, &tangent);
    }
    GXEnd();
}

void MarioActor::drawSpinInhibit() const {
}

void MarioActor::drawColdWaterDamage() const {
    u32 next;
    u32 stripHeight = 4;
    GXTexObj textures[2];
    GXDrawDone();
    GXTexModeSync();
    GXPixModeSync();
    u16 width = MR::getFrameBufferWidth();
    TDDraw::setup(1, 0, 2);
    u16 current = 0;
    GXInitTexObj(&textures[0], _1D8, width, 4, GX_TF_RGB565, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXInitTexObj(&textures[1], _1DC, width, 4, GX_TF_RGB565, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXSetLineWidth(6, GX_TO_ZERO);
    GXSetTexCopyDst(width, 4, GX_TF_RGB565, GX_FALSE);
    GXSetTexCopySrc(0, 0, width, 4);
    GXCopyTex(_1D8, GX_FALSE);
    f32 textureStep = 1.0f / static_cast< f32 >(stripHeight);
    f32 frequency = 89.0f * MR::sin((TWO_PI * mMario->mSwim->mColdWaterDamageInterval) / 120.0f);
    if (frequency > 0.0f)
        frequency = 0.0f;
    for (u32 y = 0; y < MR::getScreenHeight(); y += 4) {
        GXTexModeSync();
        GXPixModeSync();
        GXSetTexCopySrc(0, y + 4, width, 4);
        next = 1 - current;
        GXCopyTex(mRasterBuffers[next], GX_FALSE);
        GXLoadTexObj(&textures[current], GX_TEXMAP0);
        f32 v = 0.0f;
        for (u32 row = y; row < y + 4; row++) {
            f32 offset = 5.0f * MR::sin(2.0f * (((_37C + row * frequency) / MR::getScreenHeight()) * PI));
            GXBegin(GX_LINES, GX_VTXFMT0, 2);
            GXPosition3f32(offset, row, 0.0f);
            GXTexCoord2f32(0.0f, v);
            GXPosition3f32(offset + MR::getScreenWidth(), row, 0.0f);
            GXTexCoord2f32(1.0f, v);
            GXEnd();
            v += textureStep;
        }
        current = next;
    }
    GXDrawDone();
}

void MarioActor::setRasterScroll(s32 i1, s32 i2, s32 i3) {
    _1E8 = i2;
    _1E2 = 1;
    _1E4 = 0.9f * _1E4 + 0.1f * i1;
    _1EC = i3;
}

void MarioActor::updateRasterScroll() {
    if (_1E2) {
        _1E2 = 0;
        return;
    }

    _1E4 *= 0.9f;

    if (_1E4 < 1.0f) {
        _1E8 = 0;
        _1E4 = 0.0f;
    }

    if (_1EC < 1.0f) {
        _1EC = 0.0f;
    }
}

void MarioActor::drawRasterScroll(f32 amplitude, s16 period, f32 wavelength) const {
    if (wavelength == 0.0f)
        return;
    if (period == 0)
        return;
    u32 next;
    u32 stripHeight = 4;
    GXTexObj textures[2];
    GXDrawDone();
    GXTexModeSync();
    GXPixModeSync();
    u16 width = MR::getFrameBufferWidth();
    TDDraw::setup(1, 0, 2);
    u16 current = 0;
    GXInitTexObj(&textures[0], _1D8, width, 4, GX_TF_RGB565, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXInitTexObj(&textures[1], _1DC, width, 4, GX_TF_RGB565, GX_CLAMP, GX_CLAMP, GX_FALSE);
    GXSetLineWidth(6, GX_TO_ZERO);
    GXSetTexCopyDst(width, 4, GX_TF_RGB565, GX_FALSE);
    GXSetTexCopySrc(0, 0, width, 4);
    GXCopyTex(_1D8, GX_FALSE);
    f32 textureStep = 1.0f / static_cast< f32 >(stripHeight);
    f32 spatialPhase = TWO_PI / wavelength;
    f32 phase = _37C;
    phase *= TWO_PI;
    phase /= period;
    for (u32 y = 0; y < MR::getScreenHeight(); y += 4) {
        GXTexModeSync();
        GXPixModeSync();
        GXSetTexCopySrc(0, y + 4, width, 4);
        next = 1 - current;
        GXCopyTex(mRasterBuffers[next], GX_FALSE);
        GXLoadTexObj(&textures[current], GX_TEXMAP0);
        f32 v = 0.0f;
        for (u32 row = y; row < y + 4; row++) {
            f32 angle = MR::sin(spatialPhase * row);
            angle *= PI;
            angle += phase;
            f32 offset = amplitude * MR::sin(angle);
            GXBegin(GX_LINES, GX_VTXFMT0, 2);
            GXPosition3f32(offset, row, 0.0f);
            GXTexCoord2f32(0.0f, v);
            GXPosition3f32(offset + MR::getScreenWidth(), row, 0.0f);
            GXTexCoord2f32(1.0f, v);
            GXEnd();
            v += textureStep;
        }
        current = next;
    }
    GXDrawDone();
}

void MarioActor::drawMosaic() const {
}

void MarioActor::drawLifeUp() const {
}

void MarioActor::calcSpinEffect() {
    f32 baseRadius = 30.0f;
    f32 radiusScale = 5.8f;
    _6D4 = 0.0f;
    _6D8 = 0.0f;
    if (selectAction("スピン回復エフェクト") != 1)
        return;
    if (!_945)
        return;
    if (_944)
        return;
    if (!_946)
        return;
    s32 frame = _946 - 9;
    if (frame <= 0 || frame > 15) {
        _6D4 = 0.0f;
        return;
    }
    if (frame < 10) {
        _6D4 = baseRadius + (frame / 10.0f) * (_945 * radiusScale);
        _6D8 = 120.0f;
        return;
    }
    _6D4 = baseRadius + _945 * radiusScale;
    _6D8 = 120.0f;
}

void MarioActor::drawSpinEffect() const {
    if (_6D4 == 0.0f)
        return;
    if (!isEnableNerveChange())
        return;
    TDDraw::setup(0, 1, 0);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_NOOP);
    Mtx rotation;
    TVec3f center = _2A0 + mMario->mHeadVec * _6D8;
    TVec3f direction(mMario->mFrontVec);
    TVec3f previous;
    PSMTXRotAxisRad(rotation, &mMario->mHeadVec, 0.09817477f);
    GXSetLineWidth(36, GX_TO_ZERO);
    for (u32 i = 0; i <= 64; i++) {
        TVec3f point(direction);
        point.setLength(_6D4);
        point += mMario->mHeadVec * MR::getRandom() * 10.0f;
        if (i)
            TDDraw::drawLine(center + previous, center + point, 0xFFFFFF20);
        previous = point;
        PSMTXMultVecSR(rotation, &direction, &direction);
    }
    GXSetLineWidth(18, GX_TO_ZERO);
    for (u32 i = 0; i <= 64; i++) {
        TVec3f point(direction);
        point.setLength(_6D4);
        point += mMario->mHeadVec * MR::getRandom() * 10.0f;
        f32 wave = MR::sin((((_37C + i) & 31) / 32.0f) * PI);
        f32 square = wave * wave;
        u32 alpha = 160.0f * (square * square);
        if (i)
            TDDraw::drawLine(center + previous, center + point, alpha | 0x40FF00 | (alpha << 24));
        previous = point;
        PSMTXMultVecSR(rotation, &direction, &direction);
    }
    TDDraw::close();
}

void MarioActor::drawSphereMask() const {
    if (!mMario->isVisibleRecoveryWarpBubble())
        return;
    TDDraw::setup(0, 1, 0);
    GXSetZScaleOffset(1.0f, 0.00001f);
    GXSetCullMode(GX_CULL_FRONT);
    GXSetZMode(GX_TRUE, GX_GREATER, GX_TRUE);
    GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_ONE, GX_LO_NOOP);
    TVec3f position;
    getRealPos("Spine1", &position);
    TDDraw::drawSphere3D(position, 180.0f, 0xFFFFFF30, 16);
    GXSetZScaleOffset(1.0f, 0.0f);
}

void MarioActor::initDarkMask() {
    for (u32 texture = 0; texture < 2; texture++) {
        u8* image = mMaskTextures[texture]->mImage;
        for (u32 y = 0; y < 8; y++) {
            u8* pixel = image + y * 8;
            for (u32 x = 0; x < 8; x++)
                pixel[x] = 0;
        }
    }
    _1C3 = true;
    _1C4 = 0;
}

void MarioActor::updateDarkMask(u16 unused) {
    if (!_1C3)
        return;
    u8* previous = mMaskTextures[_B88]->mImage;
    _B88 = 1 - _B88;
    u8* image = mMaskTextures[_B88]->mImage;
    u8 x = MR::getRandom(0L, 8L);
    u8 y = MR::getRandom(0L, 8L);
    image[x + y * 8] = 0xF0;
    previous[x + y * 8] = 0xF0;
    DCStoreRange(image, 64);
    _1C4++;
}

bool MarioActor::drawDarkMask() const {
    if (!_1C3)
        return false;
    TDDraw::setup(1, 0, 2);
    GXSetAlphaUpdate(GX_TRUE);
    GXSetColorUpdate(GX_FALSE);
    GXSetZMode(GX_FALSE, GX_ALWAYS, GX_FALSE);
    mMaskTextures[_B88]->load(GX_TEXMAP0);
    GXSetZCompLoc(GX_FALSE);
    GXSetAlphaCompare(GX_GREATER, 1, GX_AOP_AND, GX_ALWAYS, 0);
    GXSetDstAlpha(GX_TRUE, 255);
    TVec3f spine;
    TVec2f center;
    TVec2f screenX;
    TVec2f screenY;
    TVec2f origin;
    TVec2f frameOrigin;
    TVec2f minimum;
    TVec2f maximum;
    getRealPos("Spine1", &spine);
    f32 radius = _1C4;
    if (radius >= 240.0f)
        radius = 240.0f;
    TVec3f position(mPosition);
    TDDraw::project2D(&center, position);
    TDDraw::project2D(&screenX, position + mCamDirX * radius);
    TDDraw::project2D(&screenY, position + mCamDirY * radius);
    u16 width = ((static_cast< u32 >((screenX - center).length()) + 3) & ~3) + 4;
    u16 height = ((static_cast< u32 >((screenY - center).length()) + 3) & ~3) + 4;
    TDDraw::project2D(&origin, position);
    origin.x -= width / 2;
    origin.y -= width / 2;
    MR::convertScreenPosToFrameBufferPos(&frameOrigin, origin);
    u32 left = static_cast< u32 >(frameOrigin.x);
    u32 top = static_cast< u32 >(frameOrigin.y);
    top &= ~1;
    left &= ~1;
    MR::convertFrameBufferPosToScreenPos(&minimum, TVec2f(left, top));
    MR::convertFrameBufferPosToScreenPos(&maximum, TVec2f(left + width, top + height));
    GXBegin(GX_QUADS, GX_VTXFMT0, 4);
    GXPosition3f32(minimum.x, minimum.y, 0.0f);
    GXTexCoord2f32(0.0f, 0.0f);
    GXPosition3f32(maximum.x, minimum.y, 0.0f);
    GXTexCoord2f32(4.0f, 0.0f);
    GXPosition3f32(maximum.x, maximum.y, 0.0f);
    GXTexCoord2f32(4.0f, 4.0f);
    GXPosition3f32(minimum.x, maximum.y, 0.0f);
    GXTexCoord2f32(0.0f, 4.0f);
    GXEnd();
    GXSetDstAlpha(GX_FALSE, 0);
    GXSetColorUpdate(GX_TRUE);
    GXSetZCompLoc(GX_TRUE);
    GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
    TDDraw::close();
    return true;
}

void MarioActor::showBeeFur() {
    if (mMario->isPlayerModeBee()) {
        _9E8->appear();
        static_cast< FurMulti* >(_9EC)->onDraw(-1);
    }
    if (getCarrySensor())
        MR::showModel(getCarrySensor()->mHost);
    if (mMario->isPlayerModeInvincible()) {
        MR::showJoint(getJ3DModel(), "Face0");
        _A6E = 2;
        playEffect("無敵中");
    }
}

f32 MarioState::getBlurOffset() const {
    return 0.0f;
}
