#include "nw4r/ut/Font.h"

namespace nw4r
{
    namespace ut
    {
        void Font::InitReaderFunc(FontEncoding e)
        {
            // TODO
        }

        const CharStrmReader Font::GetCharStrmReader() const
        {
            CharStrmReader reader(mReaderFunc);
            return reader;
        }
    };
};