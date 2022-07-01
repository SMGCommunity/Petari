#pragma once

#include "nw4r/ut/CharWriter.h"
#include "nw4r/ut/TagProcessor.h"

namespace nw4r {
    namespace ut {
        template<typename T>
        class TextWriterBase : public CharWriter {
        public:
            f32 mWidthLimit;
            f32 mCharSpace;
            f32 mLineSpace;
            int mTabWidth;
            u32 mDrawFlag;
            TagProcessor* mTagProcessor;
        };
    };
};