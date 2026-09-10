#include "nw4r/ut/RomFont.h"

namespace nw4r {
    namespace ut {
        RomFont::RomFont() : mFontHeader(NULL), mAlternateChar('?') {
            mDefaultWidths.left = 0;
            mDefaultWidths.glyphWidth = 0;
            mDefaultWidths.charWidth = 0;
        }

        RomFont::~RomFont() {
        }

    }
}
