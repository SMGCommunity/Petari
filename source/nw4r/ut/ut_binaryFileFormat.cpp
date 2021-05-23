#include "nw4r/ut/ut_binaryFileFormat.h"

namespace nw4r
{
    namespace ut
    {
        // Determines if a binary file is valid
        // pHeader -> the header to check
        // magic -> the magic of the incoming binary file
        // ver -> the version of the incoming binary file
        // minimumBlockCount -> the minimum amount of blocks needed for the binary file to be valid
        // returns true if valid, false if invalid
        bool IsValidBinaryFile(const BinaryFileHeader *pHeader, u32 magic, u16 ver, u16 minimumBlockCount)
        {
            // the magic has to match the magic defined in the header
            if (pHeader->mMagic != magic)
            {
                return false;
            }

            // the byte order must be 0xFEFF, which denotes a big endian file
            if (pHeader->mByteOrder != 0xFEFF)
            {
                return false;
            }

            // the version has to match the header version
            if (pHeader->mVersion != ver)
            {
                return false;
            }

            // the file size must be larger than all of the file and header blocks combined
            if (pHeader->mFileSize < sizeof(BinaryFileHeader) + sizeof(BinaryBlockHeader) * minimumBlockCount)
            {
                return false;
            }

            // the block count must be higher than the minimum required count
            if (pHeader->mBlockCount < minimumBlockCount)
            {
                return false;
            }

            return true;
        }
    };
};