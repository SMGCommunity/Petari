#pragma once

#include "nw4r/ut/Font.h"
#include "nw4r/ut/fontResourceFormat.h"

namespace nw4r {
    namespace ut {
        namespace detail {
            class ResFontBase : public Font {
            public:
                ResFontBase();

                virtual ~ResFontBase();
                virtual int GetWidth() const;
                virtual int GetHeight() const;
                virtual int GetAscent() const;
                virtual int GetDescent() const;
                virtual int GetBaselinePos() const;
                virtual int GetCellHeight() const;
                virtual int GetCellWidth() const;
                virtual int GetMaxCharWidth() const;
                virtual Type GetType() const;
                virtual GXTexFmt GetTextureFormat() const;
                virtual int GetLineFeed() const;
                virtual const CharWidths GetDefaultCharWidths() const;
                virtual void SetDefaultCharWidths(const CharWidths& widths);
                virtual bool SetAlternateChar(CharCode c);
                virtual void SetLineFeed(int linefeed);
                virtual int GetCharWidth(CharCode c) const;
                virtual const CharWidths GetCharWidths(CharCode c) const;
                virtual void GetGlyph(Glyph* glyph, CharCode c) const;
                virtual bool HasGlyph(CharCode c) const;
                virtual FontEncoding GetEncoding() const;

                void SetResourceBuffer(void*, FontInformation*);
                void* RemoveResourceBuffer();
                u16 FindGlyphIndex(CharCode c) const;
                u16 FindGlyphIndex(const FontCodeMap* pMap, CharCode c) const;
                u16 GetGlyphIndex(u16) const;
                const CharWidths& GetCharWidthsFromIndex(u16 index) const;
                const CharWidths& GetCharWidthsFromIndex(const FontWidth* pWidth, u16 index) const;
                void GetGlyphFromIndex(Glyph* pGlyph, u16 index) const;

                bool IsManaging(const void* pBuffer) const { return mResource == pBuffer; }

                void* mResource;
                FontInformation* mFontInfo;
                CharCode mLastCharCode;
                u16 mLastGlyphIndex;
            };
        };  // namespace detail

    };  // namespace ut
};  // namespace nw4r
