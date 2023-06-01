#pragma once

#include <revolution.h>
#include "nw4r/ut/CharStrmReader.h"
#include "nw4r/ut/fontResourceFormat.h"

namespace nw4r {
    namespace ut {
        typedef u16 CharCode;
        
        struct Glyph {
            const void* pTexture;
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
            enum Type {
                Null,
                Rom,
                Res,
                Pair
            };

            Font() : mReaderFunc(&CharStrmReader::ReadNextCharCP1252) {

            }

            virtual ~Font();

            virtual int GetWidth() const = 0;
            virtual int GetHeight() const = 0;
            virtual int GetAscent() const = 0;
            virtual int GetDescent() const = 0;
            virtual int GetBaselinePos() const = 0;
            virtual int GetCellHeight() const = 0;
            virtual int GetCellWidth() const = 0;
            virtual int GetMaxCharWidth() const = 0;
            virtual Type GetType() const = 0;
            virtual GXTexFmt GetTextureFormat() const = 0;
            virtual int GetLineFeed() const = 0;
            virtual const CharWidths GetDefaultCharWidths() const = 0;
            virtual void SetDefaultCharWidths(const CharWidths &) = 0;
            virtual bool SetAlternateChar(CharCode) = 0;
            virtual void SetLineFeed(int) = 0;
            virtual int GetCharWidth(CharCode) const = 0;
            virtual const CharWidths GetCharWidths(CharCode) const = 0;
            virtual void GetGlyph(Glyph *, CharCode) const = 0;
            virtual bool HasGlyph(CharCode) const = 0;
            virtual FontEncoding GetEncoding() const = 0;

            void InitReaderFunc(FontEncoding);

            CharStrmReader::ReadNextCharFunc mReaderFunc;
        };
    };
};