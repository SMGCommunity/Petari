#ifndef FONT_H
#define FONT_H

#include "types.h"
#include "nw4r/ut/CharStrmReader.h"
#include "revolution/gx/GXEnum.h"

namespace nw4r
{
    namespace ut
    {
        typedef u16 CharCode;

        struct CharWidths
        {
            union
            {
                u16 data;
                struct { s8 mLeft; u8 mGlyphWidth; };
            };

            s8 mCharWidth; // _2
        };

        struct Glyph
        {
            const void* mTex;
            CharWidths mWidths;
            u8 mHeight;
            GXTexFmt mTexFmt;
            u16 mTexWidth;
            u16 mTexHeight;
            u16 mCellX;
            u16 mCellY;
        };

        enum FontEncoding
        {
            FONT_ENCODING_UTF8,
            FONT_ENCODING_UTF16,
            FONT_ENCODING_SJIS,
            FONT_ENCODING_CP1252
        };

        struct FontTextureGlyph
        {
            u8 mCellWidth; // _0
            u8 mCellHeight; // _1
            s8 mBaseLinePos; // _2
            u8 mMaxCharWidth; // _3
            u32 mSheetSize; // _4
            u16 mSheetNum; // _8
            u16 mSheetFormat; // _A
            u16 mSheetRow; // _C
            u16 mSheetLine; // _E
            u16 mSheetWidth; // _10
            u16 mSheetHeight; // _12
            u8* mSheetSrc; // _14
        };

        struct FontWidth
        {
            u16 mIdxBegin; // _0
            u16 mIdxEnd; // _2
            FontWidth* mNextFont; // _4
            CharWidths mWidthTable[]; // _8
        };

        struct FontCodeMap
        {
            u16 mCharCodeBegin; // _0
            u16 mCharCodeEnd; // _2
            u16 mMapMethod; // _4
            u16 mPad; // _6
            FontCodeMap* mNextMap; // _8
            u16 mMapInfo[]; // _C
        };

        struct FontInformation
        {
            u8 mFontType; // _0
            s8 mLineFeed; // _1
            u16 mAltCharIdx; // _2
            CharWidths mDefaultWidth; // _3
            u8 mEncoding;
            FontTextureGlyph* mGlyph;
            FontWidth* mFontWidth;
            FontCodeMap* mMap;
            u8 mHeight;
            u8 mWidth;
            u8 mAscent;
        };

        class Font
        {
        public:

            enum Type
            {
                TYPE_NULL,
                TYPE_ROM,
                TYPE_RES, // nw4r::ut::ResFont
                TYPE_PAIR
            };

            Font() : mReaderFunc(&CharStrmReader::ReadNextCharCP1252)
            { }

            virtual ~Font();

            virtual s32 GetWidth() const = 0;
            virtual s32 GetHeight() const = 0;
            virtual s32 GetAscent() const = 0;
            virtual s32 GetDescent() const = 0;
            virtual s32 GetBaselinePos() const = 0;
            virtual s32 GetCellHeight() const = 0;
            virtual s32 GetCellWidth() const = 0;
            virtual s32 GetMaxCharWidth() const = 0;
            virtual Type GetType() const = 0;
            virtual GXTexFmt GetTextureFormat() const = 0;
            virtual s32 GetLineFeed() const = 0;
            virtual s32 GetDefaultCharWidths() const = 0;
            virtual void SetDefaultCharWidths(const CharWidths &) = 0;
            virtual bool SetAlternateChar(CharCode) = 0;
            virtual void SetLineFeed(s32) = 0;
            virtual s32 GetCharWidth(CharCode) const = 0;
            virtual const CharWidths GetCharWidths(CharCode) const = 0;
            virtual void GetGlyph(Glyph *, CharCode) const = 0;
            virtual bool HasGlyph(CharCode) const = 0;
            virtual FontEncoding GetEncoding() const = 0;

            void InitReaderFunc(FontEncoding);
            const CharStrmReader GetCharStrmReader() const;

            CharStrmReader::ReadNextCharFunc mReaderFunc;
        };
    };
};

#endif // FONT_H