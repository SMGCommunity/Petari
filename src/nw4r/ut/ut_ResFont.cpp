#include "nw4r/ut/ResFont.h"
#include "nw4r/ut/binaryFileFormat.h"

namespace nw4r {
    namespace ut {
        namespace {
            template < typename T >
            inline void ResolveOffset(T*& rpPtr, void* pBase) {
                rpPtr = reinterpret_cast< T* >(static_cast< char* >(pBase) + reinterpret_cast< s32 >(rpPtr));
            }

        }  // namespace

        ResFont::ResFont() {}

        ResFont::~ResFont() {}

        bool ResFont::SetResource(void* brfnt) {
            FontInformation* pFontInfo = NULL;
            BinaryFileHeader* fileHeader = reinterpret_cast< BinaryFileHeader* >(brfnt);

            if (!IsManaging(NULL)) {
                return false;
            }
            if (fileHeader->signature == 'RFNU') {
                BinaryBlockHeader* blockHeader;
                int nBlocks = 0;

                blockHeader = reinterpret_cast< BinaryBlockHeader* >(reinterpret_cast< u8* >(fileHeader) + fileHeader->headerSize);

                while (nBlocks < fileHeader->dataBlocks) {
                    if (blockHeader->kind == 'FINF') {
                        pFontInfo = reinterpret_cast< FontInformation* >(reinterpret_cast< u8* >(blockHeader) + sizeof(*blockHeader));
                        break;
                    }

                    blockHeader = reinterpret_cast< BinaryBlockHeader* >(reinterpret_cast< u8* >(blockHeader) + blockHeader->size);
                    nBlocks++;
                }
            } else {
                if (fileHeader->version == 0x104) {
                    if (!IsValidBinaryFile(fileHeader, 'RFNT', 0x104, 2)) {
                        return false;
                    }
                } else {
                    if (!IsValidBinaryFile(fileHeader, 'RFNT', 0x102, 2)) {
                        return false;
                    }
                }
                pFontInfo = Rebuild(fileHeader);
            }

            if (pFontInfo == NULL) {
                return false;
            }

            SetResourceBuffer(brfnt, pFontInfo);
            InitReaderFunc(GetEncoding());

            return true;
        }

        void ResFont::RemoveResource() {
            RemoveResourceBuffer();
        }

        FontInformation* ResFont::Rebuild(BinaryFileHeader* fileHeader) {
            BinaryBlockHeader* blockHeader;
            FontInformation* info = nullptr;
            int nBlocks = 0;

            blockHeader = reinterpret_cast< BinaryBlockHeader* >(reinterpret_cast< u8* >(fileHeader) + fileHeader->headerSize);

            while (nBlocks < fileHeader->dataBlocks) {
                switch (blockHeader->kind) {
                case 'FINF': {
                    info = reinterpret_cast< FontInformation* >(reinterpret_cast< u8* >(blockHeader) + sizeof(*blockHeader));
                    ResolveOffset(info->pGlyph, fileHeader);

                    if (info->pWidth != nullptr) {
                        ResolveOffset(info->pWidth, fileHeader);
                    }
                    if (info->pMap != nullptr) {
                        ResolveOffset(info->pMap, fileHeader);
                    }
                } break;

                case 'TGLP': {
                    FontTextureGlyph* glyph = reinterpret_cast< FontTextureGlyph* >(reinterpret_cast< u8* >(blockHeader) + sizeof(*blockHeader));
                    ResolveOffset(glyph->sheetImage, fileHeader);
                } break;

                case 'CWDH': {
                    FontWidth* width = reinterpret_cast< FontWidth* >(reinterpret_cast< u8* >(blockHeader) + sizeof(*blockHeader));

                    if (width->pNext != nullptr) {
                        ResolveOffset(width->pNext, fileHeader);
                    }
                } break;

                case 'CMAP': {
                    FontCodeMap* map = reinterpret_cast< FontCodeMap* >(reinterpret_cast< u8* >(blockHeader) + sizeof(*blockHeader));

                    if (map->pNext != nullptr) {
                        ResolveOffset(map->pNext, fileHeader);
                    }
                } break;

                case 'GLGR': {
                } break;
                default:
                    return nullptr;
                }

                blockHeader = reinterpret_cast< BinaryBlockHeader* >(reinterpret_cast< u8* >(blockHeader) + blockHeader->size);
                nBlocks++;
            }

            fileHeader->signature = 'RFNU';

            return info;
        };  // namespace ut
    };  // namespace ut
};  // namespace nw4r
