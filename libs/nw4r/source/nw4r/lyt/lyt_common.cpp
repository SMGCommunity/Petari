#include "nw4r/lyt/lyt_common.h"
#include "nw4r/lyt/lyt_layout.h"
#include "nw4r/lyt/types.h"
#include "nw4r/ut/inlines.h"
#include <cstring>

namespace nw4r {
    namespace lyt {
        namespace detail {
            bool EqualsResName(const char *pName1, const char *pName2) {
                return strncmp(pName1, pName2, 0x10) == 0;
            }

            bool EqualsMaterialName(const char *pName1, const char *pName2) {
                return strncmp(pName1, pName2, 0x14) == 0;
            }

            inline s32 GetSig(const char sig[4]) {
                return *reinterpret_cast<const s32*>(sig);
            }

            bool TestFileHeader(const res::BinaryFileHeader &rHeader, u32 signature) {
                return GetSig(rHeader.signature) == signature && rHeader.byteOrder == 0xFEFF;
            }

            TexCoordAry::TexCoordAry() {
                mCap = 0;
                mNum = 0;
                mpData = nullptr;
            }

            void TexCoordAry::Free() {
                if (mpData != nullptr) {
                    const u32 coordNum = mCap;
                    MEMFreeToAllocator(Layout::mspAllocator, &mpData[0][0]);
                    mpData = 0;
                    mCap = 0;
                    mNum = 0;
                }
            }

            /*
            void TexCoordAry::Reserve(u8 num) {
                if (mCap < num) {
                    Free();

                    const u32 coordNum = num;
                    math::VEC2* const pVecAry = Layout::NewArray<math::VEC2>(4 * coordNum);
                    mpData = reinterpret_cast<TexCoordQuad>(pVecAry);
                
                    if (mpData == nullptr) {
                        mCap = 0;
                    }
                }
            }
            */

            void TexCoordAry::SetSize(u8 num) {
                if (mpData != nullptr && num <= mCap) {
                    static const math::VEC2 texCoords[] = {
                        math::VEC2(0.0f, 0.0f),
                        math::VEC2(1.0f, 0.0f),
                        math::VEC2(0.0f, 1.0f),
                        math::VEC2(1.0f, 1.0f)
                    };

                    for (s32 j = mNum; j < num; j++) {
                        for (s32 i = 0; i < 4; i++) {
                            mpData[j][i] = texCoords[i];
                        }
                    }

                    mNum = num;
                }
            }

            void TexCoordAry::Copy(const void *pResTexCoord, u8 texCoordNum) {
                mNum = ut::Max(mNum, texCoordNum);
                const math::VEC2 (*src)[4] = reinterpret_cast<ConstTexCoordQuad>(pResTexCoord);

                for (s32 j = 0; j < texCoordNum; j++) {
                    for (s32 i = 0; i < 4; i++) {
                        mpData[j][i] = src[j][i];
                    }
                }
            }

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

            const ut::Color MultipleAlpha(const ut::Color col, u8 alpha) {
                ut::Color ret = col;

                if (alpha != 255) {
                    ret.a = u8(col.a * alpha / 255);
                }

                return ret;
            }
        };
    };
};