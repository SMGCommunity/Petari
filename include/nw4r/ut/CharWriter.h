#pragma once

#include <revolution.h>
#include "nw4r/math/types.h"
#include "nw4r/ut/Color.h"

namespace nw4r {
    namespace ut {

        class Font;

        class CharWriter {
        public:
            enum GradationMode
            {
                GRADMODE_NONE,                          // No gradation
                GRADMODE_H,                             // Horizontal gradation
                GRADMODE_V,                             // Vertical gradation
                NUM_OF_GRADMODE
            };

            CharWriter();
            ~CharWriter();

            struct ColorMapping
            {
                nw4r::ut::Color min;
                nw4r::ut::Color max;
            };

            struct TextColor
            {
                nw4r::ut::Color start;
                nw4r::ut::Color end;
                GradationMode gradationMode;
            };

            struct VertexColor
            {
                nw4r::ut::Color               lu;
                nw4r::ut::Color               ru;
                nw4r::ut::Color               ld;
                nw4r::ut::Color               rd;
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

            ColorMapping mColorMapping;
            VertexColor mVertexColor;
            TextColor mTextColor;
            math::VEC2 mScale;
            math::VEC3 mCursorPos;
            TextureFilter mFilter;
            u8 pad[2];
            u8 mAlpha;
            bool mIsWidthFixed;
            f32 mFixedWidth;
            const Font* mFont;
        };
    };
};