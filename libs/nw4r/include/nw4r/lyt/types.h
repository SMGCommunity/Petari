#pragma once

#include "nw4r/math/types.h"
#include "nw4r/ut/LinkList.h"
#include <cstddef>

namespace nw4r {
    namespace lyt {

        struct Size {
            Size(f32 w, f32 h) {
                width = w;
                height = h;
            }

            f32 width;      // _0
            f32 height;     // _4
        };

        struct InflationLRTB {
            f32 l;      // _0
            f32 r;      // _4
            f32 t;      // _8
            f32 b;      // _C
        };

        struct ChanCtrl {
            u8 matSrcCol;       // _0
            u8 matSrcAlp;       // _1
            u8 reserve1;        // _2
            u8 reserve2;        // _3
        };

        struct TexSRT {
            math::VEC2 translate;
            f32 rotate;
            math::VEC2 scale;
        };

        struct TexCoordGen {
            u8 texGenType;      // _0
            u8 texGenSrc;       // _1
            u8 texMtx;          // _2
            u8 reserve;         // _3
        };

        struct IndirectStage {
            u8 texCoordGen;     // _0
            u8 texMap;          // _1
            u8 scaleS;          // _2
            u8 scaleT;          // _3
        };

        struct TevStageInOp {
            u8 ab;      // _0
            u8 cd;      // _1
            u8 op;      // _2
            u8 cl;      // _3
        };

        struct TevStage {
            u8 texCoordGen;         // _0
            u8 colChan;             // _1
            u8 texMap;              // _2
            u8 swapSel;             // _3
            TevStageInOp colIn;     // _4
            TevStageInOp alpIn;     // _8
            u8 indStage;            // _C
            u8 indBiMt;             // _D
            u8 indWrap;             // _E
            u8 indFoAdUtAl;         // _F
        };

        struct TevSwapMode {
            u8 swap;            // _4
        };

        struct AlphaCompare {
            u8 comp;        // _0
            u8 op;          // _1
            u8 ref0;        // _2
            u8 ref1;        // _3
        };

        struct BlendMode {
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
        };

        typedef ut::LinkList<AnimationLink, offsetof(AnimationLink, mLink)> AnimationList; 
    };
};

typedef nw4r::math::VEC2 (*TexCoordQuad)[4];
typedef const nw4r::math::VEC2 (*ConstTexCoordQuad)[4];