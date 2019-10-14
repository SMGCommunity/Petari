#ifndef BINARYFILEFORMAT_H
#define BINARYFILEFORMAT_H

#include "types.h"

namespace nw4r
{
    namespace ut
    {
        struct BinaryFileHeader
        {
            u32 mSignature; // _0
            u16 mByteOrder; // _4
            u16 mVersion; // _6
            u32 mFileSize; // _8
            u16 mHeaderSize; // _C
            u16 mBlockCount; // _E
        };

        bool IsValidBinaryFile(const BinaryFileHeader *, u32, u16, u16);

        const u16 BYTE_ORDER_MARK = 0xFEFF;
    };
};

#endif // BINARYFILEFORMAT_H