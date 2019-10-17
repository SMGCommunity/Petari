#ifndef LYT_COMMON_H
#define LYT_COMMON_H

#include "types.h"
#include "nw4r/lyt/resources.h"

namespace nw4r
{
    namespace lyt
    {
        namespace detail
        {
            bool TestFileHeader(const res::BinaryFileHeader &, u32);
        };
    };
};

#endif // LYT_COMMON_H