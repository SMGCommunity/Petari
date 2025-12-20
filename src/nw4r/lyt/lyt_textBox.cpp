#include "nw4r/lyt/layout.h"
#include "nw4r/lyt/textBox.h"
#include "nw4r/ut/Font.h"
#include "nw4r/ut/WideTextWriter.h"
#include <cstdio>
#include <wstring.h>

namespace nw4r {
    namespace lyt {

        TextBox::~TextBox() {
            SetFont(nullptr);

            if (mpMaterial != NULL && !mpMaterial->IsUserAllocated()) {
                mpMaterial->~Material();
                Layout::FreeMemory(mpMaterial);
                mpMaterial = NULL;
            }

            FreeStringBuffer();
        }

        void TextBox::SetVtxColor(u32 idx, ut::Color color) {
            SetTextColor(idx / 2, color);
        }

        u8 TextBox::GetVtxColorElement(u32 idx) const {
            return reinterpret_cast< const u8* >(&mTextColors[idx / (4 * 2)])[idx % 4];
        }

        void TextBox::SetVtxColorElement(u32 idx, u8 value) {
            reinterpret_cast< u8* >(&mTextColors[idx / (4 * 2)])[idx % 4] = value;
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
    };  // namespace lyt
};  // namespace nw4r
