#include "nw4r/ut/ut_CharStrmReader.h"

namespace nw4r
{
    namespace ut
    {
        u16 CharStrmReader::ReadNextCharUTF8()
        {
            u16 character;

            if ((Get<u8>(0) & 0x80) == 0)
            {
                character = Get<u8>(0);
                mStream += 1;
            }
            else if ((Get<u8>(0) & 0xE0) == 0xC0)
            {
                character = ((Get<u8>(0) & 0x1F) << 6 | (Get<u8>(1) & 0x3F));
                mStream += 2;
            }
            else
            {
                character = ((Get<u8>(0) & 0x1F) << 12 | ((Get<u8>(1) & 0x3F) << 6) | (Get<u8>(2) & 0x3F));
                mStream += 3;
            }
            
            return character;
        }

        u16 CharStrmReader::ReadNextCharUTF16()
        {
            u16 character = Get<u16>(0);
            mStream += 2;
            return character;
        }

        u16 CharStrmReader::ReadNextCharCP1252()
        {
            u16 character = Get<u8>(0);
            mStream += 1;
            return character;
        }

        u16 CharStrmReader::ReadNextCharSJIS()
        {
            u16 character;

            u16 byte = Get<u8>(0);
            bool flag = byte >= 0x81 && byte < 0xa0 || byte >= 0xe0;

            if (flag)
            {
                character = (Get<u8>(0) << 8) | (Get<u8>(1));
                mStream += 2;
            }
            else
            {
                character = Get<u8>(0);
                mStream += 1;
            }
            
            return character;
        }
    };
};