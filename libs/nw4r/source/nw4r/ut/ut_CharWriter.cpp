#include "nw4r/ut/CharWriter.h"

namespace {
    void SetupGXCommon() {
        static const nw4r::ut::Color fog = 0;
        GXSetFog(GX_FOG_NONE, 0.0f, 0.0f, 0.0f, 0.0f, fog);
        GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
        GXSetZTexture(GX_ZT_DISABLE, GX_TF_Z8, 0);
        GXSetNumChans(1);
        GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
        GXSetChanCtrl(GX_COLOR1A1, GX_DISABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
        GXSetNumTexGens(1);
        GXSetTexCoordGen2(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY, GX_FALSE, 0x7D);
        GXSetNumIndStages(0);
        GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
    }
};

namespace nw4r {
    namespace ut {

        CharWriter::CharWriter()  {
            mAlpha = 255;
            mIsWidthFixed = false;
            mFixedWidth = 0;
            mFont = nullptr;
            mLoadingTexture.Reset();
            ResetColorMapping();
            SetGradiationMode(None);
            SetTextColor(Color::WHITE);
            SetScale(1, 1);
            SetCursor(0.0f, 0.0f, 0.0f);
            EnableLinearFilter(true, true);
        }

        void CharWriter::SetColorMapping(Color min, Color max) {
            mColorMapping.min = min;
            mColorMapping.max = max;
        }

        CharWriter::~CharWriter() {

        }

        void CharWriter::SetupGX() {
            mLoadingTexture.Reset();

            if ((mColorMapping.min != 0) || (mColorMapping.max != 0xFFFFFFFF)) {
                SetupGXWithColorMapping(mColorMapping.min, mColorMapping.max);
            }
            else {
                if (mFont != nullptr) {
                    GXTexFmt format = mFont->GetTextureFormat();

                    switch (format) {
                        case GX_TF_I4:
                        case GX_TF_I8:
                            SetupGXForI();
                            break;
                        case GX_TF_IA4:
                        case GX_TF_IA8:
                            SetupGXDefault();
                            break;
                        case GX_TF_RGB565:
                        case GX_TF_RGB5A3:
                        case GX_TF_RGBA8:
                            SetupGXForRGBA();
                            break;
                        
                        default:
                            SetupGXDefault();
                            break;
                    }
                }
                else {
                    SetupGXDefault();
                }
            }
        }

        void CharWriter::SetFontSize(f32 width, f32 height) {
            SetScale(width / mFont->GetWidth(), height / mFont->GetHeight());
        }

        f32 CharWriter::GetFontWidth() const {
            return mFont->GetWidth() * mScale.x;
        }

        f32 CharWriter::GetFontHeight() const {
            return mFont->GetHeight() * mScale.y;
        }

        f32 CharWriter::GetFontAscent() const {
            return mFont->GetAscent() * mScale.y;
        }

        void CharWriter::EnableLinearFilter(bool small, bool large) {
            mFilter.atSmall = small ? GX_LINEAR : GX_NEAR;
            mFilter.atLarge = large ? GX_LINEAR : GX_NEAR;
        }

        void CharWriter::LoadTexture(const Glyph &glyph, GXTexMapID slot) {
            LoadingTexture loadInfo;
            loadInfo.slot = slot;
            loadInfo.texture = glyph.pTexture;
            loadInfo.filter = mFilter;

            if (loadInfo != mLoadingTexture) {
                GXTexObj tobj;
                GXInitTexObj(&tobj, const_cast<void*>(glyph.pTexture), glyph.texWidth, glyph.texHeight, glyph.texFormat, GX_CLAMP, GX_CLAMP, GX_FALSE);
                GXInitTexObjLOD(&tobj, mFilter.atLarge, mFilter.atLarge, 0.0f, 0.0f, 0.0f, GX_DISABLE, GX_DISABLE, GX_ANISO_1);
                GXLoadTexObj(&tobj, slot);
                mLoadingTexture = loadInfo;
            }
        }

        f32 CharWriter::Print(u16 code) {
            Glyph glyph;
            f32 width;
            f32 left;

            mFont->GetGlyph(&glyph, code);

            CharWidths& widths = glyph.widths;

            if (mIsWidthFixed) {
                f32 margin = (mFixedWidth - widths.charWidth * mScale.x) / 2;
                width = mFixedWidth;
                left = margin + widths.left * mScale.x;
            }
            else {
                width = widths.charWidth * mScale.x;
                left = widths.left * mScale.x;
            }

            PrintGlyph(mCursorPos.x + left, mCursorPos.y, mCursorPos.z, glyph);
            mCursorPos.x += width;
            return width;
        }

        void CharWriter::PrintGlyph(f32 x, f32 y, f32 z, const Glyph &glyph) {
            const f32 posLeft = x;
            const f32 posTop = y;
            const f32 posRight = posLeft + glyph.widths.glyphWidth * mScale.x;
            const f32 posBottom = posTop + glyph.height * mScale.y;
            const f32 posZ = z;
            const u16 texLeft = static_cast<u16>(glyph.cellX * 0x8000 / glyph.texWidth);
            const u16 texTop = static_cast<u16>(glyph.cellY * 0x8000 / glyph.texHeight);
            const u16 texRight = static_cast<u16>((glyph.cellX + glyph.widths.glyphWidth) * 0x8000 / glyph.texWidth);
            const u16 texBottom = static_cast<u16>((glyph.cellY + glyph.height) * 0x8000 / glyph.texHeight);
        
            LoadTexture(glyph, GX_TEXMAP0);

            GXBegin(GX_QUADS, GX_VTXFMT0, 4);
            GXPosition3f32(posLeft, posTop, posZ );
            GXColor1u32(mVertexColor.lu);
            GXTexCoord2u16(texLeft, texTop);

            GXPosition3f32(posRight, posTop, posZ);
            GXColor1u32(mVertexColor.ru);
            GXTexCoord2u16(texRight, texTop);

            GXPosition3f32(posRight, posBottom, posZ);
            GXColor1u32(mVertexColor.rd);
            GXTexCoord2u16(texRight, texBottom);

            GXPosition3f32(posLeft, posBottom, posZ);
            GXColor1u32(mVertexColor.ld);
            GXTexCoord2u16(texLeft, texBottom);

            GXEnd();
        }

        void CharWriter::UpdateVertexColor() {
            mVertexColor.lu = mTextColor.start;
            mVertexColor.ru = (mTextColor.mode != Horizonal) ? mTextColor.start: mTextColor.end;
            mVertexColor.ld = (mTextColor.mode != Vertical) ? mTextColor.start: mTextColor.end;
            mVertexColor.rd = (mTextColor.mode == None) ? mTextColor.start: mTextColor.end;

            mVertexColor.lu.a = static_cast<u8>(mVertexColor.lu.a * mAlpha / 255);
            mVertexColor.ru.a = static_cast<u8>(mVertexColor.ru.a * mAlpha / 255);
            mVertexColor.ld.a = static_cast<u8>(mVertexColor.ld.a * mAlpha / 255);
            mVertexColor.rd.a = static_cast<u8>(mVertexColor.rd.a * mAlpha / 255);
        }

        void CharWriter::SetupVertexFormat() {
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XYZ, GX_F32, 0);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
            GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_TEX0, GX_TEX_ST, GX_U16, 0xF);
            GXClearVtxDesc();
            GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
            GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
            GXSetVtxDesc(GX_VA_TEX0, GX_DIRECT);
        }

        void CharWriter::SetupGXDefault() {
            SetupGXCommon();

            GXSetNumTevStages(1);
            GXSetTevDirect(GX_TEVSTAGE0);
            GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
            GXSetTevOp(GX_TEVSTAGE0, GX_MODULATE);

            SetupVertexFormat();
        }

        void CharWriter::SetupGXWithColorMapping(Color min, Color max) {
            SetupGXCommon();

            GXSetNumTevStages(2);
            GXSetTevDirect(GX_TEVSTAGE0);
            GXSetTevDirect(GX_TEVSTAGE1);
            GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
            GXSetTevSwapMode(GX_TEVSTAGE1, GX_TEV_SWAP0, GX_TEV_SWAP0);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
            GXSetTevColor(GX_TEVREG0, min);
            GXSetTevColor(GX_TEVREG1, max);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_C0, GX_CC_C1, GX_CC_TEXC, GX_CC_ZERO);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_A0, GX_CA_A1, GX_CA_TEXA, GX_CA_ZERO);
            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
            GXSetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_CPREV, GX_CC_RASC, GX_CC_ZERO);
            GXSetTevAlphaIn(GX_TEVSTAGE1, GX_CA_ZERO, GX_CA_APREV, GX_CA_RASA, GX_CA_ZERO);
            GXSetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevAlphaOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
        
            SetupVertexFormat();
        }

        void CharWriter::SetupGXForI() {
            SetupGXCommon();
            GXSetNumTevStages(1);
            GXSetTevDirect(GX_TEVSTAGE0);
            GXSetTevSwapMode(GX_TEVSTAGE0, GX_TEV_SWAP0, GX_TEV_SWAP0);
            GXSetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
            GXSetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
            GXSetTevAlphaIn(GX_TEVSTAGE0, GX_CA_ZERO, GX_CA_TEXA, GX_CA_RASA, GX_CA_ZERO);
            GXSetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            GXSetTevAlphaOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
            SetupVertexFormat();
        }

        void CharWriter::SetupGXForRGBA() {
            SetupGXDefault();
        }
    };
};