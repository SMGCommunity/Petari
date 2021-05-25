#pragma once

#include "smg.h"

namespace nw4r
{
    namespace ut
    {
        class CharStrmReader
        {
        public:
            u16 ReadNextCharUTF8();
            u16 ReadNextCharUTF16();
            u16 ReadNextCharCP1252();
            u16 ReadNextCharSJIS();

            template<typename T>
            inline T Get(int offs) const
            {
                const T* strm = reinterpret_cast<const T*>(mStream);
                return strm[offs];
            }

            const char* mStream;
        };
    };
};