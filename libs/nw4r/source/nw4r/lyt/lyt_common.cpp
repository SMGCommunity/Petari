#include "nw4r/lyt/lyt_common.h"

namespace nw4r {
    namespace lyt {
        namespace detail {
            void SetVertexFormat(bool a1, u8 texCoordCount) {
                GXClearVtxDesc();
                GXSetVtxDesc(GX_VA_POS, GX_DIRECT);

                if (a1) {
                    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
                }

                for (s32 i = 0; i < texCoordCount; i++) {
                    GXSetVtxDesc(GXAttr(GX_VA_TEX0 + i), GX_DIRECT);
                }

                GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_F32, 0);

                if (a1) {
                    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
                }

                for (s32 i = 0; i < texCoordCount; i++) {
                    GXSetVtxAttrFmt(GX_VTXFMT0, GXAttr(GX_VA_TEX0 + i), GX_TEX_ST, GX_F32, 0);
                }
            }
        };
    };
};