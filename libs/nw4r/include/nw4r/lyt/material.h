#pragma once

#include "nw4r/lyt/animation.h"
#include "nw4r/lyt/types.h"
#include "nw4r/ut/Color.h"

namespace nw4r {
    namespace lyt {
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
