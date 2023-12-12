#pragma once

#include <revolution.h>
#include "nw4r/ut/binaryFileFormat.h"

namespace nw4r {
    namespace ut {
        const u32 BINFILE_SIG_FONT              = NW4R_UT_MAKE_SIGWORD('R','F','N','T');
        const u32 BINFILE_SIG_FONT_RESOLEVED    = NW4R_UT_MAKE_SIGWORD('R','F','N','U');
        const u32 BINFILE_SIG_FONTA             = NW4R_UT_MAKE_SIGWORD('R','F','N','A');
        const u32 BINBLOCK_SIG_FINF             = NW4R_UT_MAKE_SIGWORD('F','I','N','F');
        const u32 BINBLOCK_SIG_CGLP             = NW4R_UT_MAKE_SIGWORD('C','G','L','P');
        const u32 BINBLOCK_SIG_TGLP             = NW4R_UT_MAKE_SIGWORD('T','G','L','P');
        const u32 BINBLOCK_SIG_CWDH             = NW4R_UT_MAKE_SIGWORD('C','W','D','H');
        const u32 BINBLOCK_SIG_CMAP             = NW4R_UT_MAKE_SIGWORD('C','M','A','P');
        const u32 BINBLOCK_SIG_GLGR             = NW4R_UT_MAKE_SIGWORD('G','L','G','R');
        const u32 BINBLOCK_SIG_HTGL             = NW4R_UT_MAKE_SIGWORD('H','T','G','L');

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