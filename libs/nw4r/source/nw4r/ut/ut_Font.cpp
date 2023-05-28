#include "nw4r/ut/Font.h"

namespace nw4r {
    namespace ut {
        void Font::InitReaderFunc(FontEncoding enc) {
            switch (enc) {
                case UTF8:
                    mReaderFunc = &CharStrmReader::ReadNextCharUTF8;
                    break;
                case UTF16:
                    mReaderFunc = &CharStrmReader::ReadNextCharUTF16;
                    break;
                case SJIS:
                    mReaderFunc = &CharStrmReader::ReadNextCharSJIS;
                    break;
                case CP1252:
                default:
                    mReaderFunc = &CharStrmReader::ReadNextCharCP1252;
                    break;
            }
        }
    };
};