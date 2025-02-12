#pragma once

#include "nw4r/ut/CharWriter.h"
#include "nw4r/ut/TagProcessorBase.h"

namespace nw4r {
    namespace ut {
        template<typename CharT>
        class TextWriterBase : public CharWriter {
        public:
            typedef CharT                       CharType;
            typedef const CharType*             StreamType;
            typedef TagProcessorBase<CharType>  TagProcessor;
            typedef TextWriterBase<CharType>    SelfType;

            f32 GetLineHeight() const;

            f32 mWidthLimit;
            f32 mCharSpace;
            f32 mLineSpace;
            int mTabWidth;
            u32 mDrawFlag;
            TagProcessor* mTagProcessor;
        };
    };
};
