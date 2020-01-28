#include "nw4r/ut/ResFontBase.h"

namespace nw4r
{
    namespace ut
    {
        namespace detail
        {
            ResFontBase::ResFontBase() : Font()
            {
                mRes = 0; 
                mInfo = 0;
            }

            void ResFontBase::SetResourceBuffer(void *pBuf, FontInformation *pInfo)
            {
                mRes = pBuf;
                mInfo = pInfo;
            }

            void* ResFontBase::RemoveResourceBuffer()
            {
                void* ret = mRes;
                mRes = 0;
                mInfo = 0;
                return ret;
            }

            s32 ResFontBase::GetWidth() const
            {
                return mInfo->mWidth;
            }

            s32 ResFontBase::GetHeight() const
            {
                return mInfo->mHeight;
            }

            s32 ResFontBase::GetAscent() const
            {
                return mInfo->mAscent;
            }

            s32 ResFontBase::GetDescent() const
            {
                return mInfo->mAscent - mInfo->mHeight;
            }

            s32 ResFontBase::GetBaselinePos() const
            {
                return static_cast<s32>(mInfo->mGlyph->mBaseLinePos);
            }

            s32 ResFontBase::GetCellHeight() const
            {
                return mInfo->mGlyph->mCellHeight;
            }

            s32 ResFontBase::GetCellWidth() const
            {
                return mInfo->mGlyph->mCellWidth;
            }

            s32 ResFontBase::GetMaxCharWidth() const
            {
                return mInfo->mGlyph->mMaxCharWidth;
            }

            Font::Type ResFontBase::GetType() const
            {
                return TYPE_RES;
            }

            GXTexFmt ResFontBase::GetTextureFormat() const
            {
                return static_cast<GXTexFmt>(mInfo->mGlyph->mSheetFormat);
            }

            s32 ResFontBase::GetLineFeed() const
            {
                return static_cast<s32>(mInfo->mLineFeed);
            }

            s32 ResFontBase::GetDefaultCharWidths() const
            {
                return (mInfo->mDefaultWidth.mCharWidth << 8) | mInfo->mDefaultWidth.data << 0x10;
            }

            void ResFontBase::SetDefaultCharWidths(const CharWidths &widths)
            {
                mInfo->mDefaultWidth.mLeft = widths.mLeft;
                mInfo->mDefaultWidth.mGlyphWidth = widths.mGlyphWidth;
                mInfo->mDefaultWidth.mCharWidth = widths.mCharWidth;
            }

            void ResFontBase::SetLineFeed(s32 lineFeed)
            {
                mInfo->mLineFeed = lineFeed;
            }

            s32 ResFontBase::GetCharWidth(u16) const
            {
                // todo
                return 0;
            }
        };
    };
};