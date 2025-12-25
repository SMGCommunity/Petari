#pragma once

#include "nw4r/math/types.h"
#include "nw4r/ut/Color.h"
#include "nw4r/ut/Font.h"
#include <revolution/gx/GXEnum.h>

namespace nw4r {
    namespace ut {
        class Font;

        class CharWriter {
        public:
            enum GradationMode { GRADMODE_NONE, GRADMODE_H, GRADMODE_V, NUM_OF_GRADMODE };

            CharWriter();
            ~CharWriter();

            typedef math::VEC2 CharScale;
            typedef math::VEC3 CursorPos;

            static const int TEXTURE_COODINATE_FRACTION_BITS = 15;
            static const u32 TEXTURE_COODINATE_ONE = (1 << TEXTURE_COODINATE_FRACTION_BITS);

            struct ColorMapping {
                Color min;
                Color max;
            };

            struct TextColor {
                Color start;
                Color end;
                GradationMode gradationMode;
            };

            struct VertexColor {
                Color lu;
                Color ru;
                Color ld;
                Color rd;
            };

            class TextureFilter {
            public:
                bool operator!=(const TextureFilter& rhs) const { return (atSmall != rhs.atSmall) || (atLarge != rhs.atLarge); }

                GXTexFilter atSmall;
                GXTexFilter atLarge;
            };

            class LoadingTexture {
            public:
                GXTexMapID slot;
                const void* texture;
                TextureFilter filter;

                void Reset() {
                    slot = GX_TEXMAP_NULL;
                    texture = nullptr;
                }

                bool operator!=(const LoadingTexture& rhs) const { return (slot != rhs.slot) || (texture != rhs.texture) || (filter != rhs.filter); }
            };

            void SetCursor(f32 x, f32 y) {
                mCursorPos.x = x;
                mCursorPos.y = y;
            }

            void SetCursor(f32 x, f32 y, f32 z) {
                mCursorPos.x = x;
                mCursorPos.y = y;
                mCursorPos.z = z;
            }

            f32 GetCursorX() const { return mCursorPos.x; }
            void SetCursorX(f32 x) { mCursorPos.x = x; }
            f32 GetCursorY() const { return mCursorPos.y; }
            void SetCursorY(f32 y) { mCursorPos.y = y; }
            void SetFont(const Font& rFont) { mFont = &rFont; }
            const Font* GetFont() const { return mFont; }

            void SetScale(f32 hScale, f32 vScale) {
                mScale.x = hScale;
                mScale.y = vScale;
            }

            void SetScale(f32 hvScale) {
                mScale.x = hvScale;
                mScale.y = hvScale;
            }

            f32 GetScaleH() const { return mScale.x; }

            f32 GetScaleV() const { return mScale.y; }

            void SetGradationMode(GradationMode mode) {
                mTextColor.gradationMode = mode;
                UpdateVertexColor();
            }

            bool IsWidthFixed() const { return mIsWidthFixed; }

            f32 GetFixedWidth() const { return mFixedWidth; }

            void SetupGX();
            void SetupGXForI();
            void SetupGXDefault();
            void SetupGXForRGBA();
            void SetupGXWithColorMapping(Color, Color);

            void ResetTextureCache() { mLoadingTexture.Reset(); }

            void EnableLinearFilter(bool, bool);
            void SetColorMapping(Color, Color);
            void SetTextColor(Color);
            f32 GetFontWidth() const;
            f32 GetFontHeight() const;
            void SetFontSize(f32 width, f32 height);
            void LoadTexture(const Glyph&, GXTexMapID);
            f32 GetFontAscent() const;
            f32 Print(CharCode);
            void PrintGlyph(f32, f32, f32, const Glyph&);
            void UpdateVertexColor();
            void SetupVertexFormat();

            void ResetColorMapping() { SetColorMapping(0x00000000UL, 0xFFFFFFFFUL); }

            static LoadingTexture mLoadingTexture;

            ColorMapping mColorMapping;
            VertexColor mVertexColor;
            TextColor mTextColor;
            CharScale mScale;
            CursorPos mCursorPos;
            TextureFilter mFilter;
            u8 pad[2];
            u8 mAlpha;
            bool mIsWidthFixed;
            f32 mFixedWidth;
            const Font* mFont;
        };
    };  // namespace ut
};  // namespace nw4r
