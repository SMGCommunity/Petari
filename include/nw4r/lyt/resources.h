#ifndef RESOURCES_H
#define RESOURCES_H

#include "types.h"

namespace nw4r
{
    namespace lyt
    {
        namespace res
        {
            const u16 BYTE_ORDER_MARK = 0xFEFF;
            struct BinaryFileHeader
            {
                u32 mMagic; // _0
                u16 mByteOrder; // _4
                u16 mVersion; // _6
                u32 mFileSize; // _8
                u16 mHeaderSize; // _C
                u16 mNumBlocks; // _E
            };

            struct DataBlockHeader
            {
                u32 mKind; // _0
                u32 mSize; // _4
            };
        };
    };
};

#endif // RESOURCES_H