#pragma once

#include <revolution.h>

namespace nw4r {
    namespace ut {
        class CharStrmReader {
        public:
            CharStrmReader(const CharStrmReader& rhs) : mCharStrm(rhs.mCharStrm), mReadFunc(rhs.mReadFunc) {}

            typedef u16 (CharStrmReader::*ReadNextCharFunc)();

            explicit CharStrmReader(ReadNextCharFunc func) : mCharStrm(NULL), mReadFunc(func) {}

            template < typename CharType >
            CharType GetChar(int offset = 0) const {
                const CharType* charStrm = reinterpret_cast< const CharType* >(mCharStrm);
                return *(charStrm + offset);
            }

            template < typename CharType >
            void StepStrm(int step = 1) {
                const CharType*& charStrm = reinterpret_cast< const CharType*& >(mCharStrm);
                charStrm += step;
            }

            void Set(const char* stream) { mCharStrm = stream; }

            void Set(const wchar_t* stream) { mCharStrm = stream; }

            u16 Next() { return (this->*mReadFunc)(); }

            const void* GetCurrentPos() const { return mCharStrm; }

            u16 ReadNextCharUTF8();
            u16 ReadNextCharUTF16();
            u16 ReadNextCharCP1252();
            u16 ReadNextCharSJIS();

            const void* mCharStrm;
            const ReadNextCharFunc mReadFunc;
        };
    };  // namespace ut
};  // namespace nw4r
