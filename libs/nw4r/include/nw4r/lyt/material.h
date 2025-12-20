#pragma once

#include "nw4r/lyt/animation.h"
#include "nw4r/lyt/texMap.h"
#include "nw4r/lyt/types.h"
#include "nw4r/ut/Color.h"
#include "revolution/vi.h"

namespace nw4r {
    namespace lyt {
        class TexMap;

        namespace detail {
            struct BitGXNums {
                u32 texMap : 4;
                u32 texSRT : 4;
                u32 texCoordGen : 4;
                u32 indSRT : 2;
                u32 indStage : 3;
                u32 tevSwap : 1;
                u32 tevStage : 5;
                u32 chanCtrl : 1;
                u32 matCol : 1;
                u32 alpComp : 1;
                u32 blendMode : 1;
            };
        };  // namespace detail
        class Material {
        public:
            Material(const res::Material*, const ResBlockSet&);

            const char* GetName() const { return mName; }
            bool IsUserAllocated() const { return mbUserAllocated; }

            virtual ~Material();
            virtual bool SetupGX(bool, u8);
            virtual void BindAnimation(AnimTransform*);
            virtual void UnbindAnimation(AnimTransform*);
            virtual void UnbindAllAnimation();
            virtual void Animate();
            virtual AnimationLink* FindAnimationLink(AnimTransform*);
            virtual void SetAnimationEnable(AnimTransform*, bool);

            void Init();
            void InitBitGXNums(detail::BitGXNums*);
            void ReserveGXMem(u8 texMapNum, u8 texSRTNum, u8 texCoordGenNum, u8 tevStageNum, bool allocTevSwap, u8 indStageNum, u8 indSRTNum,
                              bool allocChanCtrl, bool allocMatCol, bool allocAlpComp, bool allocBlendMode);

            u8 GetTextureNum() const { return u8(mGXMemNum.texMap); }
            void SetTextureNum(u8 num);

            u8 GetTextureCap() const { return u8(mGXMemCap.texMap); }
            u8 GetTexCoordGenCap() const { return u8(mGXMemCap.texCoordGen); }

            void SetTexCoordGenNum(u8 num);
            void SetTexture(u8 texMapIdx, const TexMap& texMap) { GetTexMapAry()[texMapIdx].Set(texMap); }

            void SetTexCoordGen(u32 idx, TexCoordGen value) { GetTexCoordGenAry()[idx] = value; }

            bool IsAlphaCompareCap() const { return mGXMemCap.alpComp; }

            bool IsTevSwapCap() const { return mGXMemCap.tevSwap; }

            bool IsChanCtrlCap() const { return mGXMemCap.chanCtrl; }

            bool IsMatColorCap() const { return mGXMemCap.matCol; }

            bool IsBlendModeCap() const { return mGXMemCap.blendMode; }

            const TexMap& GetTexture(u8 texMapIdx) const { return GetTexMapAry()[texMapIdx]; }

            const TexMap* GetTexMapAry() const;
            TexMap* GetTexMapAry();
            const TexCoordGen* GetTexCoordGenAry() const;
            TexCoordGen* GetTexCoordGenAry();
            TexSRT* GetTexSRTAry();
            ChanCtrl* GetChanCtrlAry();
            ut::Color* GetMatColAry();
            TevSwapMode* GetTevSwapAry();
            AlphaCompare* GetAlphaComparePtr();
            BlendMode* GetBlendModePtr();
            IndirectStage* GetIndirectStageAry();
            TexSRT* GetIndTexSRTAry();
            TevStage* GetTevStageAry();
            void SetName(const char*);
            void SetTevStageNum(u8);
            void SetIndStageNum(u8);
            void SetColorElement(u32, s16);
            void AddAnimationLink(AnimationLink*);

            AnimationList mAnimList;
            GXColorS10 mTevCols[3];
            ut::Color mTevKCols[4];
            detail::BitGXNums mGXMemCap;
            detail::BitGXNums mGXMemNum;
            void* mpGXMem;
            char mName[21];
            u8 mbUserAllocated;
        };
    };  // namespace lyt
};  // namespace nw4r
