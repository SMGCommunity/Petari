#pragma once

#include <revolution.h>
#include "nw4r/ut/LinkList.h"

namespace nw4r {
    namespace lyt {
        namespace res {
            class AnimationBlock;
        };

        class Material;
        class Pane;
        class ResourceAccessor;

        class AnimTransform {
        public:
            virtual ~AnimTransform();
            virtual void SetResource(const res::AnimationBlock *, ResourceAccessor *) = 0;
            virtual void SetResource(const res::AnimationBlock *, ResourceAccessor *, u16) = 0;
            virtual void Bind(Pane *, bool) = 0;
            virtual void Bind(Material *, bool) = 0;
            virtual void Animate(u32, Pane *) = 0;
            virtual void Animate(u32, Material *) = 0;

            ut::LinkListNode mLink;
            const res::AnimationBlock* mpRes;
            f32 mFrame;
        };

        namespace detail {
            AnimationLink* FindAnimationLink(AnimationList *, AnimTransform *);
            void UnbindAnimationLink(AnimationList *, AnimTransform *);
        }
    };
};
