#pragma once

#include <revolution.h>
#include "nw4r/lyt/lyt_resources.h"
#include "nw4r/math/types.h"

namespace nw4r {
    namespace lyt {
        namespace detail {
            bool EqualsResName(const char *, const char *);
            bool EqualsMaterialName(const char *, const char *);
            bool TestFileHeader(const nw4r::lyt::res::BinaryFileHeader &, u32);


            class TexCoordAry {
            public:
                TexCoordAry();

                void Free();
                void Reserve(u8);
                void SetSize(u8);
                void Copy(const void *, u8);

                u8 mCap;                    // _0
                u8 mNum;                    // _1
                math::VEC2 (*mpData)[4];    // _4
            };

            void SetVertexFormat(bool, u8);
        };
    };
};