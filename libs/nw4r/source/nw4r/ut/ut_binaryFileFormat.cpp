#include "nw4r/ut/binaryFileFormat.h"

namespace nw4r {
    namespace ut {
        bool IsValidBinaryFile(const BinaryFileHeader *header, u32 signature, u16 version, u16 minBlocks) {
            if (header->signature != signature) {
                return false;
            }

            if (header->byteOrder != 0xFEFF) {
                return false;
            }

            if (header->version != version) {
                return false;
            }

            if (header->fileSize < sizeof(BinaryFileHeader) + sizeof(BinaryBlockHeader) * minBlocks) {
                return false;
            }

            if (header->dataBlocks < minBlocks) {
                return false;
            }

            return true;
        }
    };
};