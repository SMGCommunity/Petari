#pragma once

#include "nw4r/db/assert.h"

#include <revolution.h>

namespace nw4r {
    namespace ut {
        class CharStrmReader {
        public:
            CharStrmReader(const CharStrmReader& rhs) : mCharStrm(rhs.mCharStrm), mReadFunc(rhs.mReadFunc) {
            }

            typedef u16 (CharStrmReader::*ReadNextCharFunc)();

            explicit CharStrmReader(ReadNextCharFunc func) : mCharStrm(NULL), mReadFunc(func) {
            }

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

            void Set(const char* stream) {
                mCharStrm = stream;
            }

            void Set(const wchar_t* stream) {
                NW4R_POINTER_ASSERT_AT(60, this);
                NW4R_ALIGN2_ASSERT_AT(61, stream);
                NW4R_POINTER_ASSERT_AT(62, stream);
                NW4R_ASSERT_AT(63, mReadFunc == ReadNextCharUTF16);
                mCharStrm = stream;
            }

            u16 Next() {
                NW4R_POINTER_ASSERT_AT(75, this);
                return (this->*mReadFunc)();
            }

            const void* GetCurrentPos() const {
                NW4R_POINTER_ASSERT_AT(69, this);
                return mCharStrm;
            }

            u16 ReadNextCharUTF8();
            u16 ReadNextCharUTF16();
            u16 ReadNextCharCP1252();
            u16 ReadNextCharSJIS();

            const void* mCharStrm;
            const ReadNextCharFunc mReadFunc;
        };
    };  // namespace ut
};  // namespace nw4r
