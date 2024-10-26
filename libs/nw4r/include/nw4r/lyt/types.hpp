#pragma once

#include "nw4r/math/types.hpp"
#include "nw4r/ut/LinkList.hpp"
#include <cstddef>

namespace nw4r {
    namespace lyt {

        struct Size {
            Size(f32 w, f32 h) {
                width = w;
                height = h;
            }

            f32 width;      // 0x0
            f32 height;     // 0x4
        };

        struct InflationLRTB {
            f32 l;      // 0x0
            f32 r;      // 0x4
            f32 t;      // 0x8
            f32 b;      // 0xC
        };

        struct ChanCtrl {
            ChanCtrl() : reserve1(0), reserve2(0) {
                Set(GX_SRC_VTX, GX_SRC_VTX);
            }

            void Set(GXColorSrc colSrc, GXColorSrc alpSrc) {
                matSrcCol = colSrc;
                matSrcAlp = alpSrc;
            }

            GXColorSrc GetColorSrc() const {
                return GXColorSrc(matSrcCol);
            }

            GXColorSrc GetAlphaSrc() const {
                return GXColorSrc(matSrcAlp);
            }

            u8 matSrcCol;       // 0x0
            u8 matSrcAlp;       // 0x1
            u8 reserve1;        // 0x2
            u8 reserve2;        // 0x3
        };

        struct TexSRT {
            math::VEC2 translate;
            f32 rotate;
            math::VEC2 scale;
        };

        struct TexCoordGen {
            TexCoordGen() {
                reserve = 0;
                Set(GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
            }

            void Set(GXTexGenType type, GXTexGenSrc src, u32 mtx) {
                texGenType = type;
                texGenSrc = src;
                texMtx = mtx;
            }

            GXTexGenType GetTexGenType() const {
                return GXTexGenType(texGenType);
            }

            GXTexGenSrc GetTexGenSrc() const {
                return GXTexGenSrc(texGenSrc);
            }

            u32 GetTexMtx() const {
                return texMtx;
            }

            u8 texGenType;      // 0x0
            u8 texGenSrc;       // 0x1
            u8 texMtx;          // 0x2
            u8 reserve;         // 0x3
        };

        struct IndirectStage {

            IndirectStage() {
                Set(GX_TEXCOORD0, GX_TEXMAP0, GX_ITS_1, GX_ITS_1);
            }

            void Set(GXTexCoordID id, GXTexMapID mapID, GXIndTexScale scale_s, GXIndTexScale scale_t) {
                texCoordGen = id;
                texMap = mapID;
                scaleS = scale_s;
                scaleT = scale_t;
            }

            GXTexCoordID GetTexCoordGen() const { 
                return GXTexCoordID(texCoordGen);
            }

            GXTexMapID GetTexMap() const { 
                return GXTexMapID(texMap);
            }

            GXIndTexScale GetScaleS() const { 
                return GXIndTexScale(scaleS);
            }

            GXIndTexScale GetScaleT() const { 
                return GXIndTexScale(scaleT);
            }

            u8 texCoordGen;     // 0x0
            u8 texMap;          // 0x1
            u8 scaleS;          // 0x2
            u8 scaleT;          // 0x3
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

            u8 GetA() const { 
                return u8((ab >> 0) & 0xF); 
            }

            u8 GetB() const { 
                return u8((ab >> 4) & 0xF); 
            }

            u8 GetC() const { 
                return u8((cd >> 0) & 0xF); 
            }

            u8 GetD() const { 
                return u8((cd >> 4) & 0xF); 
            }

            u8 GetOp() const { 
                return u8((op >> 0) & 0xF); 
            }

            u8 GetBias() const { 
                return u8((op >> 4) & 0x3); 
            }

            u8 GetScale() const { 
                return u8((op >> 6) & 0x3); 
            }

            bool IsClamp() const { 
                return ((cl >> 0) & 0x1) != 0; 
            }

            u8 GetOutReg() const { 
                return u8((cl >>  1) & 0x3); 
            }

            u8 GetKSel() const { 
                return u8((cl >> 3) & 0x1F);
            }

            u8 ab;      // 0x0
            u8 cd;      // 0x1
            u8 op;      // 0x2
            u8 cl;      // 0x3
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

            void SetOrder(GXTexCoordID coordGen, GXTexMapID _texMap, GXChannelID chan, GXTevSwapSel rasSel, GXTevSwapSel texSel) {
                texCoordGen = coordGen;
                colChan = chan;
                texMap = _texMap;
                swapSel = u8(((_texMap >> 8) << 0) | (rasSel << 1) | (texSel << 3));
            }

            void SetColorIn(GXTevColorArg a, GXTevColorArg b, GXTevColorArg c, GXTevColorArg d) {
                colIn.SetIn(a, b, c, d);
            }

            void SetAlphaIn(GXTevAlphaArg a, GXTevAlphaArg b, GXTevAlphaArg c, GXTevAlphaArg d) {
                alpIn.SetIn(a, b, c, d);
            }

            void SetColorOp(GXTevOp op, GXTevBias bias, GXTevScale scale, bool clamp, GXTevRegID outReg, GXTevKColorSel kSel) {
                colIn.SetOp(op, bias, scale, clamp, outReg, kSel);
            }

            void SetAlphaOp(GXTevOp op, GXTevBias bias, GXTevScale scale, bool clamp, GXTevRegID outReg, GXTevKAlphaSel kSel) {
                alpIn.SetOp(op, bias, scale, clamp, outReg, kSel);
            }

            void SetIndirect(GXIndTexStageID stage, GXIndTexFormat format, GXIndTexBiasSel biasSel, GXIndTexMtxID mtxSel, GXIndTexWrap wrapS, GXIndTexWrap wrapT, bool addPrev, bool utcLod, GXIndTexAlphaSel alphaSel) {
                indStage = stage;
                indBiMt = u8((biasSel << 0) | (mtxSel << 3));
                indWrap = u8((wrapS << 0) | (wrapT << 3));
                indFoAdUtAl = u8((format << 0) | ((addPrev ? 1 : 0) << 2) | ((utcLod ? 1 : 0) << 3) | (alphaSel << 4));
            }

            GXTexCoordID GetTexCoordGen() const { 
                return GXTexCoordID(texCoordGen); 
            }
            
            GXTexMapID GetTexMap() const { 
                return GXTexMapID  ((((swapSel >> 0) & 0x1) << 8) | texMap); 
            }

            GXChannelID GetColorChan() const { 
                return GXChannelID(colChan); 
            }

            GXTevSwapSel GetRasSwapSel() const { 
                return GXTevSwapSel((swapSel >> 1) & 0x3); 
            }

            GXTevSwapSel GetTexSwapSel() const { 
                return GXTevSwapSel((swapSel >> 3) & 0x3); 
            }

            GXTevColorArg GetColorInA() const { 
                return GXTevColorArg(colIn.GetA());
            }

            GXTevColorArg GetColorInB() const { 
                return GXTevColorArg(colIn.GetB());
            }

            GXTevColorArg GetColorInC() const { 
                return GXTevColorArg(colIn.GetC());
            }

            GXTevColorArg GetColorInD() const { 
                return GXTevColorArg(colIn.GetD());
            }

            GXTevAlphaArg GetAlphaInA() const { 
                return GXTevAlphaArg(alpIn.GetA()); 
            }

            GXTevAlphaArg GetAlphaInB() const { 
                return GXTevAlphaArg(alpIn.GetB()); 
            }

            GXTevAlphaArg GetAlphaInC() const { 
                return GXTevAlphaArg(alpIn.GetC()); 
            }

            GXTevAlphaArg GetAlphaInD() const { 
                return GXTevAlphaArg(alpIn.GetD()); 
            }

            GXTevOp GetColorOp() const { 
                return GXTevOp(colIn.GetOp()); 
            }

            GXTevBias GetColorBias() const { 
                return GXTevBias(colIn.GetBias()); 
            }

            GXTevScale GetColorScale() const { 
                return GXTevScale(colIn.GetScale()); 
            }

            bool IsColorClamp() const {                 
                return colIn.IsClamp(); 
            }

            GXTevRegID GetColorOutReg() const { 
                return GXTevRegID(colIn.GetOutReg()); 
            }

            GXTevKColorSel GetKColorSel() const { 
                return GXTevKColorSel(colIn.GetKSel()); 
            }

            GXTevOp GetAlphaOp() const { 
                return GXTevOp(alpIn.GetOp());
            }

            GXTevBias GetAlphaBias() const { 
                return GXTevBias(alpIn.GetBias());
            }

            GXTevScale GetAlphaScale() const { 
                return GXTevScale(alpIn.GetScale());
            }

            bool IsAlphaClamp() const { 
                return alpIn.IsClamp(); 
            }

            GXTevRegID GetAlphaOutReg() const { 
                return GXTevRegID(alpIn.GetOutReg());
            }

            GXTevKAlphaSel GetKAlphaSel() const { 
                return GXTevKAlphaSel(alpIn.GetKSel());
            }

            GXIndTexStageID GetIndStage() const { 
                return GXIndTexStageID (indStage);
            }

            GXIndTexFormat GetIndFormat() const { 
                return GXIndTexFormat  ((indFoAdUtAl >> 0) & 0x3);
            }

            GXIndTexBiasSel GetIndBiasSel() const { 
                return GXIndTexBiasSel ((indBiMt >> 0) & 0x7);
            }

            GXIndTexMtxID GetIndMtxSel() const { 
                return GXIndTexMtxID   ((indBiMt >> 3) & 0xF);
            }

            GXIndTexWrap GetIndWrapS() const { 
                return GXIndTexWrap    ((indWrap >> 0) & 0x7);
            }

            GXIndTexWrap GetIndWrapT() const { 
                return GXIndTexWrap    ((indWrap >> 3) & 0x7);
            }

            bool IsIndAddPrev() const { 
                return ((indFoAdUtAl >> 2) & 0x1) != 0;
            }

            bool IsIndUtcLod() const { 
                return ((indFoAdUtAl >> 3) & 0x1) != 0;
            }

            GXIndTexAlphaSel GetIndAlphaSel() const  { 
                return GXIndTexAlphaSel((indFoAdUtAl >> 4) & 0x3);
            }

            u8 texCoordGen;         // 0x0
            u8 colChan;             // 0x1
            u8 texMap;              // 0x2
            u8 swapSel;             // 0x3
            TevStageInOp colIn;     // 0x4
            TevStageInOp alpIn;     // 0x8
            u8 indStage;            // 0xC
            u8 indBiMt;             // 0xD
            u8 indWrap;             // 0xE
            u8 indFoAdUtAl;         // 0xF
        };

        struct TevSwapMode {
            TevSwapMode() {
                Set(GX_CH_RED, GX_CH_GREEN, GX_CH_BLUE, GX_CH_ALPHA);
            }

            void Set(GXTevColorChan r, GXTevColorChan g, GXTevColorChan b, GXTevColorChan a) {
                swap = u8(((r - GX_CH_RED) << 0) | ((g - GX_CH_RED) << 2) | ((b - GX_CH_RED) << 4) | ((a - GX_CH_RED) << 6));
            }

            GXTevColorChan GetR() const { 
                return GXTevColorChan(GX_CH_RED + ((swap >> 0) & 0x3)); 
            }

            GXTevColorChan GetG() const { 
                return GXTevColorChan(GX_CH_RED + ((swap >> 2) & 0x3));
            }

            GXTevColorChan GetB() const { 
                return GXTevColorChan(GX_CH_RED + ((swap >> 4) & 0x3));
            }
    
            GXTevColorChan GetA() const { 
                return GXTevColorChan(GX_CH_RED + ((swap >> 6) & 0x3));
            }

            u8 swap;            // 0x4
        };

        struct AlphaCompare {
            AlphaCompare() {
                Set(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
            }

            void Set(GXCompare aComp0, u8 aRef0, GXAlphaOp aOp, GXCompare aComp1, u8 aRef1) {
                comp = u8(aComp0 << 0 | aComp1 << 4);
                op = aOp;
                ref0 = aRef0;
                ref1 = aRef1;
            }


            GXCompare GetComp0() const {
                return GXCompare(comp & 0xF);
            }

            u8 GetRef0() const {
                return ref0;
            }

            GXAlphaOp GetOp() const {
                return GXAlphaOp(op);
            }

            GXCompare GetComp1() const {
                return GXCompare((comp >> 4) & 0xF);
            }

            u8 GetRef1() const {
                return ref1;
            }

            u8 comp;        // 0x0
            u8 op;          // 0x1
            u8 ref0;        // 0x2
            u8 ref1;        // 0x3
        };

        struct BlendMode {
            BlendMode() {
                Set(GX_BM_BLEND, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_SET);
            }

            void Set(GXBlendMode aType, GXBlendFactor aSrcFactor, GXBlendFactor aDstFactor, GXLogicOp aOp) {
                type = aType;
                srcFactor = aSrcFactor;
                dstFactor = aDstFactor;
                op = aOp;
            }

            GXBlendMode GetType() const { 
                return GXBlendMode(type);
            }
            
            GXBlendFactor GetSrcFactor() const { 
                return GXBlendFactor(srcFactor);
            }

            GXBlendFactor GetDstFactor() const {
                return GXBlendFactor(dstFactor);
            }

            GXLogicOp GetOp() const {
                return GXLogicOp(op);
            }

            u8 type;
            u8 srcFactor;
            u8 dstFactor;
            u8 op;
        };

        class AnimTransform;

        class AnimationLink {
        public:

            AnimTransform* GetAnimTransform() const {
                return mAnimTrans;
            }

            u16 GetIndex() const {
                return mIdx;
            }

            bool IsEnable() const {
                return !mbDisable;
            }

            void SetEnable(bool en) {
                mbDisable = !en;
            }

            ut::LinkListNode mLink;
        private:
            AnimTransform* mAnimTrans;
            u16 mIdx;
            bool mbDisable;
        };

        namespace detail {
            template<typename T>
            inline void SetBit(T* pBits, int pos, bool val) {
                const T mask = T(~(1 << pos));
                *pBits &= mask;
                *pBits |= (val ? 1 : 0) << pos;
            }

            template<typename T>
            inline bool TestBit(T bits, int pos) {
                const T mask = T(1 << pos);
                return 0 != (bits & mask);
            }

            template<typename T>
            const T* ConvertOffsToPtr(const void *baseAddr, u32 offs) {
                return reinterpret_cast<const T*>(static_cast<const u8 *>(baseAddr) + offs);
            }

            template<typename T>
            T* ConvertOffsToPtr(void *baseAddr, u32 offs) {
                return reinterpret_cast<T*>(static_cast<u8 *>(baseAddr) + offs);
            }
        };

        #ifdef __MWERKS__
        typedef ut::LinkList<AnimationLink, offsetof(AnimationLink, mLink)> AnimationList;
        #else
        typedef ut::LinkList<AnimationLink, 0> AnimationList;
        #endif
    };
};

typedef nw4r::math::VEC2 (*TexCoordQuad)[4];
typedef const nw4r::math::VEC2 (*ConstTexCoordQuad)[4];
