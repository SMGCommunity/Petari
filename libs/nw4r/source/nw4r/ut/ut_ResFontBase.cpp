#include "nw4r/ut/ResFontBase.h"

namespace nw4r {
    namespace ut {
        namespace detail {
            ResFontBase::ResFontBase() {
                mResource = nullptr;
                mFontInfo = nullptr;
            }

            ResFontBase::~ResFontBase() {

            }

            void ResFontBase::SetResourceBuffer(void *pBuf, FontInformation *pInf) {
                mResource = pBuf;
                mFontInfo = pInf;
            }

            void* ResFontBase::RemoveResourceBuffer() {
                void* data = mResource;
                mResource = nullptr;
                mFontInfo = nullptr;
                return data;
            }

            int ResFontBase::GetWidth() const {
                return mFontInfo->width;
            }

            int ResFontBase::GetHeight() const {
                return mFontInfo->height;
            }

            int ResFontBase::GetAscent() const {
                return mFontInfo->ascent;
            }

            int ResFontBase::GetDescent() const {
                return mFontInfo->height - mFontInfo->ascent;
            }

            int ResFontBase::GetBaselinePos() const {
                return mFontInfo->pGlyph->baselinePos;
            }

            int ResFontBase::GetCellHeight() const {
                return mFontInfo->pGlyph->cellHeight;
            }

            int ResFontBase::GetCellWidth() const {
                return mFontInfo->pGlyph->cellWidth;
            }

            int ResFontBase::GetMaxCharWidth() const {
                return mFontInfo->pGlyph->maxCharWidth;
            }

            Font::Type ResFontBase::GetType() const {
                return Res;
            }

            GXTexFmt ResFontBase::GetTextureFormat() const {
                return static_cast<GXTexFmt>(mFontInfo->pGlyph->sheetFormat);
            }

            int ResFontBase::GetLineFeed() const {
                return mFontInfo->lineFeed;
            }

            const CharWidths ResFontBase::GetDefaultCharWidths() const {
                return mFontInfo->defaultWidth;
            }

            void ResFontBase::SetDefaultCharWidths(const CharWidths &widths) {
                mFontInfo->defaultWidth = widths;
            }

            bool ResFontBase::SetAlternateChar(CharCode c) {
                u16 index = FindGlyphIndex(c);

                if (index != 0xFFFF) {
                    mFontInfo->alterCharIndex = index;
                    return true;
                }

                return false;
            }

            void ResFontBase::SetLineFeed(int feed) {
                mFontInfo->lineFeed = static_cast<s8>(feed);
            }

            int ResFontBase::GetCharWidth(CharCode c) const {
                return GetCharWidths(c).charWidth;
            }

            const CharWidths ResFontBase::GetCharWidths(CharCode c) const {
                u16 idx = GetGlyphIndex(c);
                return GetCharWidthsFromIndex(idx);
            }

            void ResFontBase::GetGlyph(Glyph *pGlyph, CharCode c) const {
                u16 idx = GetGlyphIndex(c);
                GetGlyphFromIndex(pGlyph, idx);
            }

            u16 ResFontBase::GetGlyphIndex(CharCode c) const {
                u16 idx = FindGlyphIndex(c);
                return (idx != 0xFFFF) ? idx : mFontInfo->alterCharIndex;
            }

            FontEncoding ResFontBase::GetEncoding() const {
                return (FontEncoding)mFontInfo->encoding;
            }

            u16 ResFontBase::FindGlyphIndex(CharCode c) const {
                const FontCodeMap* pMap = mFontInfo->pMap;

                while (pMap != nullptr) {
                    if ((pMap->ccodeBegin <= c) && (c <= pMap->ccodeEnd)) {
                        return FindGlyphIndex(pMap, c);
                    }

                    pMap = pMap->pNext;
                }

                return 0xFFFF;
            }

            u16 ResFontBase::FindGlyphIndex(const FontCodeMap *pMap, CharCode c) const {
                u16 index = 0xFFFF;

                switch (pMap->mappingMethod) {
                    case 0:
                    {
                        u16 offs = pMap->mapInfo[0];
                        index = static_cast<u16>(c - pMap->ccodeBegin + offs);
                    }
                    break;

                    case 1:
                    {
                        const int tbl_idx = c - pMap->ccodeBegin;
                        index = pMap->mapInfo[tbl_idx];
                    }
                    break;

                    case 2:
                    {
                        const CMapInfoScan* const scanInfo = reinterpret_cast<const CMapInfoScan*>(pMap->mapInfo);
                        const CMapScanEntry* first = &(scanInfo->entries[0]);
                        const CMapScanEntry* last = &(scanInfo->entries[scanInfo->num - 1]);

                        while (first <= last) {
                            const CMapScanEntry* mid = first + (last - first) / 2;

                            if (mid->ccode < c) {
                                first = mid + 1;
                            }
                            else if (c < mid->ccode) {
                                last = mid - 1;
                            }
                            else {
                                index = mid->index;
                                break;
                            }
                        }
                    }
                    break;
                }

                return index;
            }

            const CharWidths& ResFontBase::GetCharWidthsFromIndex(u16 index) const {
                const FontWidth* pWidth = mFontInfo->pWidth;

                while (pWidth != nullptr) {
                    if ((pWidth->indexBegin <= index) && (index <= pWidth->indexEnd)) {
                        return GetCharWidthsFromIndex(pWidth, index);
                    }

                    pWidth = pWidth->pNext;
                }

                return mFontInfo->defaultWidth;
            }

            const CharWidths& ResFontBase::GetCharWidthsFromIndex(const FontWidth *pWidth, u16 index) const {
                return pWidth->widthTable[index - pWidth->indexBegin];
            }

            void ResFontBase::GetGlyphFromIndex(Glyph *glyph, u16 index) const {
                const FontTextureGlyph& tg = *mFontInfo->pGlyph;
                const u32 cellsInASheet = static_cast<u32>(tg.sheetRow * tg.sheetLine);
                const u32 sheetNo = index / cellsInASheet;
                const u32 cellNo = index % cellsInASheet;
                const u32 cellUnitX = cellNo % tg.sheetRow;
                const u32 cellUnitY = cellNo / tg.sheetRow;
                const u32 cellPixelX = cellUnitX * (tg.cellWidth  + 1);
                const u32 cellPixelY = cellUnitY * (tg.cellHeight + 1);
                const u32 offsetBytes = sheetNo * tg.sheetSize;
                const void* pSheet = tg.sheetImage + offsetBytes;

                glyph->pTexture = pSheet;
                glyph->widths = GetCharWidthsFromIndex(index);
                glyph->height = tg.cellHeight;
                glyph->texFormat = static_cast<GXTexFmt>(tg.sheetFormat);
                glyph->texWidth = tg.sheetWidth;
                glyph->texHeight = tg.sheetHeight;
                glyph->cellX = static_cast<u16>(cellPixelX + 1);
                glyph->cellY = static_cast<u16>(cellPixelY + 1);
            }
        };
    };
};