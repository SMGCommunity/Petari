#pragma once

#include "nw4r/ut/CharStrmReader.h"
#include "nw4r/ut/fontResourceFormat.h"
#include <revolution.h>

namespace nw4r {
    namespace ut {
        typedef u16 CharCode;

        enum FontEncoding {
            FONT_ENCODING_UTF8,
            FONT_ENCODING_UTF16,
            FONT_ENCODING_SJIS,
            FONT_ENCODING_CP1252,

            FONT_ENCODING_MAX
        };

        struct Glyph {
            void* pTexture;
            CharWidths widths;
            u8 height;
            GXTexFmt texFormat;
            u16 texWidth;
            u16 texHeight;
            u16 cellX;
            u16 cellY;
        };

        class Font {
        public:
            enum Type { TYPE_NULL, TYPE_ROM, TYPE_RESOURCE, TYPE_PAIR };

            Font() : mReadFunc(&CharStrmReader::ReadNextCharCP1252) {}

            virtual ~Font() {}
            virtual int GetWidth() const = 0;                                  // at 0xC
            virtual int GetHeight() const = 0;                                 // at 0x10
            virtual int GetAscent() const = 0;                                 // at 0x14
            virtual int GetDescent() const = 0;                                // at 0x18
            virtual int GetBaselinePos() const = 0;                            // at 0x1C
            virtual int GetCellHeight() const = 0;                             // at 0x20
            virtual int GetCellWidth() const = 0;                              // at 0x24
            virtual int GetMaxCharWidth() const = 0;                           // at 0x28
            virtual Type GetType() const = 0;                                  // at 0x2C
            virtual GXTexFmt GetTextureFormat() const = 0;                     // at 0x30
            virtual int GetLineFeed() const = 0;                               // at 0x34
            virtual const CharWidths GetDefaultCharWidths() const = 0;         // at 0x38
            virtual void SetDefaultCharWidths(const CharWidths& rWidths) = 0;  // at 0x3C
            virtual bool SetAlternateChar(u16 ch) = 0;                         // at 0x40
            virtual void SetLineFeed(int lf) = 0;                              // at 0x44
            virtual int GetCharWidth(u16 ch) const = 0;                        // at 0x48
            virtual const CharWidths GetCharWidths(u16 ch) const = 0;          // at 0x4C
            virtual void GetGlyph(Glyph* pGlyph, u16 ch) const = 0;            // at 0x50
            virtual bool HasGlyph(CharCode c) const = 0;                       // at 0x54
            virtual FontEncoding GetEncoding() const = 0;                      // at 0x58

            void InitReaderFunc(FontEncoding);

            const CharStrmReader GetCharStrmReader() const NO_INLINE {
                CharStrmReader reader(mReadFunc);
                return reader;
            }

            CharStrmReader::ReadNextCharFunc mReadFunc;
        };

    };  // namespace ut
};  // namespace nw4r
