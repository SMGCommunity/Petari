#pragma once

#include "Game/Screen/ImageEffectBase.hpp"

class JUTTexture;

class BloomEffect : public ImageEffectBase {
public:
    enum BLOOM_TEX_DRAW_TYPE {
        DrawType_0 = 0,
        DrawType_1 = 1,
        DrawType_2 = 2,
    };

    /// @brief Creates a new `BloomEffect`.
    /// @param pName The pointer to the null-terminated name of the object.
    BloomEffect(const char* pName);

    void preDraw() const;
    void postDraw() const;
    u8 getIntensity1Default() const;
    u8 getIntensity2Default() const;
    bool isForEachModel() const;
    void initDraw() const;
    void drawTexture(JUTTexture*, s32, s32, u8, BLOOM_TEX_DRAW_TYPE) const;
    void blurTexture(JUTTexture*, MtxPtr, s32, s32, u32, f32, u32, const f32*) const;
    void initBlurMtx(MtxPtr, u32, f32, f32) const;
    void initBlur(JUTTexture*, u32, f32) const;
    void drawBlur(s32, s32, MtxPtr, u32, u32) const;

    /* 0x14 */ u8 _14;
    /* 0x18 */ f32 _18;
    /* 0x1C */ f32 _1C;
    /* 0x20 */ u8 _20;
    /* 0x24 */ JUTTexture* _24;
    /* 0x28 */ JUTTexture* _28;
    /* 0x2C */ JUTTexture* _2C;
    /* 0x30 */ JUTTexture* _30;
    /* 0x34 */ JUTTexture* _34;
    /* 0x38 */ JUTTexture* _38;
    /* 0x3C */ JUTTexture* _3C;
    /* 0x40 */ JUTTexture* _40;
    /* 0x44 */ JUTTexture* _44;
    /* 0x48 */ void* _48;
    /* 0x4C */ void* _4C;
};
