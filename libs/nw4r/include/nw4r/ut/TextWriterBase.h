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

            TagProcessorBase< CharT >* GetTagProcessor() const { return mTagProcessor; }
            void SetTagProcessor(TagProcessorBase< CharT >* pProcessor) { mTagProcessor = pProcessor; }

            void CalcStringRect(Rect* pRect, const CharT* pStr, int len) const;

            f32 mWidthLimit;
            f32 mCharSpace;
            f32 mLineSpace;
            int mTabWidth;
            u32 mDrawFlag;
            TagProcessor* mTagProcessor;
        };
    };  // namespace ut
};  // namespace nw4r
