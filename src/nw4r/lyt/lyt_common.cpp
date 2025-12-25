#include "nw4r/lyt/common.h"
#include "nw4r/lyt/layout.h"
#include "revolution/gx/GXEnum.h"
#include "revolution/gx/GXGeometry.h"
#include "revolution/gx/GXVert.h"
#include <cstdio>

namespace nw4r {
    namespace lyt {
        namespace detail {
            bool EqualsResName(const char* a1, const char* a2) {
                return strncmp(a1, a2, 0x10) == 0;
            }

            bool EqualsMaterialName(const char* a1, const char* a2) {
                return strncmp(a1, a2, 0x14) == 0;
            }

            bool TestFileHeader(const res::BinaryFileHeader& fileHeader, u32 sig) {
                return GetSignatureInt(fileHeader.signature) == sig && fileHeader.byteOrder == 0xFEFF;
            }

            TexCoordAry::TexCoordAry() {
                mCap = 0;
                mNum = 0;
                mData = nullptr;
            }

            void TexCoordAry::Free() {
                if (mData != nullptr) {
                    const u32 coordNum = mCap;
                    Layout::DeleteArray< math::VEC2 >(&mData[0][0], 4 * coordNum);
                    mData = nullptr;
                    mCap = 0;
                    mNum = 0;
                }
            }

            void TexCoordAry::Reserve(u8 num) {
                if (mCap < num) {
                    Free();
                    const u32 coordNum = num;
                    math::VEC2* const ary = Layout::NewArray< math::VEC2 >(4 * coordNum);
                    mData = reinterpret_cast< TexCoordQuad >(ary);
                    if (mData != nullptr) {
                        mCap = num;
                    }
                }
            }

            void TexCoordAry::SetSize(u8 num) {
                if (mData != nullptr && num <= mCap) {
                    static const math::VEC2 texCoords[] = {math::VEC2(0.0f, 0.0f), math::VEC2(1.0f, 0.0f), math::VEC2(0.0f, 1.0f),
                                                           math::VEC2(1.0f, 1.0f)};

                    for (int j = mNum; j < num; j++) {
                        for (int i = 0; i < 4; i++) {
                            mData[j][i] = texCoords[i];
                        }
                    }
                    mNum = num;
                }
            }

            void TexCoordAry::Copy(const void* pResTex, u8 texCoordNum) {
                mNum = ut::Max(mNum, texCoordNum);
                const math::VEC2(*src)[4] = reinterpret_cast< ConstTexCoordQuad >(pResTex);
                for (int j = 0; j < texCoordNum; j++) {
                    for (int i = 0; i < 4; i++) {
                        mData[j][i] = src[j][i];
                    }
                }
            }

            bool IsModulateVertexColor(ut::Color* color, u8 alpha) {
                if (alpha != 0xFF) {
                    return true;
                }

                if (color) {
                    if (color[0] != 0xFFFFFFFF || color[1] != 0xFFFFFFFF || color[2] != 0xFFFFFFFF || color[3] != 0xFFFFFFFF) {
                        return true;
                    }
                }

                return false;
            }

            const ut::Color MultipleAlpha(const ut::Color col, u8 alpha) {
                ut::Color ret = col;
                if (alpha != ut::Color::ALPHA_MAX) {
                    ret.a = u8(col.a * alpha / ut::Color::ALPHA_MAX);
                }

                return ret;
            }

            void MultipleAlpha(ut::Color* dst, const ut::Color* src, u8 a) {
                for (s32 i = 0; i < 4; i++) {
                    dst[i] = MultipleAlpha(src[i], a);
                }
            }

            void SetVertexFormat(bool a1, u8 num) {
                GXClearVtxDesc();
                GXSetVtxDesc(GX_VA_POS, GX_DIRECT);
                if (a1) {
                    GXSetVtxDesc(GX_VA_CLR0, GX_DIRECT);
                }

                for (int i = 0; i < num; i++) {
                    GXSetVtxDesc(GXAttr(GX_VA_TEX0 + i), GX_DIRECT);
                }

                GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_POS, GX_POS_XY, GX_F32, 0);
                if (a1) {
                    GXSetVtxAttrFmt(GX_VTXFMT0, GX_VA_CLR0, GX_CLR_RGBA, GX_RGBA8, 0);
                }

                for (int i = 0; i < num; ++i) {
                    GXSetVtxAttrFmt(GX_VTXFMT0, GXAttr(GX_VA_TEX0 + i), GX_TEX_ST, GX_F32, 0);
                }
            }

            void DrawQuad(const math::VEC2& basePt, const Size& size, u8 num, const math::VEC2 (*texCoords)[4], const ut::Color* vtxColors) {
                GXBegin(GX_QUADS, GX_VTXFMT0, 4);
                GXPosition2f32(basePt.x, basePt.y);

                if (vtxColors) {
                    GXColor1u32(vtxColors[0]);
                }

                for (int i = 0; i < num; i++) {
                    GXTexCoord2f32(texCoords[i][0].x, texCoords[i][0].y);
                }

                GXPosition2f32(basePt.x + size.width, basePt.y);

                if (vtxColors) {
                    GXColor1u32(vtxColors[1]);
                }

                for (int i = 0; i < num; i++) {
                    GXTexCoord2f32(texCoords[i][1].x, texCoords[i][1].y);
                }

                GXPosition2f32(basePt.x + size.width, basePt.y + size.height);

                if (vtxColors) {
                    GXColor1u32(vtxColors[3]);
                }

                for (int i = 0; i < num; i++) {
                    GXTexCoord2f32(texCoords[i][3].x, texCoords[i][3].y);
                }

                GXPosition2f32(basePt.x, basePt.y + size.height);

                if (vtxColors) {
                    GXColor1u32(vtxColors[2]);
                }

                for (int i = 0; i < num; i++) {
                    GXTexCoord2f32(texCoords[i][2].x, texCoords[i][2].y);
                }

                GXEnd();
            }

            void DrawQuad(const math::VEC2& basePt, const Size& size, u8 num, const math::VEC2 (*texCoords)[4], const ut::Color* vtxColors,
                          u8 alpha) {
                ut::Color colors[4];
                if (vtxColors) {
                    MultipleAlpha(colors, vtxColors, alpha);
                }
                DrawQuad(basePt, size, num, texCoords, vtxColors ? colors : 0);
            }
        };  // namespace detail
    };  // namespace lyt
};  // namespace nw4r
