#pragma once

#include "nw4r/lyt/resources.h"
#include "nw4r/ut/inlines.h"

namespace nw4r {
    namespace lyt {
        class ResourceAccessor;

        struct ResBlockSet {
            const res::TextureList* pTextureList;
            const res::FontList* pFontList;
            const res::MaterialList* pMaterialList;
            ResourceAccessor* pResAccessor;
        };

        namespace detail {
            bool TestFileHeader(const res::BinaryFileHeader &, u32);

            inline bool TestFileVersion(const res::BinaryFileHeader &fileHeader) {
                const u32 major = ut::BitExtract(fileHeader.version, 8, 8);
                const u32 minor = ut::BitExtract(fileHeader.version, 0, 8);
                return (major == 0 && (minor >= 8 &&  minor <= 10));
            }

            inline s32 GetSignatureInt(const char sig[4]) {
                return *reinterpret_cast<const s32*>(sig);
            }

            bool EqualsResName(const char *, const char *);
        };
    };
};
