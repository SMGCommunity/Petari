#include "nw4r/ut/ResFont.h"

namespace nw4r {
    namespace ut {
        namespace {
            template<typename T>
            inline void ResolveOffset(T*& ptr, void* base) {
                ptr = reinterpret_cast<T*>(reinterpret_cast<u8*>(base) + reinterpret_cast<u32>(ptr));
            }
        };

        ResFont::ResFont() {

        }

        ResFont::~ResFont() {

        }

        bool ResFont::SetResource(void *brfnt) {
            FontInformation* pFontInfo = nullptr;
            BinaryFileHeader* header = reinterpret_cast<BinaryFileHeader*>(brfnt);

            if (!IsManaging(nullptr)) {
                return false;
            }

            if (header->signature == 0x52464E55) {
                BinaryBlockHeader* blockHeader;
                int nBlocks = 0;

                blockHeader = reinterpret_cast<BinaryBlockHeader*>(reinterpret_cast<u8*>(header) + header->headerSize);
            
                while (nBlocks < header->dataBlocks) {
                    if (blockHeader->kind == 0x46494E46) {
                        pFontInfo = reinterpret_cast<FontInformation*>(reinterpret_cast<u8*>(blockHeader) + sizeof(*blockHeader));
                        break;
                    }

                    blockHeader = reinterpret_cast<BinaryBlockHeader*>(reinterpret_cast<u8*>(blockHeader) + blockHeader->size);
                    nBlocks++;
                }
            }
            else {
                if (header->version == 0x104) {
                    if (!IsValidBinaryFile(header, 0x52464E54, 260, 2)) { 
                        return false;
                    }
                }
                else if (!IsValidBinaryFile(header, 0x52464E54, 258, 2)) {
                    return false;
                }

                pFontInfo = Rebuild(header);
            }

            if (pFontInfo == nullptr) {
                return false;
            }

            SetResourceBuffer(brfnt, pFontInfo);
            InitReaderFunc(GetEncoding());
            return true;
        }

        void* ResFont::RemoveResource() {
            return RemoveResourceBuffer();
        }

        // the NW4R SigWord macros are required here, without them the order of the signatures is wrong
        FontInformation* ResFont::Rebuild(BinaryFileHeader* fileHeader) {
            BinaryBlockHeader* blockHeader;
            FontInformation* info = nullptr;
            int nBlocks = 0;

            blockHeader = reinterpret_cast<BinaryBlockHeader*>(reinterpret_cast<u8*>(fileHeader) + fileHeader->headerSize);

            while (nBlocks < fileHeader->dataBlocks) {
                switch (blockHeader->kind) {
                    case BINBLOCK_SIG_FINF:
                    {
                        info = reinterpret_cast<FontInformation*>(reinterpret_cast<u8*>(blockHeader) + sizeof(*blockHeader));
                        ResolveOffset(info->pGlyph, fileHeader);

                        if (info->pWidth != nullptr) {
                            ResolveOffset(info->pWidth, fileHeader);
                        }

                        if (info->pMap != nullptr) {
                            ResolveOffset(info->pMap, fileHeader);
                        }

                        break;
                    }

                    case BINBLOCK_SIG_TGLP:
                    {
                        FontTextureGlyph* glyph = reinterpret_cast<FontTextureGlyph*>(reinterpret_cast<u8*>(blockHeader) + sizeof(*blockHeader));
                        ResolveOffset(glyph->sheetImage, fileHeader);
                    }
                    break;

                    case BINBLOCK_SIG_CWDH:
                    {
                        FontWidth* width = reinterpret_cast<FontWidth*>(reinterpret_cast<u8*>(blockHeader) + sizeof(*blockHeader));
                        
                        if (width->pNext != nullptr) {
                            ResolveOffset(width->pNext, fileHeader);
                        }
                    }
                    break;
                        

                    case BINBLOCK_SIG_CMAP:
                    {
                        FontCodeMap* map = reinterpret_cast<FontCodeMap*>(reinterpret_cast<u8*>(blockHeader) + sizeof(*blockHeader));
                        
                        if (map->pNext != nullptr) {
                            ResolveOffset(map->pNext, fileHeader);
                        }
                    }
                    break;

                    case BINBLOCK_SIG_GLGR:
                    {
                        break;
                    }
                    default:
                        return nullptr;
                }

                blockHeader = reinterpret_cast<BinaryBlockHeader*>(reinterpret_cast<u8*>(blockHeader) + blockHeader->size);
                nBlocks++;
            }

            fileHeader->signature = 0x52464E55;
            return info;
        }
    };
};