#pragma once

#include "nw4r/db/assert.h"

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

            static const u32 DEFAULT_COLOR_MAPPING_MIN = 0;
            static const u32 DEFAULT_COLOR_MAPPING_MAX = 0xFFFFFFFF;

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
                bool operator!=(const TextureFilter& rhs) const {
                    return (atSmall != rhs.atSmall) || (atLarge != rhs.atLarge);
                }

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

                bool operator!=(const LoadingTexture& rhs) const {
                    return (slot != rhs.slot) || (texture != rhs.texture) || (filter != rhs.filter);
                }
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

            void SetScale(f32 hScale, f32 vScale) {
                mScale.x = hScale;
                mScale.y = vScale;
            }

            void SetScale(f32 hvScale) {
                mScale.x = hvScale;
                mScale.y = hvScale;
            }

            void SetGradationMode(GradationMode mode) {
                mTextColor.gradationMode = mode;
                UpdateVertexColor();
            }

            void SetupGX();
            void SetupGXForI();
            void SetupGXDefault();
            void SetupGXForRGBA();
            static void SetupGXWithColorMapping(Color, Color);

            void ResetTextureCache() {
                mLoadingTexture.Reset();
            }

            void EnableLinearFilter(bool, bool);

            void SetTextColor(Color start, Color end) {
                mTextColor.start = start;
                mTextColor.end = end;
                UpdateVertexColor();
            }

            f32 GetFontWidth() const;
            f32 GetFontHeight() const;
            void SetFontSize(f32 width, f32 height);
            void LoadTexture(const Glyph&, GXTexMapID);
            f32 GetFontAscent() const;
            f32 Print(CharCode);
            void PrintGlyph(f32, f32, f32, const Glyph&);
            void UpdateVertexColor();
            static void SetupVertexFormat();

            void SetFont(const Font& font) {
                NW4R_POINTER_ASSERT_AT(65, this);
                NW4R_REFERENCE_ASSERT_AT(66, font);
                mFont = &font;
            }

            const Font* GetFont() const {
                NW4R_POINTER_ASSERT_AT(71, this);
                return mFont;
            }

            void SetColorMapping(Color min, Color max) {
                NW4R_POINTER_ASSERT_AT(87, this);
                mColorMapping.min = min;
                mColorMapping.max = max;
            }

            void ResetColorMapping() {
                NW4R_POINTER_ASSERT_AT(105, this);
                SetColorMapping(DEFAULT_COLOR_MAPPING_MIN, DEFAULT_COLOR_MAPPING_MAX);
            }

            void SetAlpha(u8 alpha) {
                NW4R_POINTER_ASSERT_AT(112, this);
                mAlpha = alpha;
                UpdateVertexColor();
            }

            u8 GetAlpha() const {
                NW4R_POINTER_ASSERT_AT(118, this);
                return mAlpha;
            }

            void SetTextColor(Color color) {
                NW4R_POINTER_ASSERT_AT(135, this);
                mTextColor.start = color;
                UpdateVertexColor();
            }

            const Color GetTextColor() const {
                NW4R_POINTER_ASSERT_AT(152, this);
                return mTextColor.start;
            }

            f32 GetScaleH() const {
                NW4R_POINTER_ASSERT_AT(184, this);
                return mScale.x;
            }

            f32 GetScaleV() const {
                NW4R_POINTER_ASSERT_AT(189, this);
                return mScale.y;
            }

            bool IsWidthFixed() const {
                NW4R_POINTER_ASSERT_AT(221, this);
                return mIsWidthFixed;
            }

            f32 GetFixedWidth() const {
                NW4R_POINTER_ASSERT_AT(230, this);
                return mFixedWidth;
            }

            void SetCursorX(f32 x) {
                NW4R_POINTER_ASSERT_AT(286, this);
                mCursorPos.x = x;
            }

            void SetCursorY(f32 y) {
                NW4R_POINTER_ASSERT_AT(291, this);
                mCursorPos.y = y;
            }

            void MoveCursorX(f32 dx) {
                NW4R_POINTER_ASSERT_AT(301, this);
                mCursorPos.x += dx;
            }

            void MoveCursorY(f32 dy) {
                NW4R_POINTER_ASSERT_AT(306, this);
                mCursorPos.y += dy;
            }

            f32 GetCursorX() const {
                NW4R_POINTER_ASSERT_AT(316, this);
                return mCursorPos.x;
            }

            f32 GetCursorY() const {
                NW4R_POINTER_ASSERT_AT(321, this);
                return mCursorPos.y;
            }

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
