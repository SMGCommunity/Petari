
#pragma once

#include <revolution.h>

namespace nw4r {
    namespace lyt {
        namespace res {
            struct BinaryFileHeader {
                char signature[4];
                u16 byteOrder;
                u16 version;
                u32 fileSize;
                u16 headerSize;
                u16 dataBlocks;
            };
        };
    };
};