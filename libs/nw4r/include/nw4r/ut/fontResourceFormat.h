#pragma once

#include <revolution.h>

namespace nw4r {
    namespace ut {
        enum FontEncoding {
            UTF8,
            UTF16,
            SJIS,
            CP1252
        };

        struct CharWidths {
            s8 left;
            u8 glyphWidth;
            s8 charWidth;
        };

        struct CMapScanEntry {
            u16 ccode;
            u16 index;
        };

        struct CMapInfoScan {
            u16 num;
            CMapScanEntry entries[];  
        };

        struct FontTextureGlyph {
            u8 cellWidth;
            u8 cellHeight;
            s8 baselinePos;
            u8 maxCharWidth;
            u32 sheetSize;
            u16 sheetNum;
            u16 sheetFormat;
            u16 sheetRow;
            u16 sheetLine;
            u16 sheetWidth;
            u16 sheetHeight;
            u8* sheetImage;
        };

        struct FontWidth {
            u16 indexBegin;
            u16 indexEnd;
            FontWidth* pNext;
            CharWidths widthTable[];
        };

        struct FontCodeMap {
            u16 ccodeBegin;
            u16 ccodeEnd;
            u16 mappingMethod;
            u16 pad;
            FontCodeMap* pNext;
            u16 mapInfo[];
        };

        struct FontInformation {
            u8 fontType;
            s8 lineFeed;
            u16 alterCharIndex;
            CharWidths defaultWidth;
            u8 encoding;
            FontTextureGlyph* pGlyph;
            FontWidth* pWidth;
            FontCodeMap* pMap;
            u8 height;
            u8 width;
            u8 ascent;
            u8 pad;
        };
    };
};