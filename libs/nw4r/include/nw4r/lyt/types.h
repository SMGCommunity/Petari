#pragma once

#include "nw4r/math/types.h"
#include "nw4r/ut/LinkList.h"
#include <cstddef>

namespace nw4r {
    namespace lyt {

        struct Size {
            f32 width;
            f32 height;
        };

        class AnimTransform;

        class AnimationLink {
        public:

            AnimTransform* GetAnimTransform() const {
                return mAnimTrans;
            }

            ut::LinkListNode mLink;
        private:
            AnimTransform* mAnimTrans;
            u16 mIdx;
            bool mbDisable;
        };

        typedef ut::LinkList<AnimationLink, offsetof(AnimationLink, mLink)> AnimationList; 
    };
};

typedef nw4r::math::VEC2 (*TexCoordQuad)[4];
typedef const nw4r::math::VEC2 (*ConstTexCoordQuad)[4];