#pragma once

#include "nw4r/math/types.h"
#include "nw4r/ut/LinkList.h"
#include <cstddef>
#include <mem.h>
#include <revolution.h>

namespace nw4r {
    namespace lyt {
        namespace detail {
            typedef s16 ResS16;
            typedef u16 ResU16;
            typedef s32 ResS32;
            typedef u32 ResU32;

            template < typename T >
            inline const T* ConvertOffsToPtr(const void* baseAddr, unsigned int offs) {
                return reinterpret_cast< const T* >(static_cast< const u8* >(baseAddr) + offs);
            }

            template < typename T >
            inline T* ConvertOffsToPtr(void* baseAddress, unsigned int offset) {
                return reinterpret_cast< T* >(static_cast< u8* >(baseAddress) + offset);
            }

            template < typename T >
            inline void SetBits(T* pBits, int pos, int len, T val) {
                const u32 MaxValue = 0xFFFFFFFFU >> (32 - len);
                const T mask = T(~(MaxValue << pos));
                *pBits &= mask;
                *pBits |= val << pos;
            }

            template < typename T >
            inline T GetBits(T bits, int pos, int len) {
                const u32 mask = ~(0xFFFFFFFFU << len);
                return T((bits >> pos) & mask);
            }

            template < typename T >
            inline bool TestBit(T bits, int pos) {
                const T mask = T(1 << pos);

                return 0 != (bits & mask);
            }

            enum {
                ANIMTARGET_MATCOLOR_MATCOLOR0_R,
                ANIMTARGET_MATCOLOR_MATCOLOR0_G,
                ANIMTARGET_MATCOLOR_MATCOLOR0_B,
                ANIMTARGET_MATCOLOR_MATCOLOR0_A,
                ANIMTARGET_MATCOLOR_TEVCOLOR0_R,
                ANIMTARGET_MATCOLOR_TEVCOLOR0_G,
                ANIMTARGET_MATCOLOR_TEVCOLOR0_B,
                ANIMTARGET_MATCOLOR_TEVCOLOR0_A,
                ANIMTARGET_MATCOLOR_TEVCOLOR1_R,
                ANIMTARGET_MATCOLOR_TEVCOLOR1_G,
                ANIMTARGET_MATCOLOR_TEVCOLOR1_B,
                ANIMTARGET_MATCOLOR_TEVCOLOR1_A,
                ANIMTARGET_MATCOLOR_TEVCOLOR2_R,
                ANIMTARGET_MATCOLOR_TEVCOLOR2_G,
                ANIMTARGET_MATCOLOR_TEVCOLOR2_B,
                ANIMTARGET_MATCOLOR_TEVCOLOR2_A,
                ANIMTARGET_MATCOLOR_TEVKONST0_R,
                ANIMTARGET_MATCOLOR_TEVKONST0_G,
                ANIMTARGET_MATCOLOR_TEVKONST0_B,
                ANIMTARGET_MATCOLOR_TEVKONST0_A,
                ANIMTARGET_MATCOLOR_TEVKONST1_R,
                ANIMTARGET_MATCOLOR_TEVKONST1_G,
                ANIMTARGET_MATCOLOR_TEVKONST1_B,
                ANIMTARGET_MATCOLOR_TEVKONST1_A,
                ANIMTARGET_MATCOLOR_TEVKONST2_R,
                ANIMTARGET_MATCOLOR_TEVKONST2_G,
                ANIMTARGET_MATCOLOR_TEVKONST2_B,
                ANIMTARGET_MATCOLOR_TEVKONST2_A,
                ANIMTARGET_MATCOLOR_TEVKONST3_R,
                ANIMTARGET_MATCOLOR_TEVKONST3_G,
                ANIMTARGET_MATCOLOR_TEVKONST3_B,
                ANIMTARGET_MATCOLOR_TEVKONST3_A,

                ANIMTARGET_MATCOLOR_MAX
            };
        };  // namespace detail

        struct Size {
            Size() : width(0.0f), height(0.0f) {}

            Size(f32 aWidth, f32 aHeight) : width(aWidth), height(aHeight) {}

            f32 width;
            f32 height;
        };

        inline bool operator==(const Size& a, const Size& b) {
            return a.width == b.width && a.height == b.height;
        }

        struct InflationLRTB {
            f32 l;
            f32 r;
            f32 t;
            f32 b;
        };

        struct WindowFrameSize {
            f32 l;
            f32 r;
            f32 t;
            f32 b;
        };

        struct ChanCtrl {
            ChanCtrl() : reserve1(0), reserve2(0) { Set(GX_SRC_VTX, GX_SRC_VTX); }

            ChanCtrl(GXColorSrc colSrc, GXColorSrc alpSrc) : reserve1(0), reserve2(0) { Set(colSrc, alpSrc); }

            void Set(GXColorSrc colSrc, GXColorSrc alpSrc) {
                matSrcCol = colSrc;
                matSrcAlp = alpSrc;
            }

            GXColorSrc GetColorSrc() const { return GXColorSrc(matSrcCol); }
            GXColorSrc GetAlphaSrc() const { return GXColorSrc(matSrcAlp); }

            u8 matSrcCol;
            u8 matSrcAlp;
            u8 reserve1;
            u8 reserve2;
        };

        struct TexSRT {
            TexSRT& operator=(const TexSRT&);

            math::VEC2 translate;
            f32 rotate;
            math::VEC2 scale;
        };

        struct TexCoordGen {
            TexCoordGen() : reserve(0) { Set(GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY); }

            TexCoordGen(GXTexGenType aTexGenType, GXTexGenSrc aTexGenSrc, u32 aTexMtx)
                : reserve(0)

            {
                Set(aTexGenType, aTexGenSrc, aTexMtx);
            }

            void Set(GXTexGenType aTexGenType, GXTexGenSrc aTexGenSrc, u32 aTexMtx) {
                texGenType = aTexGenType;
                texGenSrc = aTexGenSrc;
                texMtx = u8(aTexMtx);
            }

            GXTexGenType GetTexGenType() const { return GXTexGenType(texGenType); }
            GXTexGenSrc GetTexGenSrc() const { return GXTexGenSrc(texGenSrc); }
            u32 GetTexMtx() const { return texMtx; }

            u8 texGenType;
            u8 texGenSrc;
            u8 texMtx;
            u8 reserve;
        };

        struct IndirectStage {
            IndirectStage() { Set(GX_TEXCOORD0, GX_TEXMAP0, GX_ITS_1, GX_ITS_1); }

            IndirectStage(GXTexCoordID aTexCoordGen, GXTexMapID aTexMap, GXIndTexScale aScaleS, GXIndTexScale aScaleT) {
                Set(aTexCoordGen, aTexMap, aScaleS, aScaleT);
            }

            void Set(GXTexCoordID aTexCoordGen, GXTexMapID aTexMap, GXIndTexScale aScaleS, GXIndTexScale aScaleT) {
                texCoordGen = aTexCoordGen;
                texMap = aTexMap;
                scaleS = aScaleS;
                scaleT = aScaleT;
            }
            GXTexCoordID GetTexCoordGen() const { return GXTexCoordID(texCoordGen); }
            GXTexMapID GetTexMap() const { return GXTexMapID(texMap); }
            GXIndTexScale GetScaleS() const { return GXIndTexScale(scaleS); }
            GXIndTexScale GetScaleT() const { return GXIndTexScale(scaleT); }

            u8 texCoordGen;
            u8 texMap;
            u8 scaleS;
            u8 scaleT;
        };

        struct TevStageInOp {
            void SetIn(u8 a, u8 b, u8 c, u8 d) {
                ab = u8((a << 0) | (b << 4));
                cd = u8((c << 0) | (d << 4));
            }

            void SetOp(u8 aOp, u8 bias, u8 scale, bool clamp, u8 outReg, u8 kSel) {
                op = u8((aOp << 0) | (bias << 4) | (scale << 6));
                cl = u8(((clamp ? 1 : 0) << 0) | (outReg << 1) | (kSel << 3));
            }

            u8 GetA() const { return u8((ab >> 0) & 0xF); }
            u8 GetB() const { return u8((ab >> 4) & 0xF); }
            u8 GetC() const { return u8((cd >> 0) & 0xF); }
            u8 GetD() const { return u8((cd >> 4) & 0xF); }

            u8 GetOp() const { return u8((op >> 0) & 0xF); }
            u8 GetBias() const { return u8((op >> 4) & 0x3); }
            u8 GetScale() const { return u8((op >> 6) & 0x3); }
            bool IsClamp() const { return ((cl >> 0) & 0x1) != 0; }
            u8 GetOutReg() const { return u8((cl >> 1) & 0x3); }
            u8 GetKSel() const { return u8((cl >> 3) & 0x1F); }

            u8 ab;
            u8 cd;
            u8 op;
            u8 cl;
        };

        struct TevStage {
            TevStage() {
                SetOrder(GX_TEXCOORD_NULL, GX_TEXMAP_NULL, GX_COLOR0A0, GX_TEV_SWAP0, GX_TEV_SWAP0);
                SetColorIn(GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_RASC);
                SetAlphaIn(GX_CA_ZERO, GX_CA_ZERO, GX_CA_ZERO, GX_CA_RASA);
                SetColorOp(GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV, GX_TEV_KCSEL_K0);
                SetAlphaOp(GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV, GX_TEV_KASEL_K0_R);
                SetIndirect(GX_INDTEXSTAGE0, GX_ITF_8, GX_ITB_NONE, GX_ITM_OFF, GX_ITW_OFF, GX_ITW_OFF, false, false, GX_ITBA_OFF);
            }

            void SetOrder(GXTexCoordID aTexCoordGen, GXTexMapID aTexMap, GXChannelID aColChan, GXTevSwapSel rasSel, GXTevSwapSel texSel) {
                texCoordGen = aTexCoordGen;
                colChan = aColChan;
                texMap = u8(aTexMap);
                swapSel = u8(((aTexMap >> 8) << 0) | (rasSel << 1) | (texSel << 3));
            }

            void SetColorIn(GXTevColorArg a, GXTevColorArg b, GXTevColorArg c, GXTevColorArg d) { colIn.SetIn(a, b, c, d); }

            void SetAlphaIn(GXTevAlphaArg a, GXTevAlphaArg b, GXTevAlphaArg c, GXTevAlphaArg d) { alpIn.SetIn(a, b, c, d); }

            void SetColorOp(GXTevOp op, GXTevBias bias, GXTevScale scale, bool clamp, GXTevRegID outReg, GXTevKColorSel kSel) {
                colIn.SetOp(op, bias, scale, clamp, outReg, kSel);
            }

            void SetAlphaOp(GXTevOp op, GXTevBias bias, GXTevScale scale, bool clamp, GXTevRegID outReg, GXTevKAlphaSel kSel) {
                alpIn.SetOp(op, bias, scale, clamp, outReg, kSel);
            }

            void SetIndirect(GXIndTexStageID stage, GXIndTexFormat format, GXIndTexBiasSel biasSel, GXIndTexMtxID mtxSel, GXIndTexWrap wrapS,
                             GXIndTexWrap wrapT, bool addPrev, bool utcLod, GXIndTexAlphaSel alphaSel) {
                indStage = stage;
                indBiMt = u8((biasSel << 0) | (mtxSel << 3));
                indWrap = u8((wrapS << 0) | (wrapT << 3));
                indFoAdUtAl = u8((format << 0) | ((addPrev ? 1 : 0) << 2) | ((utcLod ? 1 : 0) << 3) | (alphaSel << 4));
            }

            GXTexCoordID GetTexCoordGen() const { return GXTexCoordID(texCoordGen); }
            GXTexMapID GetTexMap() const { return GXTexMapID((((swapSel >> 0) & 0x1) << 8) | texMap); }
            GXChannelID GetColorChan() const { return GXChannelID(colChan); }
            GXTevSwapSel GetRasSwapSel() const { return GXTevSwapSel((swapSel >> 1) & 0x3); }
            GXTevSwapSel GetTexSwapSel() const { return GXTevSwapSel((swapSel >> 3) & 0x3); }

            GXTevColorArg GetColorInA() const { return GXTevColorArg(colIn.GetA()); }
            GXTevColorArg GetColorInB() const { return GXTevColorArg(colIn.GetB()); }
            GXTevColorArg GetColorInC() const { return GXTevColorArg(colIn.GetC()); }
            GXTevColorArg GetColorInD() const { return GXTevColorArg(colIn.GetD()); }

            GXTevAlphaArg GetAlphaInA() const { return GXTevAlphaArg(alpIn.GetA()); }
            GXTevAlphaArg GetAlphaInB() const { return GXTevAlphaArg(alpIn.GetB()); }
            GXTevAlphaArg GetAlphaInC() const { return GXTevAlphaArg(alpIn.GetC()); }
            GXTevAlphaArg GetAlphaInD() const { return GXTevAlphaArg(alpIn.GetD()); }

            GXTevOp GetColorOp() const { return GXTevOp(colIn.GetOp()); }
            GXTevBias GetColorBias() const { return GXTevBias(colIn.GetBias()); }
            GXTevScale GetColorScale() const { return GXTevScale(colIn.GetScale()); }
            bool IsColorClamp() const { return colIn.IsClamp(); }
            GXTevRegID GetColorOutReg() const { return GXTevRegID(colIn.GetOutReg()); }
            GXTevKColorSel GetKColorSel() const { return GXTevKColorSel(colIn.GetKSel()); }

            GXTevOp GetAlphaOp() const { return GXTevOp(alpIn.GetOp()); }
            GXTevBias GetAlphaBias() const { return GXTevBias(alpIn.GetBias()); }
            GXTevScale GetAlphaScale() const { return GXTevScale(alpIn.GetScale()); }
            bool IsAlphaClamp() const { return alpIn.IsClamp(); }
            GXTevRegID GetAlphaOutReg() const { return GXTevRegID(alpIn.GetOutReg()); }
            GXTevKAlphaSel GetKAlphaSel() const { return GXTevKAlphaSel(alpIn.GetKSel()); }

            GXIndTexStageID GetIndStage() const { return GXIndTexStageID(indStage); }
            GXIndTexFormat GetIndFormat() const { return GXIndTexFormat((indFoAdUtAl >> 0) & 0x3); }
            GXIndTexBiasSel GetIndBiasSel() const { return GXIndTexBiasSel((indBiMt >> 0) & 0x7); }
            GXIndTexMtxID GetIndMtxSel() const { return GXIndTexMtxID((indBiMt >> 3) & 0xF); }
            GXIndTexWrap GetIndWrapS() const { return GXIndTexWrap((indWrap >> 0) & 0x7); }
            GXIndTexWrap GetIndWrapT() const { return GXIndTexWrap((indWrap >> 3) & 0x7); }
            bool IsIndAddPrev() const { return ((indFoAdUtAl >> 2) & 0x1) != 0; }
            bool IsIndUtcLod() const { return ((indFoAdUtAl >> 3) & 0x1) != 0; }
            GXIndTexAlphaSel GetIndAlphaSel() const { return GXIndTexAlphaSel((indFoAdUtAl >> 4) & 0x3); }

            u8 texCoordGen;
            u8 colChan;
            u8 texMap;
            u8 swapSel;

            TevStageInOp colIn;
            TevStageInOp alpIn;

            u8 indStage;
            u8 indBiMt;
            u8 indWrap;
            u8 indFoAdUtAl;
        };

        struct TevSwapMode {
            TevSwapMode() { Set(GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA); }

            TevSwapMode(GXTevColorChan r, GXTevColorChan g, GXTevColorChan b, GXTevColorChan a) { Set(r, g, b, a); }

            void Set(GXTevColorChan r, GXTevColorChan g, GXTevColorChan b, GXTevColorChan a) {
                swap = u8(((r - GX_CH_RED) << 0) | ((g - GX_CH_RED) << 2) | ((b - GX_CH_RED) << 4) | ((a - GX_CH_RED) << 6));
            }

            GXTevColorChan GetR() const { return GXTevColorChan(GX_CH_RED + ((swap >> 0) & 0x3)); }
            GXTevColorChan GetG() const { return GXTevColorChan(GX_CH_RED + ((swap >> 2) & 0x3)); }
            GXTevColorChan GetB() const { return GXTevColorChan(GX_CH_RED + ((swap >> 4) & 0x3)); }
            GXTevColorChan GetA() const { return GXTevColorChan(GX_CH_RED + ((swap >> 6) & 0x3)); }

            u8 swap;
        };

        struct AlphaCompare {
            AlphaCompare() { Set(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0); }

            AlphaCompare(GXCompare aComp0, u8 aRef0, GXAlphaOp aOp, GXCompare aComp1, u8 aRef1) { Set(aComp0, aRef0, aOp, aComp1, aRef1); }

            void Set(GXCompare aComp0, u8 aRef0, GXAlphaOp aOp, GXCompare aComp1, u8 aRef1) {
                comp = u8(aComp0 << 0 | aComp1 << 4);
                op = aOp;
                ref0 = aRef0;
                ref1 = aRef1;
            }

            GXCompare GetComp0() const { return GXCompare((comp >> 0) & 0xF); }
            u8 GetRef0() const { return ref0; }
            GXAlphaOp GetOp() const { return GXAlphaOp(op); }
            GXCompare GetComp1() const { return GXCompare((comp >> 4) & 0xF); }
            u8 GetRef1() const { return ref1; }

            u8 comp;
            u8 op;
            u8 ref0;
            u8 ref1;
        };

        struct BlendMode {
            BlendMode() { Set(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET); }

            BlendMode(GXBlendMode aType, GXBlendFactor aSrcFactor, GXBlendFactor aDstFactor, GXLogicOp aOp) {
                Set(aType, aSrcFactor, aDstFactor, aOp);
            }

            void Set(GXBlendMode aType, GXBlendFactor aSrcFactor, GXBlendFactor aDstFactor, GXLogicOp aOp) {
                type = aType;
                srcFactor = aSrcFactor;
                dstFactor = aDstFactor;
                op = aOp;
            }

            GXBlendMode GetType() const { return GXBlendMode(type); }
            GXBlendFactor GetSrcFactor() const { return GXBlendFactor(srcFactor); }
            GXBlendFactor GetDstFactor() const { return GXBlendFactor(dstFactor); }
            GXLogicOp GetOp() const { return GXLogicOp(op); }

            u8 type;
            u8 srcFactor;
            u8 dstFactor;
            u8 op;
        };

        class AnimTransform;

        class AnimationLink {
        public:
            AnimationLink() { Reset(); }

            void Reset() { Set(nullptr, 0, false); }

            void Set(AnimTransform* pTrans, u16 idx, bool dis) {
                mAnimTrans = pTrans;
                mIdx = idx;
                mbDisable = dis;
            }

            bool IsEnable() const { return !mbDisable; }
            void SetEnable(bool enable) { mbDisable = !enable; }
            u16 GetIndex() const { return mIdx; }
            AnimTransform* GetAnimTransform() const { return mAnimTrans; }

            ut::LinkListNode mLink;
            AnimTransform* mAnimTrans;
            u16 mIdx;
            bool mbDisable;
        };

#ifdef __MWERKS__
        typedef ut::LinkList< AnimationLink, offsetof(AnimationLink, mLink) > AnimationList;
#else
        typedef ut::LinkList< AnimationLink, 0 > AnimationList;
#endif

        struct AnimationGroupRef {
            AnimationGroupRef() : flag(0) {
                memset(name, 0, sizeof(name));
                memset(padding, 0, sizeof(padding));
            }

            const char* GetName() const { return name; }

            char name[17];
            u8 flag;
            u8 padding[2];
        };

        struct AnimationShareInfo {
            const char* GetSrcPaneName() const { return srcPaneName; }

            const char* GetTargetGroupName() const { return targetGroupName; }

            char srcPaneName[17];
            char targetGroupName[17];
            u8 padding[2];
        };
    };  // namespace lyt
};  // namespace nw4r
