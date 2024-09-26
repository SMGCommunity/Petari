#pragma once

#include "nw4r/lyt/types.hpp"
#include "nw4r/lyt/lyt_texMap.hpp"
#include "nw4r/ut/Color.hpp"

namespace nw4r {
    namespace lyt {
        namespace detail {
            struct BitGXNums {
                u32 texMap          : 4;
                u32 texSRT          : 4;
                u32 texCoordGen     : 4;
                u32 indSRT          : 2;
                u32 indStage        : 3;
                u32 tevSwap         : 1;
                u32 tevStage        : 5;
                u32 chanCtrl        : 1;
                u32 matCol          : 1;
                u32 alpComp         : 1;
                u32 blendMode       : 1;
            };
        };

        class Material {
        public:
            virtual ~Material();
            virtual bool SetupGX(bool, u8);
            virtual void BindAnimation(AnimTransform *);
            virtual void UnbindAnimation(AnimTransform *);
            virtual void UnbindAllAnimation();
            virtual void Animate();
            virtual AnimationLink* FindAnimationLink(AnimTransform *);
            virtual void SetAnimationEnable(AnimTransform *, bool);

            const TexMap* GetTexMapAry() const;
            TexMap* GetTexMapAry();
            TexSRT* GetTexSRTAry();
            TexCoordGen* GetTexCoordGenAry();
            ChanCtrl* GetChanCtrlAry();
            const ut::Color* GetMatColAry() const;
            ut::Color* GetMatColAry();
            TevSwapMode* GetTevSwapAry();
            AlphaCompare* GetAlphaComparePtr();
            BlendMode* GetBlendModePtr();
            IndirectStage* GetIndirectStageAry();
            TexSRT* GetIndTexSRTAry();
            TevStage* GetTevStageAry();

            const char* GetName() const {
                return mName;
            }

            bool IsUserAllocated() const {
                return mbUserAllocated;
            }

            bool IsChanCtrlCap() const {
                return mGXMemCap.chanCtrl != 0;
            }

            bool IsMatColorCap() const {
                return mGXMemCap.matCol != 0;
            }

            bool IsTevSwapCap() const {
                return mGXMemCap.tevSwap != 0;
            }

            bool IsAlphaCompareCap() const {
                return mGXMemCap.alpComp != 0;
            }

            bool IsBlendModeCap() const {
                return mGXMemCap.blendMode != 0;
            }

            void SetName(const char *);
            void SetTextureNum(u8);
            void SetTexCoordGenNum(u8);
            void SetTevStageNum(u8);
            void SetIndStageNum(u8);
            void SetColorElement(u32, s16);

            const ut::Color GetMatColor() const {
                return GetMatColAry()[0];
            }

            AnimationList mAnimList;        // 0x4
            GXColorS10 mTevCols[3];
            ut::Color mTevKCols[4];
            detail::BitGXNums mGXMemCap;
            detail::BitGXNums mGXMemNum;
            void* mpGXMem;
            char mName[21];
            u8 mbUserAllocated;
        };
    };
};