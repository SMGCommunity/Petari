#include "nw4r/lyt/layout.h"
#include "nw4r/lyt/lyt_common.h"

/* incomplete */
namespace nw4r
{
    namespace lyt
    {
        bool Layout::Build(const void *pSrc, ResourceAccessor *pAccessor)
        {
            res::BinaryFileHeader* header = (res::BinaryFileHeader*)pSrc;

            // magic check
            if (!detail::TestFileHeader(*header, 0x524C5954))
            {
                return 0;
            }

            bool isCorrectVersion = false;

            if (((header->mVersion << 8) != 0) && ((header->mVersion & 0xFF) < 9))
            {
                isCorrectVersion = true;
            }

            if (!isCorrectVersion)
            {
                return 0;
            }

            u16 curSectionIdx = 0;
            const void *blockPtr = header->mHeaderSize + header;

            u32 textureOffset;

            while (curSectionIdx < header->mNumBlocks)
            {
                res::DataBlockHeader* curHeader = (res::DataBlockHeader*)blockPtr;

                if (curHeader->mKind == 0x70616531)
                {
                    
                }

                if (curHeader->mKind >= 0x70616531)
                {
                    if (curHeader->mKind == 0x74786C31)
                    {
                        textureOffset = (u32)curHeader;
                    }

                    if (curHeader->mKind >= 0x74786C31)
                    {
                        if (curHeader->mKind == 0x776E6431)
                        {

                        }

                        
                    }
                }

                curHeader += curHeader->mSize;
            }

            return 1;
        }
    };
};