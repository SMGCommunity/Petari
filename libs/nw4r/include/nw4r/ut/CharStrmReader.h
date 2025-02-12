#pragma once

#include <revolution.h>

namespace nw4r {
    namespace ut {
        class CharStrmReader {
        public:
            template<typename CharType>
            CharType GetChar(int offset = 0) const {
                const CharType* charStrm = reinterpret_cast<const CharType*>(mCharStrm);
                return *(charStrm + offset);
            }

            template<typename CharType>
            void StepStrm(int step = 1) {
                const CharType*& charStrm = reinterpret_cast<const CharType*&>(mCharStrm);
                charStrm += step;
            }

            typedef u16 (CharStrmReader::*ReadNextCharFunc)();

            u16 ReadNextCharUTF8();
            u16 ReadNextCharUTF16();
            u16 ReadNextCharCP1252();
            u16 ReadNextCharSJIS();

            const void* mCharStrm;
            const ReadNextCharFunc mReadFunc;
        };
    };
};
