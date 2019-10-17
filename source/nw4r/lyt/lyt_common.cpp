#include "nw4r/lyt/lyt_common.h"

namespace nw4r
{
    namespace lyt
    {
        namespace detail
        {
            bool TestFileHeader(const res::BinaryFileHeader &header, u32 magic)
            {
                bool isValid = false;

                if (magic == header.mMagic && header.mByteOrder == res::BYTE_ORDER_MARK)
                {
                    isValid = true;
                }

                return isValid;
            }
        };
    };
};