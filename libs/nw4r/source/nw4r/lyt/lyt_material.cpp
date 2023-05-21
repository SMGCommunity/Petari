#include "nw4r/lyt/lyt_common.h"
#include "nw4r/lyt/lyt_material.h"
#include "nw4r/lyt/lyt_texMap.h"
#include "nw4r/math/triangular.h"
#include "nw4r/math/arithmetic.h"
#include <cstring>
#include <new>

const GXColorS10 DefaultBlackColor = { 0, 0, 0, 0 };
const GXColorS10 DefaultWhiteColor = { 255, 255, 255, 255 };

namespace nw4r {
    namespace lyt {
        namespace {

            bool operator == (const GXColorS10 &a, const GXColorS10 &b) {
                return a.r == b.r && a.g == b.g && a.b == b.b && a.a == b.a;
            }

            bool operator != (const GXColorS10 &a, const GXColorS10 &b) {
                return ! operator == (a, b);
            }

            void CalcTextureMtx(math::MTX34* pMtx, const TexSRT& texSRT) {
                math::VEC2 center(00.5f, 00.5f);

                const f32 cosR = math::CosDeg(texSRT.rotate);
                const f32 sinR = math::SinDeg(texSRT.rotate);
                f32 a0, a1;

                a0 = cosR * texSRT.scale.x;
                a1 = -sinR * texSRT.scale.y;

                pMtx->m[0][0] = a0;
                pMtx->m[0][1] = a1;
                pMtx->m[0][2] = 0.0f;
                pMtx->m[0][3] = texSRT.translate.x + center.x + a0 * -center.x + a1 * -center.y;
            
                a0 = sinR * texSRT.scale.x;
                a1 = cosR * texSRT.scale.y;
                pMtx->m[1][0] = a0;
                pMtx->m[1][1] = a1;
                pMtx->m[1][2] = 0.0f;
                pMtx->m[1][3] = texSRT.translate.y + center.y + a0 * -center.x + a1 * -center.y;
                pMtx->m[2][0] = 0.0f;
                pMtx->m[2][1] = 0.0f;
                pMtx->m[2][2] = 0.0f;
                pMtx->m[2][3] = 0.0f;
            }

            void CalcIndTexMtx(f32 mtx[2][3], const TexSRT &texSRT) {
                const f32 cosR = math::CosDeg(texSRT.rotate);
                const f32 sinR = math::SinDeg(texSRT.rotate);

                mtx[0][0] = cosR * texSRT.scale.x;
                mtx[0][1] = -sinR * texSRT.scale.y;
                mtx[0][2] = texSRT.translate.x; 

                mtx[1][0] = sinR * texSRT.scale.x;
                mtx[1][1] = cosR * texSRT.scale.y;
                mtx[1][2] = texSRT.translate.y;
            }

            void SetIndTexMtx(GXIndTexMtxID id, const f32 mtx[2][3]) {
                f32 m00, m01, m02, m10, m11, m12, a00, a01, a02, a10, a11, a12;
                s8 scaleExp = 0;

                m00 = mtx[0][0];
                m01 = mtx[0][1];
                m02 = mtx[0][2];
                m10 = mtx[1][0];
                m11 = mtx[1][1];
                m12 = mtx[1][2];

                a00 = math::FAbs(m00);
                a01 = math::FAbs(m01);
                a02 = math::FAbs(m02);
                a10 = math::FAbs(m10);
                a11 = math::FAbs(m11);
                a12 = math::FAbs(m12);

                if (a00 >= 1.0f || a01 >= 1.0f || a02 >= 1.0f || a10 >= 1.0f || a11 >= 1.0f || a12 >= 1.0f) {
                    do {
                        if (scaleExp >= 46) {
                            break;
                        }

                        ++scaleExp;

                        m00 *= 0.5f;
                        m01 *= 0.5f;
                        m02 *= 0.5f;
                        m10 *= 0.5f;
                        m11 *= 0.5f;
                        m12 *= 0.5f;

                        a00 *= 0.5f;
                        a01 *= 0.5f;
                        a02 *= 0.5f;
                        a10 *= 0.5f;
                        a11 *= 0.5f;
                        a12 *= 0.5f;           
                    }
                    while(a00 >= 1.0f || a01 >= 1.0f || a02 >= 1.0f || a10 >= 1.0f || a11 >= 1.0f || a12 >= 1.0f);            
                }
                else if (a00 < 0.5f && a01 < 0.5f && a02 < 0.5f && a10 < 0.5f && a11 < 0.5f && a12 < 0.5f) {
                    do {
                        --scaleExp;

                        m00 *= 2.0f;
                        m01 *= 2.0f;
                        m02 *= 2.0f;
                        m10 *= 2.0f;
                        m11 *= 2.0f;
                        m12 *= 2.0f;

                        a00 *= 2.0f;
                        a01 *= 2.0f; a02 *= 2.0f;
                        a10 *= 2.0f;
                        a11 *= 2.0f;
                        a12 *= 2.0f;
                    }
                    while(a00 < 0.5f && a01 < 0.5f && a02 < 0.5f && a10 < 0.5f && a11 < 0.5f && a12 < 0.5f && scaleExp > -17);
                }

                f32 outMtx[2][3] = { m00, m01, m02, m10, m11, m12 };
                GXSetIndTexMtx(id, outMtx, scaleExp);
            }
        };

        inline u32 GetTexMtx(u32 texMtxIdx) {
            return GX_TEXMTX0 + (GX_TEXMTX1 - GX_TEXMTX0) * texMtxIdx;
        }

        inline u32 GetTexMtxIdx(u32 texMtx) {
            return (texMtx - GX_TEXMTX0) / (GX_TEXMTX1 - GX_TEXMTX0);
        }

        void SetColorComponentValue(ut::Color *pCol, u32 compIdx, s16 value) {
            const u8 val = u8(ut::Min(ut::Max(value, s16(0)), s16(255)));

            switch (compIdx) {
                case 0:
                    pCol->r = val;
                    break;
                case 1:
                    pCol->g = val;
                    break;
                case 2:
                    pCol->b = val;
                    break;
                case 3:
                    pCol->a = val;
                    break;
            }
        }

        u32 CalcOffsetTexSRTAry(const detail::BitGXNums &bitGXNums) {
            return sizeof(TexMap) * bitGXNums.texMap;
        }

        u32 CalcOffsetTexCoordGenAry(const detail::BitGXNums &bitGXNums) {
            return CalcOffsetTexSRTAry(bitGXNums) + sizeof(TexSRT) * bitGXNums.texSRT;
        }

        u32 CalcOffsetChanCtrlAry(const detail::BitGXNums &bitGXNums) {
            return CalcOffsetTexCoordGenAry(bitGXNums) + 4 * bitGXNums.texCoordGen;
        }

        u32 CalcOffsetMatColAry(const detail::BitGXNums& bitGXNums) {
            return CalcOffsetChanCtrlAry(bitGXNums) + 4 * bitGXNums.chanCtrl;
        }

        u32 CalcOffsetTevSwapAry(const detail::BitGXNums& bitGXNums) {
            return CalcOffsetMatColAry(bitGXNums) + 4 * bitGXNums.matCol;
        }

        u32 CalcOffsetGetAlphaCompare(const detail::BitGXNums& bitGXNums) {
            return CalcOffsetTevSwapAry(bitGXNums) + 4 * bitGXNums.tevSwap;
        }

        u32 CalcOffsetBlendMode(const detail::BitGXNums& bitGXNums) {
            return CalcOffsetGetAlphaCompare(bitGXNums) + 4 * bitGXNums.alpComp;
        }

        u32 CalcOffsetIndirectStageAry(const detail::BitGXNums& bitGXNums) {
            return CalcOffsetBlendMode(bitGXNums) + 4 * bitGXNums.blendMode;
        }

        u32 CalcOffsetIndTexSRTAry(const detail::BitGXNums& bitGXNums) {
            return CalcOffsetIndirectStageAry(bitGXNums) + 4 * bitGXNums.indStage;
        }

        u32 CalcOffsetTevStageAry(const detail::BitGXNums& bitGXNums) {
            return CalcOffsetIndTexSRTAry(bitGXNums) + sizeof(TexSRT) * bitGXNums.indSRT;
        }

        const TexMap* Material::GetTexMapAry() const {
            return detail::ConvertOffsToPtr<TexMap>(mpGXMem, 0);
        }

        TexMap* Material::GetTexMapAry() {
            return detail::ConvertOffsToPtr<TexMap>(mpGXMem, 0);
        }

        TexSRT* Material::GetTexSRTAry() {
            return detail::ConvertOffsToPtr<TexSRT>(mpGXMem, CalcOffsetTexSRTAry(mGXMemCap));
        }

        TexCoordGen* Material::GetTexCoordGenAry() {
            return detail::ConvertOffsToPtr<TexCoordGen>(mpGXMem, CalcOffsetTexCoordGenAry(mGXMemCap));
        }

        ChanCtrl* Material::GetChanCtrlAry() {
            return detail::ConvertOffsToPtr<ChanCtrl>(mpGXMem, CalcOffsetChanCtrlAry(mGXMemCap));
        }

        ut::Color* Material::GetMatColAry() {
            return detail::ConvertOffsToPtr<ut::Color>(mpGXMem, CalcOffsetMatColAry(mGXMemCap));
        }

        TevSwapMode* Material::GetTevSwapAry() {
            return detail::ConvertOffsToPtr<TevSwapMode>(mpGXMem, CalcOffsetTevSwapAry(mGXMemCap));
        }

        AlphaCompare* Material::GetAlphaComparePtr() {
            return detail::ConvertOffsToPtr<AlphaCompare>(mpGXMem, CalcOffsetGetAlphaCompare(mGXMemCap));
        }

        BlendMode* Material::GetBlendModePtr() {
            return detail::ConvertOffsToPtr<BlendMode>(mpGXMem, CalcOffsetBlendMode(mGXMemCap));
        }

        IndirectStage* Material::GetIndirectStageAry() {
            return detail::ConvertOffsToPtr<IndirectStage>(mpGXMem, CalcOffsetIndirectStageAry(mGXMemCap));
        }

        TexSRT* Material::GetIndTexSRTAry() {
            return detail::ConvertOffsToPtr<TexSRT>(mpGXMem, CalcOffsetIndTexSRTAry(mGXMemCap));
        }

        TevStage* Material::GetTevStageAry() {
            return detail::ConvertOffsToPtr<TevStage>(mpGXMem, CalcOffsetTevStageAry(mGXMemCap));
        }

        void Material::SetName(const char *name) {
            strncpy(mName, name, sizeof(mName) - 1);
            mName[sizeof(mName) - 1] = '\0';
        }

        void Material::SetTextureNum(u8 num) { 
            if (num > 0) {
                TexMap* const texMap = GetTexMapAry();

                for (u32 i = mGXMemNum.texMap; i < num; ++i) {
                    new (&texMap[i]) TexMap();
                }
            } 

            mGXMemNum.texMap = num;
        }

        void Material::SetTevStageNum(u8 num) {
            if (num > 0) {
                TevStage *const tevStage = GetTevStageAry();

                for (u32 i = mGXMemNum.tevStage; i < num; i++) {
                    new (&tevStage[i]) TevStage();
                }
            }

            mGXMemNum.tevStage = num;
        }

        void Material::SetIndStageNum(u8 num) {
            if (num > 0) {
                IndirectStage *const indStages = GetIndirectStageAry();

                for (u32 i = mGXMemNum.indStage; i < num; ++i) {
                    new (&indStages[i]) IndirectStage();
                }
            }

            mGXMemNum.indStage = num;
        }

        void Material::SetColorElement(u32 colorType, s16 value) {
            switch (colorType) {
                case 0:
                case 1:
                case 2:
                case 3:
                    if (mGXMemNum.matCol >= 1) {
                        ut::Color* matCols = GetMatColAry();
                        SetColorComponentValue(&matCols[0], (colorType % 4), value);
                    }
                    break;
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                case 10:
                case 11:
                case 12:
                case 13:
                case 14:
                case 15:
                    const u32 idx = (colorType - 4) / 4;

                    switch ((colorType - 4) % 4) {
                        case 0:
                            mTevCols[idx].r = value;
                            break;
                        case 1:
                            mTevCols[idx].g = value;
                            break;
                        case 2:
                            mTevCols[idx].b = value;
                            break;
                        case 3:
                            mTevCols[idx].a = value;
                            break;
                    }

                    break;
                
                case 16:
                case 17:
                case 18:
                case 19:
                case 20:
                case 21:
                case 22:
                case 23:
                case 24:
                case 25:
                case 26:
                case 27:
                case 28:
                case 29:
                case 30:
                case 31:
                    {
                        const u32 idx = (colorType - 16) / 4;
                        SetColorComponentValue(&mTevKCols[idx], (colorType - 16) % 4, value);
                        break;
                    }
            }
        }

        #ifdef NON_MATCHING
        bool Material::SetupGX(bool bModVtxCol, u8 alpha) {
            bool bUseVtxCol = true;
            bool bUseMatCol = false;

            GXSetNumChans(1);

            if (IsChanCtrlCap()) {
                const ChanCtrl* const chanCtrls = GetChanCtrlAry();
                GXSetChanCtrl(GX_COLOR0, GX_DISABLE, GX_SRC_REG, chanCtrls[0].GetColorSrc(), GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
                GXSetChanCtrl(GX_ALPHA0, GX_DISABLE, GX_SRC_REG, chanCtrls[0].GetAlphaSrc(), GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);

                bUseVtxCol = chanCtrls[0].GetColorSrc() == GX_SRC_VTX || chanCtrls[0].GetAlphaSrc() == GX_SRC_VTX;
                bUseMatCol = chanCtrls[0].GetColorSrc() == GX_SRC_REG || chanCtrls[0].GetAlphaSrc() == GX_SRC_REG;
            }
            else {
                GXSetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHT_NULL, GX_DF_NONE, GX_AF_NONE);
            }

            bool bUseRasStage = false;

            if (bUseVtxCol) {
                bUseRasStage = bUseRasStage || bModVtxCol;
            }

            if (bUseMatCol) {
                ut::Color matCol = 0xFFFFFFFF;
 
                if (IsMatColorCap()) {
                    matCol = GetMatColAry()[0]; 
                }

                matCol = detail::MultipleAlpha(matCol, alpha);
                GXSetChanMatColor(GX_COLOR0A0, matCol);

                bUseRasStage = bUseRasStage || matCol != 0xFFFFFFFF;
            }

            bool bSetTexMtx = false;
            bool bUseTexMtx[10];

            for (int i = 0; i < 10; ++i) {
                bUseTexMtx[i] = false;
            }

            GXSetNumTexGens(u8(mGXMemNum.texCoordGen));

            if (mGXMemNum.texCoordGen > 0) {
                const TexCoordGen* const texCoordGens = GetTexCoordGenAry();

                for (int i = 0; i < mGXMemNum.texCoordGen; ++i) {
                    const u32 texMtx = texCoordGens[i].GetTexMtx();

                    if (texCoordGens[i].GetTexGenType() == GX_TG_MTX2x4) {
                        if (texMtx != GX_IDENTITY) {
                            bUseTexMtx[GetTexMtxIdx(texMtx)] = true;
                            bSetTexMtx = true;
                        }
                    }

                    GXSetTexCoordGen2(GXTexCoordID(GX_TEXCOORD0 + (GX_TEXCOORD1 - GX_TEXCOORD0) * i), texCoordGens[i].GetTexGenType(), texCoordGens[i].GetTexGenSrc(), texMtx, GX_FALSE, 0x7D);
                }
            }

            if (bSetTexMtx) {
                const TexSRT *const texSRTs = GetTexSRTAry();

                for (u8 i = 0; i < mGXMemNum.texSRT; ++i) {
                    if (!bUseTexMtx[i]) {
                        continue;
                    }

                    math::MTX34 texMtx;
                    CalcTextureMtx(&texMtx, texSRTs[i]);
                    GXLoadTexMtxImm(texMtx, GetTexMtx(i), GX_MTX2x4);
                }
            }

            if (mGXMemNum.texMap > 0) {
                u32 tlutID = GX_TLUT0;
                u32 bigTlutID = GX_BIGTLUT0;
                const TexMap* const texMaps = GetTexMapAry();

                for (int i = 0; i < mGXMemNum.texMap; ++i) {
                    const TexMap& texMap = texMaps[i];

                    GXTexObj texObj;
                    texMap.Get(&texObj);

                    if (detail::IsCITexelFormat(texMap.GetTexelFormat())) {
                        u32 tlutName = 0;

                        if (texMap.GetTexelFormat() == GX_TF_C14X2) {
                            tlutName = bigTlutID++;
                        }
                        else {
                            tlutName = tlutID++;
                        }

                        GXInitTexObjTlut(&texObj, tlutName);
                        GXTlutObj tlutObj;
                        texMap.Get(&tlutObj);
                        GXLoadTlut(&tlutObj, tlutName);
                    }

                    GXLoadTexObj(&texObj, GXTexMapID(GX_TEXMAP0 + i));
                }
            }

            GXSetTevColorS10(GX_TEVREG0, mTevCols[0]);
            GXSetTevColorS10(GX_TEVREG1, mTevCols[1]);
            GXSetTevColorS10(GX_TEVREG2, mTevCols[2]);
            GXSetTevKColor(GX_KCOLOR0, mTevKCols[0]);
            GXSetTevKColor(GX_KCOLOR1, mTevKCols[1]);
            GXSetTevKColor(GX_KCOLOR2, mTevKCols[2]);
            GXSetTevKColor(GX_KCOLOR3, mTevKCols[3]);

            if (IsTevSwapCap()) {
                const TevSwapMode *const tevSwaps = GetTevSwapAry();

                for (int i = 0; i < GX_MAX_TEVSWAP; ++i) {
                    GXSetTevSwapModeTable(GXTevSwapSel(GX_TEV_SWAP0 + i),
                    tevSwaps[i].GetR(),
                    tevSwaps[i].GetG(),
                    tevSwaps[i].GetB(),
                    tevSwaps[i].GetA());
                }
            }
            else {
                GXSetTevSwapModeTable(GX_TEV_SWAP0, GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
                GXSetTevSwapModeTable(GX_TEV_SWAP1, GX_CH_RED, GX_CH_RED, GX_CH_RED, GX_CH_ALPHA);
                GXSetTevSwapModeTable(GX_TEV_SWAP2, GX_CH_GREEN, GX_CH_GREEN, GX_CH_GREEN, GX_CH_ALPHA);
                GXSetTevSwapModeTable(GX_TEV_SWAP3, GX_CH_BLUE, GX_CH_BLUE, GX_CH_BLUE, GX_CH_ALPHA);
            }

            bool bSetIndTexMtx = false;
            bool bUseIndTexMtx[3];

            for (int i = 0; i < 3; ++i) {
                bUseIndTexMtx[i] = false;
            }

            if (mGXMemNum.tevStage > 0) {
                GXSetNumTevStages(u8(mGXMemNum.tevStage));
                const TevStage* const tevStages = GetTevStageAry();

                for (int i = 0; i < mGXMemNum.tevStage; ++i) {
                    const GXTevStageID tevStage = GXTevStageID(GX_TEVSTAGE0 + i);

                    GXSetTevOrder(tevStage, tevStages[i].GetTexCoordGen(), tevStages[i].GetTexMap(), tevStages[i].GetColorChan());
                    GXSetTevSwapMode(tevStage, tevStages[i].GetRasSwapSel(), tevStages[i].GetTexSwapSel());
                    GXSetTevColorIn(tevStage, tevStages[i].GetColorInA(), tevStages[i].GetColorInB(), tevStages[i].GetColorInC(), tevStages[i].GetColorInD());
                    GXSetTevColorOp(tevStage, tevStages[i].GetColorOp(), tevStages[i].GetColorBias(), tevStages[i].GetColorScale(), tevStages[i].IsColorClamp(), tevStages[i].GetColorOutReg());
                    GXSetTevKColorSel(tevStage, tevStages[i].GetKColorSel());
                    GXSetTevAlphaIn(tevStage, tevStages[i].GetAlphaInA(), tevStages[i].GetAlphaInB(), tevStages[i].GetAlphaInC(), tevStages[i].GetAlphaInD());
                    GXSetTevAlphaOp(tevStage, tevStages[i].GetAlphaOp(), tevStages[i].GetAlphaBias(), tevStages[i].GetAlphaScale(), tevStages[i].IsAlphaClamp(), tevStages[i].GetAlphaOutReg());
                    GXSetTevKAlphaSel(tevStage, tevStages[i].GetKAlphaSel());

                    const GXIndTexMtxID indMtxSel = tevStages[i].GetIndMtxSel();
                    GXSetTevIndirect(tevStage, tevStages[i].GetIndStage(), tevStages[i].GetIndFormat(), tevStages[i].GetIndBiasSel(), indMtxSel, tevStages[i].GetIndWrapS(), tevStages[i].GetIndWrapT(), tevStages[i].IsIndAddPrev(), tevStages[i].IsIndUtcLod(), tevStages[i].GetIndAlphaSel());
                
                    if (GX_ITM_0 <= indMtxSel && indMtxSel <= GX_ITM_2) {
                        bUseIndTexMtx[indMtxSel - GX_ITM_0] = true;
                        bSetIndTexMtx = true;
                    }
                }

                bUseRasStage = true;
            }
            else {
                static GXTevKColorSel kColSels[] = {
                    GX_TEV_KCSEL_K3_A, GX_TEV_KCSEL_K3_B, GX_TEV_KCSEL_K3_G, GX_TEV_KCSEL_K3_R,
                    GX_TEV_KCSEL_K2_A, GX_TEV_KCSEL_K2_B, GX_TEV_KCSEL_K2_G, GX_TEV_KCSEL_K2_R,
                };
                static GXTevKAlphaSel kAlpSels[] = {
                    GX_TEV_KASEL_K3_A, GX_TEV_KASEL_K3_B, GX_TEV_KASEL_K3_G, GX_TEV_KASEL_K3_R,
                    GX_TEV_KASEL_K2_A, GX_TEV_KASEL_K2_B, GX_TEV_KASEL_K2_G, GX_TEV_KASEL_K2_R,
                };

                u8 tevStageID = GX_TEVSTAGE0;

                if (mGXMemNum.texMap == 0) {
                    const GXTevStageID tevStage = GXTevStageID(tevStageID);
                    GXSetTevOrder(tevStage, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
                    GXSetTevColorIn(tevStage, GX_CC_ZERO, GX_CC_C1, GX_CC_RASC, GX_CC_ZERO);
                    GXSetTevAlphaIn(tevStage, GX_CA_ZERO, GX_CA_A1, GX_CA_RASA, GX_CA_ZERO);
                    ++tevStageID;
                    bUseRasStage = true;
                }
                else {
                    if (mGXMemNum .texMap == 1) {
                        const GXTevStageID tevStage = GXTevStageID(tevStageID);
                        GXSetTevOrder(tevStage, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
                        GXSetTevColorIn(tevStage, GX_CC_C0, GX_CC_C1, GX_CC_TEXC, GX_CC_ZERO);
                        GXSetTevAlphaIn(tevStage, GX_CA_A0, GX_CA_A1, GX_CA_TEXA, GX_CA_ZERO);
                        ++tevStageID;
                    }
                    else {
                        if (mGXMemNum.texMap == 2) {
                            GXTevStageID tevStage = GXTevStageID(tevStageID);
                            GXSetTevOrder(tevStage, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR_NULL);
                            GXSetTevColorIn(tevStage, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_TEXC);
                            GXSetTevAlphaIn(tevStage, GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_TEXA);
                            ++tevStageID;

                            tevStage = GXTevStageID(tevStageID);
                            GXSetTevOrder(tevStage, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR_NULL);
                            GXSetTevColorIn(tevStage, GX_CC_TEXC, GX_CC_CPREV, GX_CC_KONST, GX_CC_ZERO);
                            GXSetTevAlphaIn(tevStage, GX_CA_TEXA, GX_CA_APREV, GX_CA_KONST, GX_CA_ZERO);
                            GXSetTevKColorSel(tevStage, kColSels[0]);
                            GXSetTevKAlphaSel(tevStage, kAlpSels[0]);
                            ++tevStageID;
                        }
                        else {
                            for (int i = 0; i < mGXMemNum.texMap; i++) {
                                const GXTevStageID tevStage = GXTevStageID(tevStageID);
                                GXSetTevOrder(tevStage, GXTexCoordID(GX_TEXCOORD0 + i), GXTexMapID(GX_TEXMAP0 + i), GX_COLOR_NULL);
                                GXTevColorArg colDIn = i == 0 ? GX_CC_ZERO: GX_CC_CPREV;
                                GXTevAlphaArg alpDIn = i == 0 ? GX_CA_ZERO: GX_CA_APREV;
                                GXSetTevColorIn(tevStage, GX_CC_ZERO, GX_CC_TEXC, GX_CC_KONST, colDIn);
                                GXSetTevAlphaIn(tevStage, GX_CA_ZERO, GX_CA_TEXA, GX_CA_KONST, alpDIn);
                                GXSetTevKColorSel(tevStage, kColSels[i]);
                                GXSetTevKAlphaSel(tevStage, kAlpSels[i]);
                                ++tevStageID;
                            }
                        }

                        if (mTevCols[0] != DefaultBlackColor || mTevCols[1] != DefaultWhiteColor) {
                            const GXTevStageID tevStage = GXTevStageID(tevStageID);
                            GXSetTevOrder(tevStage, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR_NULL);
                            GXSetTevColorIn(tevStage, GX_CC_C0, GX_CC_C1, GX_CC_CPREV, GX_CC_ZERO);
                            GXSetTevAlphaIn(tevStage, GX_CA_A0, GX_CA_A1, GX_CA_APREV, GX_CA_ZERO);
                            ++tevStageID;
                        }
                    }

                    if (bUseRasStage) {
                        const GXTevStageID tevStage = GXTevStageID(tevStageID);
                        GXSetTevOrder(tevStage, GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0);
                        GXSetTevColorIn(tevStage, GX_CC_ZERO, GX_CC_CPREV, GX_CC_RASC, GX_CC_ZERO);
                        GXSetTevAlphaIn(tevStage, GX_CA_ZERO, GX_CA_APREV, GX_CA_RASA, GX_CA_ZERO);
                        ++tevStageID;
                    }
                }

                const u8 tevStageNum = tevStageID;
                for (u8 id = GX_TEVSTAGE0; id < tevStageNum; ++id) {
                    const GXTevStageID tevStage = GXTevStageID(id);
                    GXSetTevColorOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                    GXSetTevAlphaOp(tevStage, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
                    GXSetTevDirect(tevStage);
                    GXSetTevSwapMode(tevStage, GX_TEV_SWAP0, GX_TEV_SWAP0);
                }

                GXSetNumTevStages(tevStageNum);
            }

            if (bSetIndTexMtx) {
                const TexSRT* const indTexSRTs = GetIndTexSRTAry();

                for (int i = 0; i < mGXMemNum.indSRT; ++i) {
                    if (!bUseIndTexMtx[i]) {
                        continue;
                    }

                    f32 mtx[2][3];
                    CalcIndTexMtx(mtx, indTexSRTs[i]);
                    SetIndTexMtx(GXIndTexMtxID(GX_ITM_0 + i), mtx);
                }
            }

            GXSetNumIndStages(u8(mGXMemNum.indStage));

            if (mGXMemNum.indSRT > 0) {
                const IndirectStage* const indirectStages = GetIndirectStageAry();

                for (int i = 0; i < mGXMemNum.indStage; ++i) {
                    const GXIndTexStageID indStage = GXIndTexStageID(GX_INDTEXSTAGE0 + i);
                    GXSetIndTexOrder(indStage, indirectStages[i].GetTexCoordGen(), indirectStages[i].GetTexMap());
                    GXSetIndTexCoordScale(indStage, indirectStages[i].GetScaleS(), indirectStages[i].GetScaleT());
                }
            }

            if (IsAlphaCompareCap()) {
                const AlphaCompare* const alp = GetAlphaComparePtr();
                GXSetAlphaCompare(alp->GetComp0(), alp->GetRef0(), alp->GetOp(), alp->GetComp1(), alp->GetRef1());
            }
            else {
                GXSetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
            }

            if (IsBlendModeCap()) {
                const BlendMode* const blend = GetBlendModePtr();
                GXSetBlendMode(blend->GetType(), blend->GetSrcFactor(), blend->GetDstFactor(), blend->GetOp());
            }
            else {
                GXSetBlendMode(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
            }

            return bUseRasStage && bUseVtxCol;
        }
        #endif
    };
};