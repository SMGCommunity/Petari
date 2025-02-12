#pragma once

#include <revolution.h>

namespace nw4r {
    namespace ut {
        typedef u32 SigWord;

        struct BinaryFileHeader {
            SigWord signature;
            u16 byteOrder;
            u16 version;
            u32 fileSize;
            u16 headerSize;
            u16 dataBlocks;
        };

        struct BinaryBlockHeader {
            SigWord kind;
            u32 size;
        };

        bool IsValidBinaryFile(const BinaryFileHeader *, u32, u16, u16);
    };
};
