#pragma once

#include <revolution.h>
#include <nw4r/math/types.h>
#include <nw4r/ut/Color.h>
#include <nw4r/ut/Font.h>

namespace nw4r {
    namespace ut {
        class CharWriter {
        public:
            enum GradationMode {
                None,
                Horizonal,
                Vertical
            };

            struct ColorMapping {
                Color min;
                Color max;
            };

            struct TextColor {
                Color start;
                Color end;
                GradationMode mode;
            };

            struct VertexColor {
                Color lu;
                Color ru;
                Color ld;
                Color rd;
            };

            class TextureFilter {
            public:
                GXTexFilter atSmall;
                GXTexFilter atLarge;

                bool operator !=(const TextureFilter &rhs) const {
                    return (atSmall != rhs.atSmall) || (atLarge != rhs.atLarge);
                }
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

                bool operator !=(const LoadingTexture &rhs) const {
                    return (slot != rhs.slot) || (texture != rhs.texture) || (filter != rhs.filter);
                }
            };


            inline void LoadTexture(const Glyph &, GXTexMapID);

            void SetScale(f32 h, f32 v) {
                mScale.x = h;
                mScale.y = v;
            }

            void ResetColorMapping() {
                SetColorMapping(0, 0xFFFFFFFF);
            }

            inline void SetGradiationMode(GradationMode mode) {
                mTextColor.mode = mode;
                UpdateVertexColor();
            }

            inline void SetTextColor(Color color) {
                mTextColor.start = color;
                UpdateVertexColor();
            }

            inline void SetCursor(f32 x, f32 y, f32 z) {
                mCursorPos.x = x;
                mCursorPos.y = y;
                mCursorPos.z = z;
            }

            CharWriter();
            ~CharWriter();

            void EnableLinearFilter(bool, bool);
            void SetColorMapping(Color, Color);
            void SetupGX();
            void SetFontSize(f32, f32);
            f32 GetFontWidth() const;
            f32 GetFontHeight() const;
            f32 GetFontAscent() const;
            void UpdateVertexColor();
            f32 Print(u16);
            void PrintGlyph(f32, f32, f32, const Glyph &);

            static void SetupGXWithColorMapping(Color, Color);
            static void SetupVertexFormat();

            static void SetupGXDefault();
            static void SetupGXForI();
            static void SetupGXForRGBA();

            static LoadingTexture mLoadingTexture;

            ColorMapping mColorMapping;
            VertexColor mVertexColor;
            TextColor mTextColor;
            math::VEC2 mScale;
            math::VEC3 mCursorPos;
            TextureFilter mFilter;
            u16 pad;
            u8 mAlpha;
            bool mIsWidthFixed;
            f32 mFixedWidth;
            const Font* mFont;
        };
    };
};