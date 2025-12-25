#pragma once

#include "nw4r/ut/CharWriter.h"
#include "nw4r/ut/TagProcessorBase.h"

namespace nw4r {
    namespace ut {
        template < typename CharT >
        class TextWriterBase : public CharWriter {
        public:
            typedef CharT CharType;
            typedef const CharType* StreamType;
            typedef TagProcessorBase< CharType > TagProcessor;
            typedef TextWriterBase< CharType > SelfType;

            enum PositionFlag {
                HORIZONTAL_ALIGN_LEFT = 0x0,
                HORIZONTAL_ALIGN_CENTER = 0x1,
                HORIZONTAL_ALIGN_RIGHT = 0x2,
                HORIZONTAL_ALIGN_MASK = 0x3,

                HORIZONTAL_ORIGIN_LEFT = 0x00,
                HORIZONTAL_ORIGIN_CENTER = 0x10,
                HORIZONTAL_ORIGIN_RIGHT = 0x20,
                HORIZONTAL_ORIGIN_MASK = 0x30,

                VERTICAL_ORIGIN_TOP = 0x000,
                VERTICAL_ORIGIN_MIDDLE = 0x100,
                VERTICAL_ORIGIN_BOTTOM = 0x200,
                VERTICAL_ORIGIN_BASELINE = 0x300,
                VERTICAL_ORIGIN_MASK = 0x300,
                VERTICAL_BASE_TOP = 0x0000,
                VERTICAL_BASE_BASELINE = VERTICAL_ORIGIN_BASELINE
            };

            enum ContextFlag { CONTEXT_NO_CHAR_SPACE = 0x1 };

            static const u32 DEFAULT_DRAWFLAG = (HORIZONTAL_ALIGN_LEFT | HORIZONTAL_ORIGIN_LEFT | VERTICAL_ORIGIN_TOP);

            TextWriterBase();
            ~TextWriterBase();

            f32 GetWidthLimit() const { return mWidthLimit; }
            void SetWidthLimit(f32 limit) { mWidthLimit = limit; }
            f32 GetCharSpace() const { return mCharSpace; }
            void SetCharSpace(f32 space) { mCharSpace = space; }
            f32 GetLineSpace() const { return mLineSpace; }
            void SetLineSpace(f32 space) { mLineSpace = space; }
            int GetTabWidth() const { return mTabWidth; }
            void SetTabWidth(int width) { mTabWidth = width; }
            u32 GetDrawFlag() const { return mDrawFlag; }
            void SetDrawFlag(u32 flag) { mDrawFlag = flag; }

            f32 GetLineHeight() const;

            TagProcessorBase< CharT >* GetTagProcessor() const { return mTagProcessor; }
            void SetTagProcessor(TagProcessorBase< CharT >* pProcessor) { mTagProcessor = pProcessor; }
            f32 CalcStringWidth(StreamType str, int length) const;
            void CalcStringRect(Rect* pRect, const CharT* pStr, int len) const;
            bool CalcLineRectImpl(Rect*, StreamType*, int);
            void CalcStringRectImpl(Rect*, StreamType, int);
            f32 Print(StreamType str, int length);
            f32 PrintImpl(StreamType, int);
            f32 CalcLineWidth(StreamType, int);
            f32 AdjustCursor(f32*, f32*, StreamType, int);

            bool IsDrawFlagSet(u32 mask, u32 flag) const { return (mDrawFlag & mask) == flag; }

            static TagProcessor mDefaultTagProcessor;

            f32 mWidthLimit;
            f32 mCharSpace;
            f32 mLineSpace;
            int mTabWidth;
            u32 mDrawFlag;
            TagProcessor* mTagProcessor;
        };
    };  // namespace ut
};  // namespace nw4r
