#ifndef RESFONTBASE_H
#define RESFONTBASE_H

#include "nw4r/ut/Font.h"

namespace nw4r
{
    namespace ut
    {
        namespace detail
        {
            class ResFontBase : public Font
            {
            public:

                typedef u16 GlyphIndex;

                ResFontBase();

                virtual ~ResFontBase();

                virtual s32 GetWidth() const;
                virtual s32 GetHeight() const;
                virtual s32 GetAscent() const;
                virtual s32 GetDescent() const;
                virtual s32 GetBaselinePos() const;
                virtual s32 GetCellHeight() const;
                virtual s32 GetCellWidth() const;
                virtual s32 GetMaxCharWidth() const;
                virtual Type GetType() const;
                virtual GXTexFmt GetTextureFormat() const;
                virtual s32 GetLineFeed() const;
                virtual s32 GetDefaultCharWidths() const;
                virtual void SetDefaultCharWidths(const CharWidths &);
                virtual bool SetAlternateChar(CharCode);
                virtual void SetLineFeed(s32);
                virtual s32 GetCharWidth(CharCode) const;
                virtual const CharWidths GetCharWidths(CharCode) const;
                virtual void GetGlyph(Glyph *, CharCode) const;
                virtual bool HasGlyph(CharCode) const;
                virtual FontEncoding GetEncoding() const;
            
                void SetResourceBuffer(void *, FontInformation *);
                void* RemoveResourceBuffer();

                void* mRes;
                FontInformation* mInfo;
                CharCode mLastCode;
                GlyphIndex mLastGlyphIdx;
            };
        };
    };
};

#endif // RESFONTBASE_H