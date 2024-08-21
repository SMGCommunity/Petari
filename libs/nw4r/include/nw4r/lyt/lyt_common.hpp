#pragma once

#include <revolution.h>
#include "nw4r/lyt/lyt_resources.hpp"
#include "nw4r/lyt/lyt_resourceAccessor.hpp"
#include "nw4r/math/types.hpp"
#include "nw4r/ut/Color.hpp"

namespace nw4r {
    namespace lyt {

        struct ResBlockSet {
            const res::TextureList* pTextureList;
            const res::FontList* pFontList;
            const res::MaterialList* PMaterialList;
            ResourceAccessor* pResAccessor;
        };

        namespace detail {
            bool EqualsResName(const char *, const char *);
            bool EqualsMaterialName(const char *, const char *);
            bool TestFileHeader(const nw4r::lyt::res::BinaryFileHeader &, u32);

            inline bool IsCITexelFormat(GXTexFmt fmt) {
                return (fmt == 8 || fmt == 0x9 || fmt == 0xA);
            }

            class TexCoordAry {
            public:
                TexCoordAry();

                void Free();
                void Reserve(u8);
                void SetSize(u8);
                void Copy(const void *, u8);

                u8 mCap;                    // 0x0
                u8 mNum;                    // 0x1
                math::VEC2 (*mpData)[4];    // 0x4
            };

            void SetVertexFormat(bool, u8);
            const ut::Color MultipleAlpha(const ut::Color, u8);
            void MultipleAlpha(ut::Color *, const ut::Color *, u8);
        };
    };
};