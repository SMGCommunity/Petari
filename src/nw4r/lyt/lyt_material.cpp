#include "nw4r/lyt/animation.h"
#include "nw4r/lyt/layout.h"
#include "nw4r/lyt/material.h"
#include "nw4r/lyt/texMap.h"
#include "nw4r/lyt/types.h"
#include "nw4r/math/triangular.h"
#include "nw4r/math/types.h"
#include "nw4r/ut/inlines.h"
#include <cstdio>

static const GXColorS10 DefaultBlackColor = {0, 0, 0, 0};
static const GXColorS10 DefaultWhiteColor = {255, 255, 255, 255};

namespace {
    bool operator==(const GXColorS10& rLhs, const GXColorS10& rRhs) {
        return rLhs.r == rRhs.r && rLhs.g == rRhs.g && rLhs.b == rRhs.b && rLhs.a == rRhs.a;
    }
    bool operator!=(const GXColorS10& rLhs, const GXColorS10& rRhs) {
        return !(rLhs == rRhs);
    }

    void CalcTextureMtx(nw4r::math::MTX34* pMtx, const nw4r::lyt::TexSRT& rSrt) {
        nw4r::math::VEC2 center(0.5f, 0.5f);

        f32 cr = nw4r::math::CosDeg(rSrt.rotate);
        f32 sr = nw4r::math::SinDeg(rSrt.rotate);

        f32 a0, a1;

        a0 = cr * rSrt.scale.x;
        a1 = -sr * rSrt.scale.y;

        pMtx->_00 = a0;
        pMtx->_01 = a1;
        pMtx->_02 = 0.0f;
        pMtx->_03 = rSrt.translate.x + center.x + a0 * -center.x + a1 * -center.y;

        a0 = sr * rSrt.scale.x;
        a1 = cr * rSrt.scale.y;

        pMtx->_10 = a0;
        pMtx->_11 = a1;
        pMtx->_12 = 0.0f;
        pMtx->_13 = rSrt.translate.y + center.y + a0 * -center.x + a1 * -center.y;

        pMtx->_20 = 0.0f;
        pMtx->_21 = 0.0f;
        pMtx->_22 = 1.0f;
        pMtx->_23 = 0.0f;
    }

    void CalcIndTexMtx(f32 mtx[2][3], const nw4r::lyt::TexSRT& rSrt) {
        f32 cr = nw4r::math::CosDeg(rSrt.rotate);
        f32 sr = nw4r::math::SinDeg(rSrt.rotate);

        mtx[0][0] = cr * rSrt.scale.x;
        mtx[0][1] = -sr * rSrt.scale.y;
        mtx[0][2] = rSrt.translate.x;

        mtx[1][0] = sr * rSrt.scale.x;
        mtx[1][1] = cr * rSrt.scale.y;
        mtx[1][2] = rSrt.translate.y;
    }

    inline u32 GetTexMtxIdx(u32 mtx) {
        return (mtx - GX_TEXMTX0) / 3;
    }
    inline u32 GetTexMtx(u32 idx) {
        return GX_TEXMTX0 + idx * 3;
    }

    void SetColorComponentValue(nw4r::ut::Color* pColor, u32 idx, s16 value) {
        u8 bvalue = nw4r::ut::Min< s16 >(nw4r::ut::Max< s16 >(value, 0), 255);

        switch (idx % 4) {
        case 0: {
            pColor->r = bvalue;
            break;
        }

        case 1: {
            pColor->g = bvalue;
            break;
        }

        case 2: {
            pColor->b = bvalue;
            break;
        }

        case 3: {
            pColor->a = bvalue;
            break;
        }

        default: {
            break;
        }
        }
    }

    u32 CalcOffsetTexSRTAry(const nw4r::lyt::detail::BitGXNums& rNums) {
        return rNums.texMap * ROUND_UP(sizeof(nw4r::lyt::TexMap), 4);
    }

    u32 CalcOffsetTexCoordGenAry(const nw4r::lyt::detail::BitGXNums& rNums) {
        return rNums.texSRT * ROUND_UP(sizeof(nw4r::lyt::TexSRT), 4) + CalcOffsetTexSRTAry(rNums);
    }

    u32 CalcOffsetChanCtrlAry(const nw4r::lyt::detail::BitGXNums& rNums) {
        return rNums.texCoordGen * ROUND_UP(sizeof(nw4r::lyt::TexCoordGen), 4) + CalcOffsetTexCoordGenAry(rNums);
    }

    u32 CalcOffsetMatColAry(const nw4r::lyt::detail::BitGXNums& rNums) {
        return rNums.chanCtrl * ROUND_UP(sizeof(nw4r::lyt::ChanCtrl), 4) + CalcOffsetChanCtrlAry(rNums);
    }

    u32 CalcOffsetTevSwapAry(const nw4r::lyt::detail::BitGXNums& rNums) {
        return rNums.matCol * ROUND_UP(sizeof(nw4r::ut::Color), 4) + CalcOffsetMatColAry(rNums);
    }

    u32 CalcOffsetGetAlphaCompare(const nw4r::lyt::detail::BitGXNums& rNums) {
        return rNums.tevSwap * ROUND_UP(sizeof(nw4r::lyt::TevSwapMode), 4) + CalcOffsetTevSwapAry(rNums);
    }

    u32 CalcOffsetBlendMode(const nw4r::lyt::detail::BitGXNums& rNums) {
        return rNums.alpComp * ROUND_UP(sizeof(nw4r::lyt::AlphaCompare), 4) + CalcOffsetGetAlphaCompare(rNums);
    }

    u32 CalcOffsetIndirectStageAry(const nw4r::lyt::detail::BitGXNums& rNums) {
        return rNums.blendMode * ROUND_UP(sizeof(nw4r::lyt::BlendMode), 4) + CalcOffsetBlendMode(rNums);
    }

    u32 CalcOffsetIndTexSRTAry(const nw4r::lyt::detail::BitGXNums& rNums) {
        return rNums.indStage * ROUND_UP(sizeof(nw4r::lyt::IndirectStage), 4) + CalcOffsetIndirectStageAry(rNums);
    }

    u32 CalcOffsetTevStageAry(const nw4r::lyt::detail::BitGXNums& rNums) {
        return rNums.indSRT * ROUND_UP(sizeof(nw4r::lyt::TexSRT), 4) + CalcOffsetIndTexSRTAry(rNums);
    }

};  // namespace

namespace nw4r {
    namespace lyt {

        Material::~Material() {
            UnbindAllAnimation();

            if (mpGXMem != NULL) {
                Layout::FreeMemory(mpGXMem);
                mpGXMem = NULL;
            }
        }

        const TexMap* Material::GetTexMapAry() const {
            return detail::ConvertOffsToPtr< TexMap >(mpGXMem, 0);
        }

        TexMap* Material::GetTexMapAry() {
            return detail::ConvertOffsToPtr< TexMap >(mpGXMem, 0);
        }

        TexSRT* Material::GetTexSRTAry() {
            return detail::ConvertOffsToPtr< TexSRT >(mpGXMem, CalcOffsetTexSRTAry(mGXMemCap));
        }

        TexCoordGen* Material::GetTexCoordGenAry() {
            return detail::ConvertOffsToPtr< TexCoordGen >(mpGXMem, CalcOffsetTexCoordGenAry(mGXMemCap));
        }

        ChanCtrl* Material::GetChanCtrlAry() {
            return detail::ConvertOffsToPtr< ChanCtrl >(mpGXMem, CalcOffsetChanCtrlAry(mGXMemCap));
        }

        ut::Color* Material::GetMatColAry() {
            return detail::ConvertOffsToPtr< ut::Color >(mpGXMem, CalcOffsetMatColAry(mGXMemCap));
        }

        TevSwapMode* Material::GetTevSwapAry() {
            return detail::ConvertOffsToPtr< TevSwapMode >(mpGXMem, CalcOffsetTevSwapAry(mGXMemCap));
        }

        AlphaCompare* Material::GetAlphaComparePtr() {
            return detail::ConvertOffsToPtr< AlphaCompare >(mpGXMem, CalcOffsetGetAlphaCompare(mGXMemCap));
        }

        BlendMode* Material::GetBlendModePtr() {
            return detail::ConvertOffsToPtr< BlendMode >(mpGXMem, CalcOffsetBlendMode(mGXMemCap));
        }

        IndirectStage* Material::GetIndirectStageAry() {
            return detail::ConvertOffsToPtr< IndirectStage >(mpGXMem, CalcOffsetIndirectStageAry(mGXMemCap));
        }

        TexSRT* Material::GetIndTexSRTAry() {
            return detail::ConvertOffsToPtr< TexSRT >(mpGXMem, CalcOffsetIndTexSRTAry(mGXMemCap));
        }

        TevStage* Material::GetTevStageAry() {
            return detail::ConvertOffsToPtr< TevStage >(mpGXMem, CalcOffsetTevStageAry(mGXMemCap));
        }

        void Material::SetName(const char* pName) {
            strncpy(mName, pName, sizeof(mName) - 1);
            mName[sizeof(mName) - 1] = '\0';
        }

        void Material::SetTextureNum(u8 num) {
            if (num > 0) {
                TexMap* const pTexMap = GetTexMapAry();

                for (u32 i = mGXMemNum.texMap; i < num; i++) {
                    new (&pTexMap[i]) TexMap();
                }
            }

            mGXMemNum.texMap = num;
        }

        void Material::SetTexCoordGenNum(u8 num) {
            if (num > 0) {
                TexCoordGen* const pTexCoordGen = GetTexCoordGenAry();

                for (u32 i = mGXMemNum.texCoordGen; i < num; i++) {
                    new (&pTexCoordGen[i]) TexCoordGen();
                }
            }

            mGXMemNum.texCoordGen = num;
        }

        void Material::SetTevStageNum(u8 num) {
            if (num > 0) {
                TevStage* const pTevStage = GetTevStageAry();

                for (u32 i = mGXMemNum.tevStage; i < num; i++) {
                    new (&pTevStage[i]) TevStage();
                }
            }

            mGXMemNum.tevStage = num;
        }

        void Material::SetIndStageNum(u8 num) {
            if (num > 0) {
                IndirectStage* const pIndStage = GetIndirectStageAry();

                for (u32 i = mGXMemNum.indStage; i < num; i++) {
                    new (&pIndStage[i]) IndirectStage();
                }
            }

            mGXMemNum.indStage = num;
        }

        void Material::SetColorElement(u32 idx, s16 value) {
            switch (idx) {
            case detail::ANIMTARGET_MATCOLOR_MATCOLOR0_R:
            case detail::ANIMTARGET_MATCOLOR_MATCOLOR0_G:
            case detail::ANIMTARGET_MATCOLOR_MATCOLOR0_B:
            case detail::ANIMTARGET_MATCOLOR_MATCOLOR0_A:
                if (mGXMemNum.matCol >= 1) {
                    ut::Color* matCols = GetMatColAry();
                    SetColorComponentValue(&matCols[0], (idx - detail::ANIMTARGET_MATCOLOR_MATCOLOR0_R) % 4, value);
                }
                break;

            case detail::ANIMTARGET_MATCOLOR_TEVCOLOR0_R:
            case detail::ANIMTARGET_MATCOLOR_TEVCOLOR0_G:
            case detail::ANIMTARGET_MATCOLOR_TEVCOLOR0_B:
            case detail::ANIMTARGET_MATCOLOR_TEVCOLOR0_A:
            case detail::ANIMTARGET_MATCOLOR_TEVCOLOR1_R:
            case detail::ANIMTARGET_MATCOLOR_TEVCOLOR1_G:
            case detail::ANIMTARGET_MATCOLOR_TEVCOLOR1_B:
            case detail::ANIMTARGET_MATCOLOR_TEVCOLOR1_A:
            case detail::ANIMTARGET_MATCOLOR_TEVCOLOR2_R:
            case detail::ANIMTARGET_MATCOLOR_TEVCOLOR2_G:
            case detail::ANIMTARGET_MATCOLOR_TEVCOLOR2_B:
            case detail::ANIMTARGET_MATCOLOR_TEVCOLOR2_A: {
                const u32 regIdx = (idx - detail::ANIMTARGET_MATCOLOR_TEVCOLOR0_R) / 4;
                switch ((idx - detail::ANIMTARGET_MATCOLOR_TEVCOLOR0_R) % 4) {
                case 0:
                    mTevCols[regIdx].r = value;
                    break;
                case 1:
                    mTevCols[regIdx].g = value;
                    break;
                case 2:
                    mTevCols[regIdx].b = value;
                    break;
                case 3:
                    mTevCols[regIdx].a = value;
                    break;
                }
            } break;

            case detail::ANIMTARGET_MATCOLOR_TEVKONST0_R:
            case detail::ANIMTARGET_MATCOLOR_TEVKONST0_G:
            case detail::ANIMTARGET_MATCOLOR_TEVKONST0_B:
            case detail::ANIMTARGET_MATCOLOR_TEVKONST0_A:
            case detail::ANIMTARGET_MATCOLOR_TEVKONST1_R:
            case detail::ANIMTARGET_MATCOLOR_TEVKONST1_G:
            case detail::ANIMTARGET_MATCOLOR_TEVKONST1_B:
            case detail::ANIMTARGET_MATCOLOR_TEVKONST1_A:
            case detail::ANIMTARGET_MATCOLOR_TEVKONST2_R:
            case detail::ANIMTARGET_MATCOLOR_TEVKONST2_G:
            case detail::ANIMTARGET_MATCOLOR_TEVKONST2_B:
            case detail::ANIMTARGET_MATCOLOR_TEVKONST2_A:
            case detail::ANIMTARGET_MATCOLOR_TEVKONST3_R:
            case detail::ANIMTARGET_MATCOLOR_TEVKONST3_G:
            case detail::ANIMTARGET_MATCOLOR_TEVKONST3_B:
            case detail::ANIMTARGET_MATCOLOR_TEVKONST3_A: {
                const u32 regIdx = (idx - detail::ANIMTARGET_MATCOLOR_TEVKONST0_R) / 4;
                SetColorComponentValue(&mTevKCols[regIdx], (idx - detail::ANIMTARGET_MATCOLOR_TEVKONST0_R) % 4, value);
            } break;
            }
        }

    };  // namespace lyt
};  // namespace nw4r
