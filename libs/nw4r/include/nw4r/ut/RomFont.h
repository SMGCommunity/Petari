#ifndef NW4R_UT_ROMFONT_H_
#define NW4R_UT_ROMFONT_H_

#include <revolution/types.h>
#include "nw4r/ut/Font.h"

namespace nw4r {
    namespace ut {

        class RomFont : public Font {
        public:
            RomFont();
            virtual ~RomFont();

            bool Load(void* buffer);
            void* Unload();
            static u32 GetRequireBufferSize();

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

            virtual void GetGlyph(Glyph* glyphPtr, CharCode c) const;

            virtual bool HasGlyph(CharCode c) const;

            virtual FontEncoding GetEncoding() const;

        private:
            const static int CHAR_PTR_BUFFER_SIZE = 3;

            static u16 mFontEncode;
            OSFontHeader* mFontHeader;
            CharWidths mDefaultWidths;
            CharCode mAlternateChar;

            void MakeCharPtr(char* buffer, CharCode c) const;

            CharCode HandleUndefinedChar(CharCode c) const;
        };

    }
}
#endif
