#include "nw4r/lyt/layout.h"
#include "nw4r/lyt/resourceAccessor.h"
#include "nw4r/lyt/textBox.h"
#include "nw4r/ut/Font.h"
#include "nw4r/ut/ResFont.h"
#include "nw4r/ut/WideTextWriter.h"
#include <cstdio>
#include <wstring.h>

namespace nw4r {
    namespace lyt {

        namespace {
            inline u8 ClampColor(s16 colVal) {
                return u8(colVal < 0 ? 0 : (colVal > 255 ? 255 : colVal));
            }

            ut::Color GetColor(const GXColorS10& src) {
                GXColor dst;
                dst.r = ClampColor(src.r);
                dst.g = ClampColor(src.g);
                dst.b = ClampColor(src.b);
                dst.a = ClampColor(src.a);
                return ut::Color(dst);
            }
        };  // namespace

        TextBox::TextBox(const res::TextBox* pBlock, const ResBlockSet& resBlockSet) : Pane(pBlock) {
            u16 allocStrBufLen = static_cast< u16 >(pBlock->textBufBytes / sizeof(wchar_t));
            if (allocStrBufLen > 0) {
                allocStrBufLen -= 1;
            }

            Init(allocStrBufLen);

            if (pBlock->textStrBytes >= sizeof(wchar_t) && mTextBuf) {
                const wchar_t* const pBlockText = detail::ConvertOffsToPtr< wchar_t >(pBlock, pBlock->textStrOffset);
                const u16 resStrLen = static_cast< u16 >(pBlock->textStrBytes / sizeof(wchar_t) - 1);
                SetString(pBlockText, 0, resStrLen);
            }

            for (int i = 0; i < 2; ++i) {
                mTextColors[i] = pBlock->textCols[i];
            }

            mFontSize = pBlock->fontSize;
            mTextPosition = pBlock->textPosition;
            mBits.textAlignment = pBlock->textAlignment;
            mCharSpace = pBlock->charSpace;
            mLineSpace = pBlock->lineSpace;

            const res::Font* const fonts = detail::ConvertOffsToPtr< res::Font >(resBlockSet.pFontList, sizeof(*resBlockSet.pFontList));
            const char* const fontName = detail::ConvertOffsToPtr< char >(fonts, fonts[pBlock->fontIdx].nameStrOffset);

            if (ut::Font* pFont = resBlockSet.pResAccessor->GetFont(fontName)) {
                mpFont = pFont;
            } else if (void* fontRes = resBlockSet.pResAccessor->GetResource('font', fontName, 0)) {
                ut::ResFont* pResFont = Layout::NewObj< ut::ResFont >();
                pResFont->SetResource(fontRes);

                mpFont = pResFont;
                mBits.bAllocFont = true;
            }

            const u32* const matOffsTbl = detail::ConvertOffsToPtr< u32 >(resBlockSet.pMaterialList, sizeof(*resBlockSet.pMaterialList));
            const res::Material* const pResMaterial =
                detail::ConvertOffsToPtr< res::Material >(resBlockSet.pMaterialList, matOffsTbl[pBlock->materialIdx]);
            mpMaterial = Layout::NewObj< Material >(pResMaterial, resBlockSet);
        }

        void TextBox::Init(u16 allocStrLen) {
            mTextBuf = 0;
            mTextBufBytes = 0;
            mTextLen = 0;
            mpFont = 0;
            mFontSize = Size(0, 0);
            SetTextPositionH(1);
            SetTextPositionV(1);
            mLineSpace = 0;
            mCharSpace = 0;
            mpTagProcessor = 0;
            memset(&mBits, 0, sizeof(mBits));

            if (allocStrLen > 0) {
                AllocStringBuffer(allocStrLen);
            }
        }

        TextBox::~TextBox() {
            SetFont(nullptr);

            if (mpMaterial != NULL && !mpMaterial->IsUserAllocated()) {
                mpMaterial->~Material();
                Layout::FreeMemory(mpMaterial);
                mpMaterial = NULL;
            }

            FreeStringBuffer();
        }

        const ut::Color TextBox::GetVtxColor(u32 idx) const {
            return GetTextColor(idx / 2);
        }

        void TextBox::SetVtxColor(u32 idx, ut::Color color) {
            SetTextColor(idx / 2, color);
        }

        void TextBox::SetTextColor(u32 type, ut::Color value) {
            mTextColors[type] = value;
        }

        u8 TextBox::GetVtxColorElement(u32 idx) const {
            return reinterpret_cast< const u8* >(&mTextColors[idx / (4 * 2)])[idx % 4];
        }

        void TextBox::SetVtxColorElement(u32 idx, u8 value) {
            reinterpret_cast< u8* >(&mTextColors[idx / (4 * 2)])[idx % 4] = value;
        }

        const ut::Rect TextBox::GetTextDrawRect() const {
            ut::WideTextWriter writer;

            return GetTextDrawRect(&writer);
        }

        const ut::Rect TextBox::GetTextDrawRect(const DrawInfo&) const {
            return GetTextDrawRect();
        }

        void TextBox::DrawSelf(const DrawInfo& drawInfo) {
            if (!mTextBuf || !mpFont || !mpMaterial) {
                return;
            }

            LoadMtx(drawInfo);

            ut::WideTextWriter writer;

            ut::Rect textRect = GetTextDrawRect(&writer);

            ut::Color topCol = detail::MultipleAlpha(mTextColors[0], mGlbAlpha);
            ut::Color btmCol = detail::MultipleAlpha(mTextColors[1], mGlbAlpha);
            writer.SetGradationMode(topCol != btmCol ? ut::CharWriter::GRADMODE_V : ut::CharWriter::GRADMODE_NONE);
            writer.SetTextColor(topCol, btmCol);
            ut::Color minCol = GetColor(mpMaterial->GetTevColor(0));
            ut::Color maxCol = GetColor(mpMaterial->GetTevColor(1));
            writer.SetColorMapping(minCol, maxCol);
            writer.SetupGX();

            GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
        }

        void TextBox::SetFont(const ut::Font* pFont) {
            if (mBits.bAllocFont) {
                mpFont->~Font();
                Layout::FreeMemory(const_cast< ut::Font* >(mpFont));
                mBits.bAllocFont = false;
            }

            mpFont = pFont;

            if (mpFont != NULL) {
                SetFontSize(Size(static_cast< f32 >(mpFont->GetWidth()), static_cast< f32 >(mpFont->GetHeight())));
            } else {
                SetFontSize(Size(0.0f, 0.0f));
            }
        }

        const ut::Rect TextBox::GetTextDrawRect(ut::WideTextWriter* pWriter) const {
            ut::Rect rect;

            pWriter->SetCursor(0.0f, 0.0f);

            pWriter->SetFont(*mpFont);
            pWriter->SetFontSize(mFontSize.width, mFontSize.height);

            pWriter->SetLineSpace(mLineSpace);
            pWriter->SetCharSpace(mCharSpace);

            pWriter->SetWidthLimit(mSize.width);
            pWriter->SetDrawFlag(MakeDrawFlag());

            if (mpTagProcessor != NULL) {
                pWriter->SetTagProcessor(mpTagProcessor);
            }

            pWriter->CalcStringRect(&rect, mTextBuf, mTextLen);

            math::VEC2 base = GetVtxPos();
            rect.MoveTo(base.x + (mSize.width - rect.GetWidth()) * GetTextMagH(), base.y + (mSize.height - rect.GetHeight()) * GetTextMagV());

            return rect;
        }

        u16 TextBox::GetStringBufferLength() const {
            if (mTextBufBytes == 0) {
                return 0;
            }

            return mTextBufBytes / sizeof(wchar_t) - 1;
        }

        void TextBox::AllocStringBuffer(u16 len) {
            if (len == 0) {
                return;
            }

            u16 chars = len + 1;
            u16 bytes = chars * sizeof(wchar_t);

            if (bytes > mTextBufBytes) {
                FreeStringBuffer();
                mTextBuf = static_cast< wchar_t* >(Layout::AllocMemory(bytes));

                if (mTextBuf != NULL) {
                    mTextBufBytes = bytes;
                }
            }
        }

        void TextBox::FreeStringBuffer() {
            if (mTextBuf == NULL) {
                return;
            }

            Layout::FreeMemory(mTextBuf);
            mTextBuf = NULL;
            mTextBufBytes = 0;
        }

        u16 TextBox::SetString(const wchar_t* pStr, u16 pos) {
            return SetString(pStr, pos, wcslen(pStr));
        }

        u16 TextBox::SetString(const wchar_t* pStr, u16 pos, u16 len) {
            if (mTextBuf == NULL) {
                return 0;
            }

            const u16 maxlen = GetStringBufferLength();
            if (pos >= maxlen) {
                return 0;
            }

            const u16 chars = ut::Min< u16 >(len, maxlen - pos);
            memcpy(&mTextBuf[pos], pStr, chars * sizeof(wchar_t));

            mTextLen = pos + chars;
            mTextBuf[mTextLen] = L'\0';

            return chars;
        }

        f32 TextBox::GetTextMagH() const {
            f32 mag = 0.0f;

            switch (GetTextPositionH()) {
            default:
            case 0: {
                mag = 0.0f;
                break;
            }

            case 1: {
                mag = 0.5f;
                break;
            }

            case 2: {
                mag = 1.0f;
                break;
            }
            }

            return mag;
        }

        f32 TextBox::GetTextMagV() const {
            f32 mag = 0.0f;

            switch (GetTextPositionV()) {
            default:
            case 0: {
                mag = 0.0f;
                break;
            }

            case 1: {
                mag = 0.5f;
                break;
            }

            case 2: {
                mag = 1.0f;
                break;
            }
            }

            return mag;
        }

        u32 TextBox::MakeDrawFlag() const {
            u32 flag = 0;

            switch (GetTextPositionH()) {
            case 1: {
                flag |= 1;
                break;
            }

            case 2: {
                flag |= 2;
                break;
            }

            default: {
                break;
            }
            }

            return flag;
        }

        f32 TextBox::GetTextAlignMag() const {
            switch (GetTextAlignment()) {
            case 0:
            default:
                return GetTextMagH();
            case 1:
                return 0;
            case 2:
                return 0.5f;
            case 3:
                return 1.0f;
            }
        }
    };  // namespace lyt
};  // namespace nw4r
