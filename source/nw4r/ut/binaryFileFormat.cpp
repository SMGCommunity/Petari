#include "nw4r/ut/binaryFileFormat.h"

namespace nw4r
{
    namespace ut
    {
        bool IsValidBinaryFile(const nw4r::ut::BinaryFileHeader* header, u32 sig, u16 version, u16 minBlockCount = 1)
        {
            if (header->mSignature != sig)
            {
                return 0;
            }
            
            if (header->mByteOrder != BYTE_ORDER_MARK)
            {
                return 0;
            }
            
            if (header->mVersion != version)
            {
                return 0;
            }            
            
            if (((minBlockCount & 0xFFFF) << 3) + 0x10 != header->mFileSize)
            {
                // todo
            }

            return 1;
        }
    };
};