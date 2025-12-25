#pragma once

#include "nw4r/lyt/common.h"
#include "nw4r/lyt/pane.h"
#include "nw4r/ut/Color.h"
#include "nw4r/ut/RuntimeTypeInfo.h"

namespace nw4r {
    namespace ut {
        class Font;
        template < typename CharType >
        class TagProcessorBase;

        template < typename CharType >
        class TextWriterBase;
    };  // namespace ut

    namespace lyt {
        class DrawInfo;
        struct ResBlockSet;

        class TextBox : public Pane {
        public:
            NW4R_UT_RUNTIME_TYPEINFO;

            TextBox(const res::TextBox*, const ResBlockSet&);

            virtual ~TextBox();
            virtual void DrawSelf(const DrawInfo& drawInfo);
            virtual const ut::Color GetVtxColor(u32) const;
            virtual void SetVtxColor(u32, ut::Color);
            virtual u8 GetVtxColorElement(u32) const;
            virtual void SetVtxColorElement(u32, u8);
            virtual void AllocStringBuffer(u16);
            virtual void FreeStringBuffer();
            virtual u16 SetString(const wchar_t*, u16 dstIdx = 0);
            virtual u16 SetString(const wchar_t*, u16, u16);

            const ut::Rect GetTextDrawRect(ut::TextWriterBase< wchar_t >*) const;
            void SetTextColor(ut::Color, ut::Color);
            void SetFont(const ut::Font*);
            u16 GetStringBufferLength() const;
            u32 MakeDrawFlag() const;
            f32 GetTextMagH() const;
            f32 GetTextMagV() const;

            void SetFontSize(const Size& rFontSize) { mFontSize = rFontSize; }

            u8 GetTextPositionH() const { return detail::GetHorizontalPosition(mTextPosition); }
            u8 GetTextPositionV() const { return detail::GetVerticalPosition(mTextPosition); }

            wchar_t* mTextBuf;
            ut::Color mTextColors[2];
            const ut::Font* mpFont;
            Size mFontSize;
            f32 mLineSpace;
            f32 mCharSpace;
            ut::TagProcessorBase< wchar_t >* mpTagProcessor;
            u16 mTextBufBytes;
            u16 mTextLen;
            u8 mTextPosition;
            struct {
                u8 bAllocFont : 1;
                u8 textAlignment : 2;
            } mBits;
        };
    };  // namespace lyt
};  // namespace nw4r
