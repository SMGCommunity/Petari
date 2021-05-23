#pragma once

#include "types.h"

namespace nw4r
{
    namespace ut
    {
        struct BinaryFileHeader
        {
            u32 mMagic; // _0
            u16 mByteOrder; // _4
            u16 mVersion; // _6
            u32 mFileSize; // _8
            u16 mHeaderSize; // _C
            u16 mBlockCount; // _E
        };

        struct BinaryBlockHeader
        {
            u32 mType; // _0
            u32 mSize; // _4
        };

        bool IsValidBinaryFile(const BinaryFileHeader *pHeader, u32 sig, u16 version, u16 minimumBlockCount);
    };
};