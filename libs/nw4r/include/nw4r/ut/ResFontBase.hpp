#pragma once

#include "nw4r/ut/Font.h"

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
                virtual void SetDefaultCharWidths(const CharWidths &);
                virtual bool SetAlternateChar(CharCode);
                virtual void SetLineFeed(int);
                virtual int GetCharWidth(CharCode) const;
                virtual const CharWidths GetCharWidths(CharCode) const;
                virtual void GetGlyph(Glyph *, CharCode) const;
                virtual bool HasGlyph(CharCode) const;
                virtual FontEncoding GetEncoding() const;

                void SetResourceBuffer(void *, FontInformation *);
                void* RemoveResourceBuffer();
                const CharWidths& GetCharWidthsFromIndex(u16) const;
                const CharWidths& GetCharWidthsFromIndex(const FontWidth *, u16) const;
                u16 FindGlyphIndex(CharCode c) const;
                u16 FindGlyphIndex(const FontCodeMap *, CharCode) const;
                void GetGlyphFromIndex(Glyph *, u16) const;
                u16 GetGlyphIndex(CharCode) const;

                inline bool IsManaging(const void *ptr) const {
                    return mResource == ptr;
                }

                void* mResource;
                FontInformation* mFontInfo;
                mutable CharCode mLastCharCode;
                mutable u16 mLastGlyphIndex;
            };
        };
    };
};