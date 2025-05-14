#pragma once

#include "nw4r/lyt/resources.h"
#include "nw4r/math/types.h"
#include "nw4r/ut/Color.h"
#include "nw4r/ut/inlines.h"

namespace nw4r {
    namespace lyt {
        namespace detail {
            inline bool IsCITexelFormat(_GXTexFmt fmt) {
                return fmt == GX_TF_C4 || fmt == GX_TF_C8 || fmt == GX_TF_C14X2;
            }

            bool IsModulateVertexColor(ut::Color *, u8);
            const ut::Color MultipleAlpha(const ut::Color, u8);
            void SetVertexFormat(bool, u8);
            void DrawQuad(const math::VEC2 &, const Size &, u8, const math::VEC2 (*)[4], const ut::Color *);
            void DrawQuad(const math::VEC2 &, const Size &, u8, const math::VEC2(*)[4], const ut::Color *, u8);

            class TexCoordAry {
            public:
                TexCoordAry();

                void Free();
                void Reserve(u8);
                void SetSize(u8);
                void Copy(const void *, u8);

                u8 mCap;
                u8 mNum;
                math::VEC2 (*mData)[4];
            };
        };

        class ResourceAccessor;

        struct ResBlockSet {
            const res::TextureList* pTextureList;
            const res::FontList* pFontList;
            const res::MaterialList* pMaterialList;
            ResourceAccessor* pResAccessor;
        };

        namespace detail {
            bool EqualsResName(const char *, const char *);
            bool EqualsMaterialName(const char *, const char *);
            bool TestFileHeader(const res::BinaryFileHeader &, u32);

            inline bool TestFileVersion(const res::BinaryFileHeader &fileHeader) {
                const u32 major = ut::BitExtract(fileHeader.version, 8, 8);
                const u32 minor = ut::BitExtract(fileHeader.version, 0, 8);
                return (major == 0 && (minor >= 8 &&  minor <= 10));
            }

            inline s32 GetSignatureInt(const char sig[4]) {
                return *reinterpret_cast<const s32*>(sig);
            }

            
        };
    };
};
