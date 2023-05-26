#pragma once

#include <revolution.h>

namespace nw4r {
    namespace ut {
        class CharStrmReader {
        public:
            typedef u16 (CharStrmReader::*ReadNextCharFunc)();

            explicit CharStrmReader(ReadNextCharFunc func) : mCharStrm(nullptr), mReadFunc(func) {

            }

            template<typename T>
            T GetChar(int offset) const {
                const T* charStrm = reinterpret_cast<const T*>(mCharStrm);
                return *(charStrm + offset);
            }

            template<typename T>
            void StepStrm(int step) {
                const T*& charStrm = reinterpret_cast<const T*&>(mCharStrm);
                charStrm += step;
            }

            u16 ReadNextCharUTF8();
            u16 ReadNextCharUTF16();
            u16 ReadNextCharCP1252();
            u16 ReadNextCharSJIS();

            const void* mCharStrm;
            const ReadNextCharFunc mReadFunc;
        };
    };
};