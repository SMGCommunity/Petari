#pragma once

#include <revolution.h>

 #define NW4R_UT_MAKE_SIGWORD(a,b,c,d)        \
    static_cast<u32>(        \
          (static_cast<u8>(a) << 24)        \
        | (static_cast<u8>(b) << 16)        \
        | (static_cast<u8>(c) <<  8)        \
        | (static_cast<u8>(d) <<  0)        \
    )

namespace nw4r {
    namespace ut {
        struct BinaryFileHeader {
            u32 signature;
            u16 byteOrder;
            u16 version;
            u32 fileSize;
            u16 headerSize;
            u16 dataBlocks;
        };

        struct BinaryBlockHeader {
            u32 kind;
            u32 size;
        };

        bool IsValidBinaryFile(const BinaryFileHeader *, u32, u16, u16);
    };
};