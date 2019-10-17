#ifndef ANIMATION_H
#define ANIMATION_H

#include "types.h"
#include "nw4r/ut/LinkList.h"

namespace nw4r
{

    namespace lyt
    {

        namespace res
        {
            struct AnimationBlock;
        };

        class AnimTransform
        {
        public:
            u16 GetFrameSize() const;
            bool IsLoopData() const;

            nw4r::ut::LinkListNode mLink;
            const res::AnimationBlock* mResBlock;
            f32 mFrame;
        };
    };
};

#endif // ANIMATION_H