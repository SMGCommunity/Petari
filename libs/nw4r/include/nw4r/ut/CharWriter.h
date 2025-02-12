#pragma once

#include "nw4r/math/types.h"
#include "nw4r/ut/Color.h"
#include <revolution/gx/GXEnum.h>

namespace nw4r {
    namespace ut {
        class Font;

        class CharWriter {
        public:
            enum GradationMode {
                GRADMODE_NONE,
                GRADMODE_H,
                GRADMODE_V,
                NUM_OF_GRADMODE
            };

            CharWriter();
            ~CharWriter();

            typedef math::VEC2 CharScale;
            typedef math::VEC3 CursorPos;

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
                GXTexFilter atSmall;
                GXTexFilter atLarge;
            };

            class LoadingTexture {
            public:
                GXTexMapID slot;
                const void* texture;
                TextureFilter filter;
            };

            void SetCursor(f32 x, f32 y) {
                mCursorPos.x = x;
                mCursorPos.y = y;
            }

            f32 GetCursorX() const {
                return mCursorPos.x;
            }

            f32 GetCursorY() const {
                return mCursorPos.y;
            }

            f32 GetFontWidth() const;

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
    };
};
